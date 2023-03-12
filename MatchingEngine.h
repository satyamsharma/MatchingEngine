//
// Created by Satyam Sharma on 3/10/23.
//

#pragma once


#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <unordered_map>
#include "OrderBook.h"

using namespace std;
typedef std::list<std::string> results_t;

class MatchingEngine {
public:
    explicit MatchingEngine(results_t* result) { resultPayload = result; }

    std::unordered_map<uint32_t, Order> OrderIdToOrder;
    std::unordered_map<std::string, OrderBook> SymbolToOrderBook;

    results_t* resultPayload;

    void InsertOrder(uint32_t oid, const std::string& symbol, Side side, uint16_t qty, double px);
    void CancelOrder(uint32_t orderId);
    void LoadBookToPrint();
};
