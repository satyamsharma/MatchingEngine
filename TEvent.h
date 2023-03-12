#pragma once

#include <string>
#include <list>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "Types.h"

template<bool isExec = false>
class TEvent
{
public:
    TEvent(ExecType e, uint32_t oid, const std::string& s, Side side, uint16_t qty, double px)
        : execType{e}, orderId{oid}, symbol{std::move(s)}, side{side}, qty{qty}, price{px}
    {}

    TEvent() = delete;
    ~TEvent() = default;

    // TEvent attributes
    ExecType execType;
    uint32_t orderId;
    std::string symbol;
    Side side;
    uint16_t qty;
    double price;

    // Self-reference to the OrderBook for O(1) cancel
    std::list<TEvent<>*>::iterator itr;

    template<bool enabled = isExec>
    void print(std::ostream& os, const TEvent& order, typename std::enable_if<!enabled>::type* = 0) const
    {
        os << static_cast<char>(order.execType) << " " << order.orderId;

        if (order.execType == ExecType::New)
        {
            os << " " << order.symbol << " " << static_cast<char>(order.side)
               << " " << order.qty << " " << order.price;
        }
    }

    template<bool enabled = isExec>
    void print(std::ostream& os, const TEvent& order, typename std::enable_if<enabled>::type* = 0) const
    {
        os << static_cast<char>(order.execType) << " " << order.orderId
           << " " << order.symbol
           << " " << order.qty<< " " << order.price;
    }

    friend std::ostream& operator<<(std::ostream& os, const TEvent& order)
    {
        os << std::fixed << std::setprecision(5);
        order.print(os, order);
        return os;
    }

    std::string toString() const {
        std::stringstream ss;
        ss << (*this);
        return ss.str();
    }
};

typedef TEvent<> Order;
typedef TEvent<true> Exec;