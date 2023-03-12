#pragma once

enum class Side : char {
    Bid = 'B',
    Ask = 'S'
};

enum class ExecType : char {
    Filled = 'F',
    Cancelled = 'X',
    New = 'P',
    ErrorReject = 'E'
};

constexpr char DUPLICATE_ERR[] = "Duplicate order id";