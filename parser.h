#pragma once

#include <iostream>
#include <vector>
#include "workNum.h"
#include "nfp.h"
#include <cstdint>
#include <algorithm>
#include <stdio.h>



class Parser {
    public:
        bool ParseFirstCase(char** argv, WorkNumFixedPoint& worker);
        bool ParseSecondCase(char** argv, WorkNumFixedPoint& worker);

        bool ABParsing(const char* arg, WorkNumFixedPoint& worker, NumberFixedPoint& num);
        bool RoundingParsing(const std::string& arg, WorkNumFixedPoint& worker);
        bool OperationParsing(const std::string& arg, WorkNumFixedPoint& worker);
        bool NumberParsing(const std::string& arg, WorkNumFixedPoint& worker, NumberFixedPoint& num);

    private:
        const std::vector<char> kRoundingValues_{'0', '1', '2', '3'};
        const std::vector<char> kOperationsValues_{'-', '+', '*', '/'};
};