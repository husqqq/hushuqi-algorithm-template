#ifndef TEMPLATE_INLINE_0A198FBA8AF053107089
#define TEMPLATE_INLINE_0A198FBA8AF053107089

#include <bits/stdc++.h>

using namespace std;

class BinaryMatrix {
    int rows, columns, blocks;
    vector<vector<uint64_t>> data;

  public:
    BinaryMatrix(int rows = 0, int columns = 0)
        : rows(rows), columns(columns), blocks((columns + 63) / 64), data(rows, vector<uint64_t>(blocks)) {}

    int rowCount() const { return rows; }
    int columnCount() const { return columns; }

    bool get(int row, int column) const { return data[row][column / 64] >> (column % 64) & 1; }

    void set(int row, int column, bool value = true) {
        uint64_t mask = uint64_t{1} << (column % 64);
        if (value) data[row][column / 64] |= mask;
        else data[row][column / 64] &= ~mask;
    }

    void swapRows(int first, int second) { swap(data[first], data[second]); }

    void xorRows(int target, int source) {
        for (int block = 0; block < blocks; ++block) data[target][block] ^= data[source][block];
    }

    void xorRowFrom(int target, const BinaryMatrix& other, int source) {
        assert(columns == other.columns);
        for (int block = 0; block < blocks; ++block) data[target][block] ^= other.data[source][block];
    }

    static BinaryMatrix multiply(const BinaryMatrix& left, const BinaryMatrix& right) {
        assert(left.columns == right.rows);
        BinaryMatrix result(left.rows, right.columns);
        for (int row = 0; row < left.rows; ++row) {
            for (int middle = 0; middle < left.columns; ++middle) {
                if (left.get(row, middle)) result.xorRowFrom(row, right, middle);
            }
        }
        return result;
    }

    // Reduced row echelon form on the first coefficientColumns columns.
    pair<int, vector<int>> eliminate(int coefficientColumns) {
        int rank = 0;
        vector<int> pivotRow(coefficientColumns, -1);
        for (int column = 0; column < coefficientColumns && rank < rows; ++column) {
            int selected = rank;
            while (selected < rows && !get(selected, column)) ++selected;
            if (selected == rows) continue;
            swapRows(selected, rank);
            for (int row = 0; row < rows; ++row) {
                if (row != rank && get(row, column)) xorRows(row, rank);
            }
            pivotRow[column] = rank++;
        }
        return {rank, pivotRow};
    }

    string rowString(int row, int left = 0, int right = -1) const {
        if (right == -1) right = columns;
        string result;
        result.reserve(right - left);
        for (int column = left; column < right; ++column) result += char('0' + get(row, column));
        return result;
    }
};
#endif


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    BinaryMatrix matrix(n, m + 1);
    for (int row = 0; row < n; ++row) {
        string input;
        cin >> input;
        for (int column = 0; column < m; ++column) matrix.set(row, column, input[column] == '1');
    }
    string rightHandSide;
    cin >> rightHandSide;
    for (int row = 0; row < n; ++row) matrix.set(row, m, rightHandSide[row] == '1');

    auto [rank, pivotRow] = matrix.eliminate(m);
    for (int row = rank; row < n; ++row) {
        if (matrix.get(row, m)) return cout << -1 << '\n', 0;
    }
    string particular(m, '0');
    vector<int> freeColumns;
    for (int column = 0; column < m; ++column) {
        if (pivotRow[column] == -1) freeColumns.push_back(column);
        else particular[column] += matrix.get(pivotRow[column], m);
    }
    cout << freeColumns.size() << '\n' << particular << '\n';
    for (int freeColumn : freeColumns) {
        string basis(m, '0');
        basis[freeColumn] = '1';
        for (int column = 0; column < m; ++column) {
            if (pivotRow[column] != -1 && matrix.get(pivotRow[column], freeColumn)) basis[column] = '1';
        }
        cout << basis << '\n';
    }
}
