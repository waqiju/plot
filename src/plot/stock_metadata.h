#pragma once
#include <string>


class StockMetadata
{
public:
    std::string code;
    std::string data;
    std::string time;
    // ohlc
    double open;
    double high;
    double low;
    double close;
    double vol;
    double amount;
};