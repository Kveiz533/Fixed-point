#include "parser.h"


int kAmountOfArgsInFirstCase = 3;
int kAmountOfArgsInSecondCase = 5;


void Parser::ParseFirstCase(char** argv, WorkNumFixedPoint& worker) {
    NumberFixedPoint num;

    ABParsing(argv[1], worker, num);
    RoundingParsing(argv[2], worker);
    NumberParsing(argv[3], worker, num);

    worker.Print(num,  worker.IsNeg(num));
}

void Parser::ParseSecondCase(char** argv, WorkNumFixedPoint& worker) {
    NumberFixedPoint num1, num2;

    ABParsing(argv[1], worker, num1);
    num2.A = num1.A;
    num2.B = num1.B;
    RoundingParsing(argv[2], worker);
    OperationParsing(argv[3], worker);
    NumberParsing(argv[4], worker, num1);
    NumberParsing(argv[5], worker, num2);

    worker.MakeOperation(num1, num2);
}

void Parser::ABParsing(const std::string& arg, WorkNumFixedPoint& worker, NumberFixedPoint& num) {
    if (std::count(arg.begin(), arg.end(), '.') == 1) {
        std::string part1 = arg.substr(0, arg.find('.'));
        std::string part2 = arg.substr(arg.find('.') + 1, arg.length() - arg.find('.'));

        if (worker.IsDecNumber(part1) && worker.IsDecNumber(part2)) {
            try {
                int a = std::stoi(part1);
                int b = std::stoi(part2);  
                if (a + b <= 32) {
                    num.A = a;
                    num.B = b;
                    return;
                } 

            } catch (const std::out_of_range& obj) {
                std::cerr << "Values A and B are out of range" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
    }
    std::cerr << "Invalid A and B values" << std::endl;
    exit(EXIT_FAILURE);
}

void Parser::RoundingParsing(const std::string& arg, WorkNumFixedPoint& worker) {
    bool is_correct = false;
    for (int i = 0; i < kRoundingValues_.size(); i++) {
        if (kRoundingValues_[i] == arg[0]) {
            worker.SetRoundingType(kRoundingValues_[i] - '0');
            is_correct = true;
            break;
        }
    }

    if (!is_correct || arg.length() != 1) {
        std::cerr << "Incorrect rounding value" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Parser::OperationParsing(const std::string& arg, WorkNumFixedPoint& worker) {
    bool is_correct = false;
    for (int i = 0; i < kOperationsValues_.size(); i++) {
        if (kOperationsValues_[i] == arg[0]) {
            worker.SetOperationType(kOperationsValues_[i]);
            is_correct = true;
            break;
        }
    }

    if (!is_correct || arg.length() != 1) {
        std::cerr << "Incorrect operation value" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Parser::NumberParsing(const std::string& arg, WorkNumFixedPoint& worker, NumberFixedPoint& num) {
    if (arg.length() <= 10 && arg.length() >= 3 && arg[0] == '0' &&
     arg[1] == 'x' && worker.IsHexNumber(arg)) {
        size_t pos;
        num.value = std::stoul(arg, &pos, 16);
        int shift = (32 - (num.A + num.B));
        num.value = (num.value << shift) >> (shift);
        return;
    }
    std::cerr << "Invalid number value" << std::endl;
    exit(EXIT_FAILURE);
}

