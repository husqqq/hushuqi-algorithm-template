#include "../include/full/5_3_008.hpp"

signed main()
{
    assert(edgeConn(0, {}) == 0);
    assert(edgeConn(1, {{0, 0, 7}}) == 0);
    assert(edgeConn(3, {{0, 1, 2}}) == 0);
    assert(edgeConn(3, {{0, 1, 1}, {1, 2, 1}, {2, 0, 1}}) == 2);
    assert(edgeConn(2, {{0, 1, 3}, {0, 1, 4}}) == 7);

    assert(vertexConn({}) == 0);
    assert(vertexConn({{}}) == 0);
    assert(vertexConn({{1}, {0}, {}}) == 0);
    assert(vertexConn({{1}, {0, 2}, {1}}) == 1);
    assert(vertexConn({{1, 3}, {0, 2}, {1, 3}, {0, 2}}) == 2);
    assert(vertexConn({{1, 2, 3}, {0, 2, 3}, {0, 1, 3}, {0, 1, 2}}) == 3);
}
