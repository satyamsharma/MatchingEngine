//
// Created by Satyam Sharma on 3/10/23.
//

#include "MatchingEngine.h"

void MatchingEngine::InsertOrder(uint32_t oid, const std::string& symbol, Side side, uint16_t qty, double px)
{
    // Create OrderBook if it does not already exist
    OrderBook& book = SymbolToOrderBook[symbol];

    if (OrderIdToOrder.find(oid) != OrderIdToOrder.end())
    {
        stringstream ss;
        ss << "E ";
        ss << oid;
        ss << " " << DUPLICATE_ERR;

        resultPayload->push_back(ss.str());
        return;
    }

    OrderIdToOrder.emplace(
            std::make_pair(oid, Order{ExecType::New, oid, symbol, side, qty, px}));

    auto& order = OrderIdToOrder.find(oid)->second;

    // See if it can cross (list from the other side)
    if (order.side == Side::Bid)
    {
        auto askOrderItr = book.asks.begin();

        while (askOrderItr != book.asks.end() and order.qty > 0)
        {
            if (order.price >= (*askOrderItr)->price)
            {
                double tradePrice = (*askOrderItr)->price;
                uint16_t tradeQty = std::min((*askOrderItr)->qty, order.qty);

                resultPayload->push_back(Exec{ExecType::Filled, order.orderId, order.symbol,
                                              order.side, tradeQty, tradePrice}.toString());
                resultPayload->push_back(Exec{ExecType::Filled, (*askOrderItr)->orderId,
                                              (*askOrderItr)->symbol, (*askOrderItr)->side,
                                              tradeQty, tradePrice}.toString());

                if (tradeQty >= (*askOrderItr)->qty)
                {
                    book.asks.erase(askOrderItr++);
                }
                else
                {
                    (*askOrderItr)->qty -= tradeQty;
                }

                order.qty -= tradeQty;
            }
            else
            {
                break;
            }
        }

        if (order.qty > 0)
        {
            auto bidOrderItr = book.bids.begin();
            while (bidOrderItr != book.bids.end())
            {
                if (order.price > (*bidOrderItr)->price)
                {
                    break;
                }
                bidOrderItr++;
            }
            order.itr = book.bids.insert(bidOrderItr, &order);
        }
    }
    else if (order.side == Side::Ask)
    {
        auto bidOrderItr = book.bids.begin();

        while (bidOrderItr != book.bids.end() and order.qty > 0)
        {
            if (order.price <= (*bidOrderItr)->price)
            {
                double tradePrice = (*bidOrderItr)->price;
                uint16_t tradeQty = std::min((*bidOrderItr)->qty, order.qty);

                resultPayload->push_back(Exec {ExecType::Filled, order.orderId, order.symbol,
                                               order.side, tradeQty, tradePrice}.toString());
                resultPayload->push_back(Exec{ExecType::Filled, (*bidOrderItr)->orderId, (*bidOrderItr)->symbol,
                                              (*bidOrderItr)->side, tradeQty, tradePrice}.toString());

                if (tradeQty >= (*bidOrderItr)->qty)
                {
                    book.bids.erase(bidOrderItr++);
                }
                else
                {
                    (*bidOrderItr)->qty -= tradeQty;
                }

                order.qty -= tradeQty;
            }
            else
            {
                break;
            }
        }

        if (order.qty > 0)
        {
            auto askOrderItr = book.asks.begin();
            while (askOrderItr != book.asks.end())
            {
                if (order.price < (*askOrderItr)->price)
                {
                    break;
                }
                askOrderItr++;
            }
            order.itr = book.asks.insert(askOrderItr, &order);
        }
    }
}

void MatchingEngine::CancelOrder(uint32_t orderId)
{
    auto orderIdFound = OrderIdToOrder.find(orderId);
    auto order = orderIdFound->second;
    order.execType = ExecType::Cancelled;

    if (order.side == Side::Bid)
    {
        SymbolToOrderBook[order.symbol].bids.erase((order.itr));
    }
    else
    {
        SymbolToOrderBook[order.symbol].asks.erase((order.itr));
    }

    resultPayload->push_back(order.toString());
}

void MatchingEngine::LoadBookToPrint()
{
    std::stringstream ss;
    for (auto& book : SymbolToOrderBook)
    {
        ss << book.second;
        resultPayload->push_back(
                ss.str().substr(0, ss.str().size()-1));
    }
}