#include "parser.h"
#include <cstdint>




bool Parser::ParseFirstCase(char** argv, WorkNumFixedPoint& worker) {
    NumberFixedPoint num;

    bool is_ab_parsed = ABParsing(argv[1], worker, num);
    bool is_rounding_parsed = RoundingParsing(argv[2], worker);
    bool is_number_parsed = NumberParsing(argv[3], worker, num);

    if (is_ab_parsed && is_rounding_parsed && is_number_parsed) {
        worker.Print(num,  worker.IsNeg(num));
        return true;
    } 
    return false;
}

bool Parser::ParseSecondCase(char** argv, WorkNumFixedPoint& worker) {
    NumberFixedPoint num1, num2;

    bool is_ab_parsed = ABParsing(argv[1], worker, num1);
    num2.A = num1.A;
    num2.B = num1.B;
    bool is_rounding_parsed = RoundingParsing(argv[2], worker);
    bool is_operation_parsed = OperationParsing(argv[3], worker);
    bool is_number1_parsed = NumberParsing(argv[4], worker, num1);
    bool is_number2_parsed = NumberParsing(argv[5], worker, num2);

    if (is_ab_parsed && is_rounding_parsed && is_operation_parsed && is_number1_parsed && is_number2_parsed) {
        worker.MakeOperation(num1, num2);
        return true;
    } 
    return false;
}

bool Parser::ABParsing(const char* arg, WorkNumFixedPoint& worker, NumberFixedPoint& num) {
    uint32_t A = UINT32_MAX;
    uint32_t B = UINT32_MAX;
    sscanf(arg, "%u.%u", &A, &B);

    if (A + B <= 32) {
        num.A = A;
        num.B = B;
        return true;
    } 

    std::cerr << "Invalid A and B values" << std::endl;
    return false;
}

bool Parser::RoundingParsing(const std::string& arg, WorkNumFixedPoint& worker) {
    for (int i = 0; i < kRoundingValues_.size(); i++) {
        if (kRoundingValues_[i] == arg[0] && arg.length() == 1) {
            worker.SetRoundingType(kRoundingValues_[i] - '0');
            return true;
        }
    }

    std::cerr << "Incorrect rounding value" << std::endl;
    return false;
}

bool Parser::OperationParsing(const std::string& arg, WorkNumFixedPoint& worker) {
    for (int i = 0; i < kOperationsValues_.size(); i++) {
        if (kOperationsValues_[i] == arg[0] && arg.length() == 1) {
            worker.SetOperationType(kOperationsValues_[i]);
            return true;
        }
    }
    std::cerr << "Incorrect operation value" << std::endl;
    return false;
}

bool Parser::NumberParsing(const std::string& arg, WorkNumFixedPoint& worker, NumberFixedPoint& num) {
    if (arg.length() <= 10 && arg.length() >= 3 && arg[0] == '0' &&
     arg[1] == 'x' && worker.IsHexNumber(arg)) {
        size_t pos;
        num.value = std::stoul(arg, &pos, 16);
        int shift = (32 - (num.A + num.B));
        num.value = (num.value << shift) >> (shift);
        return true;
    }
    std::cerr << "Invalid number value" << std::endl;
    return false;
}

