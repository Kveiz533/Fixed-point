#include "workNum.h"


int kPrecision = 19;


uint64_t WorkNumFixedPoint::Pow(uint64_t num, int pow) {
    uint64_t result = 1;
    for (int i = 0; i < pow; ++i) {
        result *= num;
    }
    return result;
}

bool WorkNumFixedPoint::IsNeg(NumberFixedPoint& num) {
    if (num.value >> (num.A + num.B - 1) == 1) {
        return true;
    }
    return false;
}

void WorkNumFixedPoint::MakeOperation(NumberFixedPoint& num1, NumberFixedPoint& num2) {
    bool is_neg1 = IsNeg(num1);
    bool is_neg2 = IsNeg(num2);
    bool is_res_neg = is_neg1 ^ is_neg2;

    int num_len = num1.A + num1.B;
    int shift;

    NumberFixedPoint result;
    result.A = num1.A;
    result.B = num1.B;

    if (operation_type_ == '+' || operation_type_ == '-') {
        uint32_t operation_res;
        if (operation_type_ == '+') {
            operation_res = num1.value + num2.value;  
        } else {
            operation_res = num1.value - num2.value;
        }
        FormatingForSumOrMin(operation_res, num_len);
        result.value = operation_res;
        is_res_neg = result.value >> (num_len - 1);

    } else if (operation_type_ == '*' || operation_type_ == '/') {
        if (is_neg1) {
            num1.value = -num1.value;
            num1.value = num1.value << (32 - num_len) >> (32 - num_len);
        }

        if (is_neg2) {
            num2.value = -num2.value;
            num2.value = num2.value << (32 - num_len) >> (32 - num_len);
        }


        uint64_t operation_res;
        if (operation_type_ == '*') {
            operation_res = (uint64_t) num1.value * num2.value;
            shift = num1.B;
        } else {
            if (num2.value == 0) {
                std::cout << "div_by_zero" << std::endl;
                exit(EXIT_SUCCESS);
            }

            uint64_t divised = (uint64_t) num1.value << (64 - num_len);
            operation_res = divised / num2.value;
            shift = 64 - result.A  - 2 * result.B;
        }

        FormatingForMultOrDiv(operation_res, is_res_neg,  shift);

        if (is_res_neg) {
            operation_res = -operation_res;
        }

        operation_res = operation_res << (64 - num_len) >> (64 - num_len);
        result.value = operation_res;
        is_res_neg = IsNeg(result);
    }

    Print(result, is_res_neg);
}

bool WorkNumFixedPoint::IsDecNumber(const std::string& line) {
    for (int i = 0 ; i < line.length(); i++) {
        if (!std::isdigit(line[i])) {
            return false;
        }
    }
    return true;
}

bool WorkNumFixedPoint::IsHexNumber(const std::string& line) {
    int cnt = 0;
    for (int i = 2 ; i < line.length(); i++) {
        if (std::isdigit(line[i]) || (line[i] >= 'A' && line[i] <= 'F') ||
         (line[i] >= 'a' && line[i] <= 'f')) {
            ++cnt;
        }
    }
    return cnt == (line.length() - 2);
}

void WorkNumFixedPoint::SetOperationType(const char& operation_type) {
    operation_type_ = operation_type;
}

void WorkNumFixedPoint::SetRoundingType(const int& rounding_type) {
    rounding_type_ = rounding_type;
}

void WorkNumFixedPoint::FormatingForSumOrMin(uint32_t& num, int& num_len) {
    num = (num << (32 - num_len) >> (32 - num_len));
}

void WorkNumFixedPoint::FormatingForMultOrDiv(uint64_t& num, bool is_neg, int shift) {
    if (rounding_type_ == 0) {
        TowardZeroBin(num, shift);
    } else if (rounding_type_ == 1) {
        TowardNearestEvenBin(num, shift);
    } else if (rounding_type_ == 2) {
        TowardPosInfinityBin(num, is_neg, shift);
    } else {
        TowardNegInfinityBin(num, is_neg, shift);
    }
}

void WorkNumFixedPoint::FormatingForPrint(uint64_t& fractional, bool is_neg, int len) {
    if (rounding_type_ == 0) {
        TowardZero(fractional, len);
    } else if (rounding_type_ == 1) {
        TowardNearestEven(fractional, len);
    } else if (rounding_type_ == 2) {
        TowardPosInfinity(fractional, is_neg, len);
    } else {
        TowardNegInfinity(fractional, is_neg, len);
    }
}

void WorkNumFixedPoint::Print(NumberFixedPoint& num, bool is_neg) {
    uint32_t A_part;
    uint32_t B_part;

    uint64_t fractional_part = 0;
    int len = 0;

    if (is_neg) {
        A_part = -num.value << (32 - num.A - num.B) >> (32 - num.A);
        B_part = -num.value << (32 - num.B);
    } else {
        A_part = num.value >> num.B;
        B_part = num.value << (32 - num.B);
    }

    for (int i = 1; i <= std::min(num.B, kPrecision); i++) {
        uint32_t bit = B_part >> (31);
        fractional_part = fractional_part * 10 + Pow(5, i) * (bit); 
        len++;
        B_part <<= 1;
    }

    std::string final_result;
    int amount_zeros_before_frac;

    if (len >= 4) {
        FormatingForPrint(fractional_part, is_neg, len);
        int res_len_after_formating = std::to_string(fractional_part).length();
        if (res_len_after_formating <= 3) {
            amount_zeros_before_frac = (3 - res_len_after_formating);
        } else {
            amount_zeros_before_frac = 3;
        }
        A_part += fractional_part / 1000;

    } else {
        amount_zeros_before_frac = abs( len - (int) std::to_string(fractional_part).length()); 
    }

    std::vector<char> zeros_aft(std::max(3 - len, 0), '0'); 
    std::string zeros_after_frac(zeros_aft.begin(), zeros_aft.end());

    std::vector<char> zeros_bef(amount_zeros_before_frac, '0');
    std::string zeros_before_frac(zeros_bef.begin(), zeros_bef.end());

    std::string rest = (zeros_before_frac + std::to_string(fractional_part) + zeros_after_frac).substr(0, 3);

    final_result = std::to_string(A_part) + "." + rest;
    if (is_neg && final_result != "0.000") {
        final_result = "-" + final_result;
    }

    std::cout << final_result << std::endl;
}

void WorkNumFixedPoint::TowardZero(uint64_t& fractional, int len) {
    fractional /= Pow(10, (len - 3));
}

void WorkNumFixedPoint::TowardZeroBin(uint64_t& num, int shift) {
    num >>= shift;
}

void WorkNumFixedPoint::TowardNegInfinity(uint64_t& fractional, bool is_neg, int len) {
    uint64_t discarded_part = fractional % Pow(10, len - 3);

    fractional /= Pow(10, (len - 3));

    if (is_neg && discarded_part != 0) { 
        fractional += 1;
    } 
}

void WorkNumFixedPoint::TowardNegInfinityBin(uint64_t& num, bool is_neg, int shift) {
    uint64_t discarded_part = num & (Pow(2, shift) - 1);
    num >>= shift;

    if (is_neg && discarded_part != 0) { 
        num += 1;
    } 
}

void WorkNumFixedPoint::TowardPosInfinity(uint64_t& fractional, bool is_neg, int len) {
    uint64_t discarded_part = fractional % Pow(10, len - 3);
    fractional /= Pow(10, (len - 3));

    if (!is_neg && discarded_part != 0) { 
        fractional += 1;
    } 
}

void WorkNumFixedPoint::TowardPosInfinityBin(uint64_t& num, bool is_neg, int shift) {
    uint64_t discarded_part = num & (Pow(2, shift) - 1);
    num >>= shift;

    if (!is_neg && discarded_part != 0) {
        num += 1;
    }
}

void WorkNumFixedPoint::TowardNearestEven(uint64_t& fractional, int len) {
    uint64_t discarded_part = fractional % Pow(10, len - 3);
    fractional /= Pow(10, (len - 3));

    uint64_t first_bit_after_B_part = discarded_part / Pow(10, len - 4);
    uint64_t all_bits_after_first_bit_after_B_part = discarded_part % Pow(10, len - 4);
    bool is_B_part_even = (fractional % 2 == 0);
        
    if (first_bit_after_B_part > 5) {
        fractional += 1;
    } else if (first_bit_after_B_part == 5) {
        if (all_bits_after_first_bit_after_B_part) {
            fractional += 1;
        } else {
            if (!is_B_part_even) {
                fractional += 1;
            }
        }
    }
}

void WorkNumFixedPoint::TowardNearestEvenBin(uint64_t& num, int shift) {
    uint64_t discarded_part = num & (Pow(2, shift) - 1);
    num >>= shift;

    uint64_t first_bit_after_B_part = (discarded_part & (Pow(2, shift - 1)));
    uint64_t all_bits_after_first_bit_after_B_part = (discarded_part & (Pow(2, shift - 1) - 1));
    uint64_t last_B_part_bit = (num & 1);

    if (first_bit_after_B_part != 0) {
        if (all_bits_after_first_bit_after_B_part != 0) {
            num += 1;
        } else {
            if (last_B_part_bit != 0) {
                num += 1;
            }
        }
    }
}