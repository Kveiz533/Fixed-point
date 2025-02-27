#include <cstdlib>
#include <iostream>
#include "parser.h"
#include "workNum.h"


const int kAmountOfArgsInFirstCase = 3;
const int kAmountOfArgsInSecondCase = 5;

int main(int argc, char**argv) {
    Parser parser;
    WorkNumFixedPoint worker;
    bool is_correct_number_of_args = false;
    bool is_worked_correctly = false;

    if (argc == kAmountOfArgsInFirstCase + 1) {
        is_correct_number_of_args = true;
        is_worked_correctly = parser.ParseFirstCase(argv, worker);
    } else if (argc == kAmountOfArgsInSecondCase + 1) {
        is_correct_number_of_args = true;
        is_worked_correctly = parser.ParseSecondCase(argv, worker);
    }     
    
    if (!is_worked_correctly) {
        return 1;
    } else if (!is_correct_number_of_args) { 
        std::cerr << "Wrong number of arguments" << std::endl;
        return 1;
    }

    return 0;
}