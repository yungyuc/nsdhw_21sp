## Proposal for Pairs Trading 


A pairs trade is a trading strategy that involves matching a long position with a short position in two stocks with a high correlation. It uses statistical and technical analysis to seek out potential market-neutral profits.

In this term project, I would like to optmize the Pairs Trading computing engine developed by the classmate in my lab. He combined the pairs trading techinque with Taiwan Stock Market intraday high-frequency trading, whcih I think is an opportunity and is suitalbe for NSD term project.

### Basic information

Github repo : [https://github.com/q40603/pairs_trade_nsd](https://github.com/q40603/pairs_trade_nsd)

Pairs Trading always comes along with a term "Cointegration". The basic ideas of Cointegration is to test whether two time series move similarly. When applied to stock market, we can find two assets that move with cointegration, assign different weight at the assets respectively, and sell the high priced stock, buy the low priced stock. 

| two stock series | Coinegration series |
| -------- | -------- |
| ![](https://i.imgur.com/ajKsdUc.png)     | ![](https://i.imgur.com/2cPbJ7F.png)    |


By using this strategy, we can gain the profit in both bear market and bull market. However, trading the assets too frequesntly may suffers from the huge trading cost.  








### Problem to solve


#### Prerequiement

* For each trading day ( 交易日 ), we can trade the stock from 9:00 ~ 13:30
* To test whether two stock price has cointegration, we must collect sufficient data to do cointegration test.
* We choose the top 150 company stock price as our input data.
* Some of the stock suffers from bad liquidity, which means there exists empty value in our data. So we compute the average price within each minute, and backfill the data with empty value.
* We collect the data between 9:16 ~ 11:46 to do cointegration test
* Trade the stock from 11:46~13:30

The original version of the Pairs Trading computing engine is written in Python, and it takes about 2 minutes to do the computing:
* We have to test every two stock of top 150 company (11175 combinations)

#### Math Part
Consider a K-dimensional  Gaussian VAR(p) model with a trend component:

![](https://i.imgur.com/gfkwosJ.png)
![](https://i.imgur.com/r9i2H42.png)
Then we have the vector error correction model (VECM) representation:

![](https://i.imgur.com/jHDZEFU.png)


Johansen (1995) show five types of VECM models. 
![](https://i.imgur.com/q0DDp8O.png)


Select pairs by cointegration approach:
1. unit-root test
    ADF test
2. information criteria 
    BIC test
5. Johansen test for cointegration
6. Likelihood ratio test for selecting adequate VECM model





#### What I want to imporve

* The structure of the code
* CI procedure
* The computaion speed



### Perspective users


Trader or the students under department of Finance.

### System architecture

input be like:

stock table
| | 2330 (TSMC) | 2454(MTK) |... |
| -------- | -------- | -------- | -------- |
| 1st min average price     | 612     | 933     |...     |
| 2nd min average price     | 614     | 930     |...     |

1. Stock raw tick data preprocessing
2. VAR model test
3. portmanteau test
4. Normality test
5. model selection
6. calculate weight of two stocks
7. output

pairs table
| id | stock1 | stock2 | weight 1 | weight2 | mean | standard deviation |
| -------- | -------- | -------- | -------- |-------- |-------- |-------- |
| 1| 2330     | 2454     | 0.3...    | 0.6... | 2.3... |  0.1... |
|  2| 2317     | 2308     | 0.1...     | 0.8... |  1.7... | 0.2... |


### API description


Python : top-level API wrapped from C++ code


| Name | input | return |description |
| -------- | -------- | -------- |  -------- |
| cal_pairs     |  stock table (pandas object)  | pairs table  |  user may import the function input the table mentioned above to do the calculation  |   |
| back_test | pairs table| profit, win rate  | user may calculate the profit with the function  | 


C++ : 
| Name | input | return |description |
| -------- | -------- | -------- |  -------- |
| ADF_test     |  series  | bool  |  check if the series is stationary  |   |
| VAR | series  | return model object |vector autoregrssive model, there are some existing library for this like statsmodels.tsa.api |
| order_select | series  |  int  | select best BIC order  |
| rank | data,model,p | int | test for each model |

There should be more, but I am not sure what is the best way to modularize it.


### Engineering infrastructure

The basic goal of this project is to speed up the original python version work.
Since there are many vector manipulation, I will make use of MKL library and also other existing C++ statisic libraries. The final goal is to make the user import the function from python to use. I will also add some Unit test and even CI to this project. Since this is a huge project (in my opinion), I will make use of TDD principles and do whatever the best I can do.

### Schedule

| date | work | comment |
| -------- | -------- | -------- |
| 4/7 ~ 4/30   | Input data processing, modularize the computation (design part)    |
| 5/1 ~ 5/15 | ADF test ,VAR model, order select |
| 5/15 ~ presentation| VECM weight calcualtion | 



### Question

1. In Numerical Software Development, are there any special design pattern for us to follow?
2. What is the best way to moduralize the whole computaion
3. In numerical computaion, many function relys on the previous function output, can we use stub software testing techinque in Numerical Software Devlopment?

### References

[1. Johansen-Test-for-Cointegrating](https://www.quantstart.com/articles/Johansen-Test-for-Cointegrating-Time-Series-Analysis-in-R//)