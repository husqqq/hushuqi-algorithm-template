// QOJ candidate template extracted from the verified standalone solver.
// Keep this file outside the formal template until it is approved.

#include <bits/stdc++.h>
using namespace std;

constexpr int MAX_BITS = 2005;
bitset<MAX_BITS> allBits;

struct TernaryRow
{
    bitset<MAX_BITS> one, two, zero;

    TernaryRow()
    {
        zero = allBits;
    }

    void set(int column, int value)
    {
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
        return one[column] ? 1 : (two[column] ? 2 : 0);
    }

    void add(const TernaryRow &other)
    {
        auto newOne = (one & other.zero) | (two & other.two) | (zero & other.one);
        auto newTwo = (one & other.one) | (two & other.zero) | (zero & other.two);
        one = newOne;
        two = newTwo;
        zero = allBits ^ one ^ two;
    }

    void subtract(const TernaryRow &other)
    {
        auto newOne = (one & other.zero) | (two & other.one) | (zero & other.two);
        auto newTwo = (one & other.two) | (two & other.zero) | (zero & other.one);
        one = newOne;
        two = newTwo;
        zero = allBits ^ one ^ two;
    }
};

inline void solveTernaryLinearSystem(istream &cin, ostream &cout)
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    allBits.set();
    int variables, equations;
    cin >> variables >> equations;
    vector<TernaryRow> a(equations);
    for (auto &row : a)
    {
        for (int column = 0; column <= variables; column++)
        {
            int value;
            cin >> value;
            row.set(column, value % 3);
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
    for (int column = 0; column < variables; column++)
    {
        int value = 0;
        if (pivotRow[column] != -1)
        {
            auto &row = a[pivotRow[column]];
            value = row.get(column) * row.get(variables) % 3;
        }
        cout << value << " \n"[column + 1 == variables];
    }
}
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solveTernaryLinearSystem(cin, cout);
}
