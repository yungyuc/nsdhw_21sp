#include <array>
#include <boost/asio.hpp>
#include <boost/version.hpp>
#include <boost/beast.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sstream>

using namespace std;
using namespace boost::asio;
using namespace boost::beast::http;
using namespace boost::algorithm;

io_service global_io_service;

const string ok_string = "HTTP/1.0 200 OK\r\n";
const string internal_server_error_string = "HTTP/1.0 500 Internal Server Error\r\n";
const char name_value_separator[] = { ':', ' ' };
const char crlf[] = { '\r', '\n' };
struct _header
{
  string name;
  string value;
};


class HttpSession : public enable_shared_from_this<HttpSession> {
    private:
        enum { max_length = 1024 };
        ip::tcp::socket _socket;
        array<char, max_length> _data;
        bool is_cgi = true;
        string query, cgi, method = {"GET"}, target, protocol, host;
        string s_addr, s_port, r_addr, r_port;
        stringstream ss;
        string header;
        reply _reply;

    public:
        HttpSession(ip::tcp::socket socket) : _socket(move(socket)) {}
        void start() { do_read(); }

    private:
        void do_read() {
            auto self(shared_from_this());
            _socket.async_read_some(buffer(_data, max_length),
            [this, self](boost::system::error_code ec, size_t length) 
            {
                if (!ec) { 
                    for(auto &s : _data){
                        ss << s;
                    }
                    header = ss.str();
                    parse();
                    run();
                }
            });
        }

        void parse() {
            boost::system::error_code ec;
            request_parser<string_body> p;
            p.put(buffer(header), ec);
            //cout<<p.get()<<endl;
            // cout << p.get().target() << endl;
            // cout << p.get().method() << endl;
            // cout << p.get()[field::access_control_request_method] << endl;
            // target = p.get()[field::target];

            boost::regex cgi_exp("[a-zA-Z0-9_]*.cgi");
            boost::smatch cgi_match;
            target = string(p.get().target());

            if (boost::regex_search(target, cgi_match, cgi_exp)){
              is_cgi = true;
              cgi = string(cgi_match[0].first, cgi_match[0].second);
              // method = p.get().method_string().data();
              host = string(p.get()[field::host]);
              host.erase(remove(host.begin(), host.end(), '\n'),host.end());

              protocol = to_string(p.get().version());
              protocol.insert(1,string("."));
              protocol = "HTTP/" + protocol;


              boost::regex query_exp("[?][^s].*");
              boost::smatch query_match;

              if (boost::regex_search(target, query_match, query_exp)){
                query = string(query_match[0].first, query_match[0].second);
                query.erase(remove(query.begin(), query.end(), '?'),query.end());
              }
            }
            else{
              is_cgi = false;
            }

            s_addr = _socket.local_endpoint().address().to_string();
            s_port = to_string(_socket.local_endpoint().port());
            r_addr = _socket.remote_endpoint().address().to_string();
            r_port = to_string(_socket.remote_endpoint().port());
        }
  
	void run() {
    if(is_cgi){
      
      pid_t pid;
      global_io_service.notify_fork(io_service::fork_prepare);
      while((pid = fork()) < 0){
        usleep(1000);
      }

      if(pid == 0) {
        int socket_fd = _socket.native_handle();
        global_io_service.notify_fork(io_service::fork_child);
        
        char* const arg[2] = {const_cast<char*>(cgi.c_str()), NULL};
        
        // dup2(_socket.native_handle(), 0);
        set_env();
        OK();
        dup2(socket_fd, 0);
        dup2(socket_fd, 1);
        dup2(socket_fd, 2);
        // execvp(("./"+cgi).c_str(), arg);
        if((execvp(("./"+cgi).c_str(), arg) < 0)){
          exit(EXIT_FAILURE);
        }        

      }
      else {
        global_io_service.notify_fork(io_service::fork_parent);
        // return;
      }
    }

    else {
      do_write();
    }
	}

	void set_env() {
		setenv("REQUEST_METHOD", method.c_str(), 1);
		setenv("REQUEST_URI", target.c_str(), 1);
		setenv("QUERY_STRING", query.c_str(), 1);
		setenv("SERVER_PROTOCOL", protocol.c_str(), 1);
		setenv("HTTP_HOST", host.c_str(), 1);
		setenv("SERVER_ADDR", s_addr.c_str(), 1);
		setenv("SERVER_PORT", s_port.c_str(), 1);
		setenv("REMOTE_ADDR", r_addr.c_str(), 1);
		setenv("REMOTE_PORT", r_port.c_str(), 1);

    // cout << "REQUEST_METHOD = " << method.c_str() << endl;
    // cout << "REQUEST_URI = " << target.c_str() << endl;
    // cout << "QUERY_STRING = " << query.c_str() << endl;
    // cout << "SERVER_PROTOCOL = " << protocol.c_str() << endl;
    // cout << "HTTP_HOST = " << host.c_str() << endl;
    // cout << "SERVER_ADDR = " << s_addr.c_str() << endl;
    // cout << "SERVER_PORT = " << s_port.c_str() << endl;
    // cout << "REMOTE_ADDR = " << r_addr.c_str() << endl;
    // cout << "REMOTE_PORT = " << r_port.c_str() << endl<<endl;
    // setenv("PATH", "/bin:./bin:.", 1);
	}

  void do_write() {
    auto self(shared_from_this());
    async_write(_socket, _reply.to_buffers(),
    [this, self](boost::system::error_code ec, size_t)
    {
        if (!ec)
        {
            // Initiate graceful connection closure.
            boost::system::error_code ignored_ec;
            _socket.shutdown(ip::tcp::socket::shutdown_both,
            ignored_ec);
        }
    });
  }
    void OK ()
    {
    auto self(shared_from_this());
    _socket.async_send(
        buffer("HTTP/1.1 200 OK\r\n", strlen("HTTP/1.1 200 OK\r\n")),
        [this, self](boost::system::error_code ec, size_t /* length */) {
        });
    }
};

class HttpServer {
 private:
  ip::tcp::acceptor _acceptor;
  ip::tcp::socket _socket;

 public:
  HttpServer(short port)
      : _acceptor(global_io_service, ip::tcp::endpoint(ip::tcp::v4(), port)),
        _socket(global_io_service) {
    do_accept();
  }

 private:
  void do_accept() {
    _acceptor.async_accept(_socket, [this](boost::system::error_code ec) {
      if (!ec) make_shared<HttpSession>(move(_socket))->start();

      do_accept();
    });
  }
};


int main(int argc, char* const argv[]) {
  if (argc != 2) {
    cerr << "Usage:" << argv[0] << " [port]" << endl;
    return 1;
  }
//  setenv("PATH", "/bin:./bin:.", 1);

  try {
    unsigned short port = atoi(argv[1]);
    HttpServer server(port);
    global_io_service.run();
  } catch (exception& e) {
    cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
