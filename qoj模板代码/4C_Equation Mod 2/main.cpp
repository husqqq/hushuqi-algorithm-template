#include <bits/stdc++.h>
using namespace std;

class FastInput
{
    static constexpr int BUFFER_SIZE = 1 << 20;
    char buffer[BUFFER_SIZE];
    int position = 0, length = 0;

  public:
    int readInt()
    {
        char c;
        do
        {
            if (position == length)
            {
                length = fread(buffer, 1, BUFFER_SIZE, stdin);
                position = 0;
            }
            c = buffer[position++];
        } while (c < '0' || c > '9');
        int value = 0;
        do
        {
            value = value * 10 + c - '0';
            if (position == length)
            {
                length = fread(buffer, 1, BUFFER_SIZE, stdin);
                position = 0;
            }
            c = buffer[position++];
        } while ('0' <= c && c <= '9');
        return value;
    }
};

int main()
{
    FastInput input;
    int variables = input.readInt();
    int equations = input.readInt();
    int words = (variables + 1 + 63) / 64;
    vector<vector<unsigned long long>> a(equations, vector<unsigned long long>(words));
    for (int i = 0; i < equations; i++)
    {
        for (int j = 0; j <= variables; j++)
        {
            if (input.readInt() & 1)
            {
                a[i][j >> 6] |= 1ULL << (j & 63);
            }
        }
    }

    vector<int> pivotColumn;
    int rank = 0;
    for (int column = 0; column < variables && rank < equations; column++)
    {
        int pivot = rank;
        while (pivot < equations && !((a[pivot][column >> 6] >> (column & 63)) & 1ULL))
        {
            pivot++;
        }
        if (pivot == equations)
        {
            continue;
        }
        swap(a[pivot], a[rank]);
        for (int row = rank + 1; row < equations; row++)
        {
            if (!((a[row][column >> 6] >> (column & 63)) & 1ULL))
            {
                continue;
            }
            for (int word = column >> 6; word < words; word++)
            {
                a[row][word] ^= a[rank][word];
            }
        }
        pivotColumn.push_back(column);
        rank++;
    }

    vector<unsigned long long> solution((variables + 63) / 64);
    for (int row = rank - 1; row >= 0; row--)
    {
        int column = pivotColumn[row];
        int value = (a[row][variables >> 6] >> (variables & 63)) & 1ULL;
        int firstWord = (column + 1) >> 6;
        if (firstWord < (int)solution.size())
        {
            unsigned long long mask = ~0ULL << ((column + 1) & 63);
            value ^= __builtin_parityll(a[row][firstWord] & solution[firstWord] & mask);
            for (int word = firstWord + 1; word < (int)solution.size(); word++)
            {
                value ^= __builtin_parityll(a[row][word] & solution[word]);
            }
        }
        if (value & 1)
        {
            solution[column >> 6] |= 1ULL << (column & 63);
        }
    }
    for (int i = 0; i < variables; i++)
    {
        if (i)
        {
            putchar(' ');
        }
        putchar('0' + ((solution[i >> 6] >> (i & 63)) & 1ULL));
    }
    putchar('\n');
}
