#include "mathf.h"


std::string Mathf::TickNumberToString(float number, int decimal)
{
    if (number >= 1e9)
    {
        return std::string("NaN");
    }

    char a[50];
    int tail_a = 0;
    // 负号
    if (number < 0)
    {
        a[tail_a++] = '-';
        number = -number;
    }
    // 整数部分
    int tail_part_1 = tail_a;
    int duplicate = number + number * 1e-9;
    for (int i = 1e9; i >= 1; i /= 10)
    {
        if (duplicate < i && tail_a == tail_part_1)
        {
            continue;
        }

        int digit = static_cast<int>(duplicate) / i;
        a[tail_a++] = '0' + digit;
        duplicate -= digit * i;
    }
    if (tail_a == tail_part_1)
    {
        a[tail_a++] = '0';
    }
    // 小数部分
    int tail_part_2 = tail_a;
    int decimal_digit_count = 0;
    duplicate = (number - static_cast<int>(number)) * 1e9 + (number - static_cast<int>(number)) * 1e4;
    for (int i = 1e8; i >= 1; i /= 10)
    {
        if (duplicate <= number * 1e5 || duplicate >= 1e9)
        {
            break;
        }
        if (tail_a == tail_part_2)
        {
            a[tail_a++] = '.';
        }

        int digit = static_cast<int>(duplicate) / i;
        a[tail_a++] = '0' + digit;
        duplicate -= digit * i;
        // 限制小数点后位数
        if (decimal > 0 && ++decimal_digit_count >= decimal)
        {
            break;
        }
    }
    // 尾巴
    a[tail_a] = 0;

    std::string result = std::string(a);
    return result;
}

std::string TickNumberToString_Obsolete(float number)
{
    if (number >= 1e9)
    {
        return std::string("NaN");
    }

    char a[50];
    int tail_a = 0;
    // 负号
    if (number < 0)
    {
        a[tail_a++] = '-';
        number = -number;
    }
    // 整数部分
    int tail_part_1 = tail_a;
    int duplicate = number + number * 1e-9;
    for (int i = 1e9; i >= 1; i /= 10)
    {
        if (duplicate < i && tail_a == tail_part_1)
        {
            continue;
        }

        int digit = static_cast<int>(duplicate) / i;
        a[tail_a++] = '0' + digit;
        duplicate -= digit * i;
    }
    if (tail_a == tail_part_1)
    {
        a[tail_a++] = '0';
    }
    // 小数部分
    int tail_part_2 = tail_a;
    duplicate = (number - static_cast<int>(number)) * 1e9 + (number - static_cast<int>(number)) * 1e4;
    for (int i = 1e8; i >= 1; i /= 10)
    {
        if (duplicate <= number * 1e5 || duplicate >= 1e9)
        {
            break;
        }
        if (tail_a == tail_part_2)
        {
            a[tail_a++] = '.';
        }

        int digit = static_cast<int>(duplicate) / i;
        a[tail_a++] = '0' + digit;
        duplicate -= digit * i;
    }
    // 尾巴
    a[tail_a] = 0;

    std::string result = std::string(a);
    return result;
}