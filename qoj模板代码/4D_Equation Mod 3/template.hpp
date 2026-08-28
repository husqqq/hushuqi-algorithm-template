#pragma once

// QOJ candidate template extracted from the verified standalone solver.
// Keep this file outside the formal template until it is approved.

#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int MAX_BITS = 2005;
bitset<MAX_BITS> allBits;

struct TernaryRow
{
    bitset<MAX_BITS> one, two, zero; // 分别记录系数为 1、2、0 的列。

    TernaryRow()
    {
        zero = allBits;
    }

    void set(int column, int value)
    {
        // column 是列下标，value 是 0、1、2 之一；设置该列的系数。
        one.reset(column);
        two.reset(column);
        zero.reset(column);
        if (value == 0)
        {
            zero.set(column);
        }
        else if (value == 1)
        {
            one.set(column);
        }
        else
        {
            two.set(column);
        }
    }

    int get(int column) const
    {
        // column 是列下标；返回该列的三元系数。
        return one[column] ? 1 : (two[column] ? 2 : 0);
    }

    void add(const TernaryRow &other)
    {
        // other 是另一行；把 other 加到当前行，所有系数按模 3 运算。
        auto newOne = (one & other.zero) | (two & other.two) | (zero & other.one);
        auto newTwo = (one & other.one) | (two & other.zero) | (zero & other.two);
        one = newOne;
        two = newTwo;
        zero = allBits ^ one ^ two;
    }

    void subtract(const TernaryRow &other)
    {
        // other 是另一行；从当前行减去 other，所有系数按模 3 运算。
        auto newOne = (one & other.zero) | (two & other.one) | (zero & other.two);
        auto newTwo = (one & other.two) | (two & other.zero) | (zero & other.one);
        one = newOne;
        two = newTwo;
        zero = allBits ^ one ^ two;
    }
};

vector<int> solveMod3(const vector<vector<int>> &matrix, int variables)
{
    // matrix 每行是 variables 个系数加一个常数项；返回一组解，输入须保证方程组可解。
    assert(0 <= variables && variables + 1 <= MAX_BITS);
    allBits.set();
    int equations = matrix.size();
    vector<TernaryRow> a(equations);
    for (int i = 0; i < equations; i++)
    {
        assert((int)matrix[i].size() == variables + 1);
        auto &row = a[i];
        for (int column = 0; column <= variables; column++)
        {
            int value = matrix[i][column] % 3;
            if (value < 0) value += 3;
            row.set(column, value);
        }
    }
    vector<int> pivotRow(variables, -1);
    int rank = 0;
    for (int column = 0; column < variables && rank < equations; column++)
    {
        int pivot = rank;
        while (pivot < equations && a[pivot].get(column) == 0)
        {
            pivot++;
        }
        if (pivot == equations)
        {
            continue;
        }
        swap(a[pivot], a[rank]);
        int pivotValue = a[rank].get(column);
        for (int row = 0; row < equations; row++)
        {
            if (row == rank)
            {
                continue;
            }
            int value = a[row].get(column);
            if (!value)
            {
                continue;
            }
            if (value == pivotValue)
            {
                a[row].subtract(a[rank]);
            }
            else
            {
                a[row].add(a[rank]);
            }
        }
        pivotRow[column] = rank++;
    }
    vector<int> answer(variables);
    for (int column = 0; column < variables; column++)
    {
        if (pivotRow[column] != -1)
        {
            auto &row = a[pivotRow[column]];
            answer[column] = row.get(column) * row.get(variables) % 3;
        }
    }
    return answer;
}
