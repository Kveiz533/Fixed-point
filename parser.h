#pragma once

#include <iostream>
#include <vector>
#include "workNum.h"
#include "nfp.h"
#include <cstdint>
#include <algorithm>



class Parser {
    public:
        void ParseFirstCase(char** argv, WorkNumFixedPoint& worker);
        void ParseSecondCase(char** argv, WorkNumFixedPoint& worker);

        void ABParsing(const std::string& arg, WorkNumFixedPoint& worker, NumberFixedPoint& num);
        void RoundingParsing(const std::string& arg, WorkNumFixedPoint& worker);
        void OperationParsing(const std::string& arg, WorkNumFixedPoint& worker);
        void NumberParsing(const std::string& arg, WorkNumFixedPoint& worker, NumberFixedPoint& num);

    private:
        const std::vector<char> kRoundingValues_{'0', '1', '2', '3'};
        const std::vector<char> kOperationsValues_{'-', '+', '*', '/'};
};