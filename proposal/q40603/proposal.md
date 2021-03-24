Proposal for Pairs Trading 
===========================

A pairs trade is a trading strategy that involves matching a long position with a short position in two stocks with a high correlation. It uses statistical and technical analysis to seek out potential market-neutral profits.

In this term project, I would like to optmize the Pairs Trading computing engine developed by the classmate in my lab. He combined the pairs trading techinque with Taiwan Stock Market intraday high-frequency trading, whcih I think is an opportunity and is suitalbe for NSD term project.

Basic information
=================

Pairs Trading always comes along with a term "Cointegration". The basic ideas of Cointegration is to test whether two time series move similarly. When applied to stock market, we can find two assets that move with cointegration, assign different weight at the assets respectively, and sell the high priced stock, buy the low priced stock. 

| two stock series | Coinegration series |
| -------- | -------- |
| ![](https://i.imgur.com/ajKsdUc.png)     | ![](https://i.imgur.com/2cPbJ7F.png)    |


By using this strategy, we can gain the profit in both bear market and bull market. However, trading the assets too frequesntly may suffers from the huge trading cost.  








Problem to solve
================

### Prerequiement

* For each trading day ( 交易日 ), we can trade the stock from 9:00 ~ 13:30
* To test whether two stock price has cointegration, we must collect sufficient data to do cointegration test.
* We choose the top 150 company stock price as our input data.
* Some of the stock suffers from bad liquidity, which means there exists empty value in our data. So we compute the average price within each minute, and backfill the data with empty value.
* We collect the data between 9:16 ~ 11:46 to do cointegration test
* Trade the stock from 11:46~13:30

The original version of the Pairs Trading computing engine is written in Python, and it takes about 2 minutes to do the computing:
* We have to test every two stock of top 150 company (11175 combinations)

### Math Part

待補

### What I want to imporve

* The structure of the code
* CI procedure
* The computaion speed



Perspective users
=================

Trader or the students under department of Finance.

System architecture
===================
(TBD)

Analyze how your system takes input, produces results, and performs any other
operations.

Describe the system's work flow.  You may consider to use a flow chart but it
is not required.

Specify the constraints assume in your system.  Describe how it is modularized.

API description
===============
(TBD)
Show how a user programmatically uses your system.  You are supposed to
implement the system using both C++ and Python.  Describe how a user writes a
script in the system.  If you provide API in both C++ and Python, describe
both.

Engineering infrastructure
==========================
(TBD)
Describe how you plan to put together the build system, testing framework, and
documentation.  Show how you will do version control.

You may use continuous integration, but it is not required.  If you use it,
describe how it works in your code development.

Schedule
========
(TBD)
Itemize the work to do and list the work items in a timeline.  Estimate the
efforts of each item.

References
==========

List the external references for the information provided in the proposal.