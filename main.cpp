#include <cstdlib>
#include <iostream>
#include "parser.h"
#include "workNum.h"
#include <sstream>
#include <iterator>

extern int kAmountOfArgsInFirstCase;
extern int kAmountOfArgsInSecondCase;


std::vector<std::string> SplitString(const std::string& str) {
    std::istringstream iss(str);
    return {std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>()};
}


int main(int argc, char**argv) {
    Parser parser;
    WorkNumFixedPoint worker;

    if (argc == kAmountOfArgsInFirstCase + 1) {
        parser.ParseFirstCase(argv, worker);
    } else if (argc == kAmountOfArgsInSecondCase + 1) {
        parser.ParseSecondCase(argv, worker);
    } else {
        std::cerr << "Incorrect number of arguments" << std::endl;
        exit(EXIT_FAILURE);
    }

    return 0;
}