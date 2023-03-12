#pragma once

#include <list>
#include <unordered_map>
#include <iostream>
#include "TEvent.h"

class OrderBook {
public:
    std::list<Order*> bids;
    std::list<Order*> asks;

    friend std::ostream& operator<<(std::ostream& os, const OrderBook& orderBook)
    {
        for (auto it = orderBook.asks.rbegin(); it != orderBook.asks.rend(); ++it)
        {
            os << **it << '\n';
        }

        for (auto it = orderBook.bids.begin(); it != orderBook.bids.end(); ++it)
        {
            os << **it << '\n';
        }
        return os;
    }
};
