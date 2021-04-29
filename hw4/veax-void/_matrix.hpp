struct ByteCounterImpl {
    std::size_t allocated = 0;
    std::size_t deallocated = 0;

}; /* ByteCounterImpl */

class ByteCounter {
public:
    ByteCounter()
        : m_impl(new ByteCounterImpl) {}

    void increase(std::size_t amount) 
    { m_impl->allocated += amount; }

    void decrease(std::size_t amount) 
    { m_impl->deallocated += amount; }

    std::size_t bytes() const { return m_impl->allocated - m_impl->deallocated; }
    std::size_t allocated() const { return m_impl->allocated; }
    std::size_t deallocated() const { return m_impl->deallocated; }

private:
    ByteCounterImpl* m_impl;

}; /* ByteCounter */

template <class T>
struct MyAllocator {
    using value_type = T;
    // Just use the default constructor of 
    // ByteCounter for the data member "counter".
    MyAllocator() = default;

    T* allocate(std::size_t n) {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T)) {
            throw std::bad_alloc();
        }
        const std::size_t bytes = n * sizeof(T);
        T* p = static_cast<T*>(std::malloc(bytes));
        if (p) {
            counter.increase(bytes);
            return p;
        } else {
            throw std::bad_alloc();
        }
    }

    void deallocate(T* p, std::size_t n) noexcept {
        std::free(p);

        const std::size_t bytes = n * sizeof(T);
        counter.decrease(bytes);
    }

    ByteCounter counter;

}; /* MyAllocator */