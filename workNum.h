#pragma once 

#include <iostream>
#include "nfp.h"
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>


class WorkNumFixedPoint {
    public:
        bool IsNeg(NumberFixedPoint& num);
        bool IsHexNumber(const std::string& line);
        bool IsDecNumber(const std::string& line);
        uint64_t Pow(uint64_t num, int pow);

        void MakeOperation(NumberFixedPoint& num1, NumberFixedPoint& num2);

        void FormatingForSumOrMin(uint32_t& num, int& num_len);
        void FormatingForMultOrDiv(uint64_t& num, bool is_neg, int shift);
        void FormatingForPrint(uint64_t& fractional, bool is_neg, int len);

        void SetRoundingType(const int& rounding_type);
        void SetOperationType(const char& operation_type);

        void Print(NumberFixedPoint& num, bool is_neg);
    private:
        void TowardZeroBin(uint64_t& num, int shift);
        void TowardNearestEvenBin(uint64_t& num, int shift);
        void TowardPosInfinityBin(uint64_t& num, bool is_neg, int shift);
        void TowardNegInfinityBin(uint64_t& num, bool is_neg, int shift);

        void TowardZero(uint64_t& fractional, int len);
        void TowardNearestEven(uint64_t& fractional, int len);
        void TowardPosInfinity(uint64_t& fractional, bool is_neg, int len);
        void TowardNegInfinity(uint64_t& fractional, bool is_neg, int len);

        int rounding_type_ = 0;
        char operation_type_ = ' ';

};