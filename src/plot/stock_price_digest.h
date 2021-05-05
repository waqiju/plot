#pragma once
#include <string>


class StockPriceDigest
{
public:
    std::string code;
    std::string date;
    std::string time;
    // ohlc
    double open;
    double high;
    double low;
    double close;
    double previous_close;
    double vol;
    double amount;
};