===========================
Proposal for Bitcoin trading bot
===========================

Bitcoin continues making headlines by breaking the currency rate to USD. The current price of 
a single bitcoin dollar exceeds $50,000 US dollars continuously for the past month. Bitcoin is 
a great hedging instrument like gold as it has limited supply and have pros like difficult 
to manipulate as it is decentralized.

The cash/currency that we used every day are all issued by the government, it might not seems 
to be a big problem at a first glance. However, government-based currency can have 
quantitative easing(QE) when they think it's needed. The problem is the newly generated money ("new money") 
didn't go into the ordinary citizen's hand, instead, it goes to the enterprise.

Even though the amount of money increased after QE, but, ordinary citizens saving remains the same.
In other words, every time when QE being introduced, we all lost money. That's why it's needed to have 
hedging strategery no matter it's bitcoin or not.

Therefore, the reason that I choose this topic is because of hedging instrument, but also for speculation.
Because the currency fluctuation is so high that a buy and a sell can generate gains, so it is worth to
develop a trading bot specify for it.

Basic information
=================

The GitHub repository (URL):
https://github.com/oscar97123/Bitcoin-trading-bot

The project aims to develop an auto trading bot specially for bitcoin trading in order to gain profit 
from buying the coin at low price but selling high. More detailly, the program will analyze the market activities 
such as the trading throughput before making according decisions.

Since the project is solely focus on latency improvement, therefore, for simplicity, the program will validate the calculation latency 
improvement when performing the calculation on the last 30 days' historical price data in order to prevent real-time I/O 
system responce bottleneck.


Problem to solve
================

The idea of a trading bot is to automatically complete the buy and sell procedure based on the instructions given.
The instruction might be set as a strictly by human or much smartly, by analysing the exchange insight of 
recent market activities such as trading volume and Candlestick chart. Which, at most cases, is achieved by 
adapting machine learning for analysing the past month's market activity. RNN / LSTM are the machine learning 
model widely used.

In this project, I'm going to applies the latter approach as it is much intelligent that the program itself 
can have continous adjustment according to the recent market changes. Furthermore, I found the computation time 
can be improved significantly since one of the Crypto trading bot "freqtrade" is written in Python language.

The goals that I want to achieved for the project are as follow.

|1. 5% calculation time improvement compares to exist trading bot "freqtrade". <The comparing platform TO-BE determined>

|2. Reference others Cyprotocurrency market activity before making any buy/sell decision.

|3. Users can choose simulated trading mode / actual trading mode though Telegram bot or though the python API.

<<Math OR Algorithm>>
|<TO-DO>



Perspective users
=================

Any crypto traders and newbies to crypto trading.

The user may use the software though Telegram bot or though python API.


System architecture
===================

<TO-DO>

Analyze how your system takes input, produces results, and performs any other
operations.

Describe the system's work flow.  You may consider to use a flow chart but it
is not required.

Specify the constraints assume in your system.  Describe how it is modularized.

API description
===============

<TO-DO>

The telegram bot UI and function design is as follow:
<<<Image showcasing telegram bot UI>>>
<<<Describe python API>>>

Show how a user programmatically uses your system.  You are supposed to
implement the system using both C++ and Python.  Describe how a user writes a
script in the system.  If you provide API in both C++ and Python, describe
both.

Engineering infrastructure
==========================

There will be 2 branches in the development. The first branch(master) holds the stable version of the software(with unit test assurance).
The second branch(development) will be a workstation for developing features, moreover, the unit test will perform in this branch either. 
Only when the unit test passes and no crashes occurred, then the development branch merge into the master branch as a stable build. 

Here's the image showcasing the proposed Version Control.

.. image:: https://i.imgur.com/DlnArn8.png
                :alt: DlnArn8

The project will split into functions for maintainability and the function name will be meaningful to make sure the readability is on point. 
Besides, there will be comments on complex code.

Schedule
========

4/11 - 4/18(1 week)  - Research
4/19 - 5/2 (2 weeks) - 1st prototype (Integrate Binance's RSET API for getting 
real-time crypto price, simple transaction rule testing(eg. sell all bitcoin if the price raise 5%).
5/3  - 5/23(3 weeks) - C++ computation development and optimization + testing
5/24 - 5/31(1 week)  - High-level API development (Python + Telegram bot - if time is sufficient) + testing
6/1  - 6/6(aprx. 1 week) - Overall test, minor changes, bugfixes

References
==========

freqtrade:
https://www.freqtrade.io/en/stable/bot-usage/
