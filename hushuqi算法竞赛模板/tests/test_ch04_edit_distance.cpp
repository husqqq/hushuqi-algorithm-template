#define NDEBUG
#include "../include/full/4_1_013.hpp"

void require(bool ok)
{
    if (!ok)
    {
        abort();
    }
}

bool checkDistance(const string &p, const string &t)
{
    // p、t 是任意字节串；核对 Myers 与滚动 DP 的全局编辑距离。
    int want = editDist(p, t);
    if (myersDist(p, t) != want)
    {
        return false;
    }
    if (t.size() <= 64 && myersDist(t, p) != want)
    {
        return false;
    }
    return true;
}

signed main()
{
    require(checkDistance("", ""));
    require(checkDistance("", string(257, '\0')));
    require(checkDistance(string(64, 'x'), string(63, 'x')));
    require(checkDistance(string(65, 'x'), string(200, 'x')));
    require(checkDistance(string(200, 'x'), string(65, 'x')));

    string p(64, '\0');
    p[0] = '\0';
    p[1] = (char)128;
    p[62] = (char)254;
    p[63] = (char)255;
    string t;
    for (int c = 0; c < 256; c++)
    {
        t.push_back((char)c);
    }
    require(checkDistance(p, t));
    require(myersDist(string(1, (char)255), string(256, (char)255)) == 255);

    mt19937_64 rng(20261021);
    for (int tc = 0; tc < 500; tc++)
    {
        string a(rng() % 65, '\0');
        string b(rng() % 96, '\0');
        for (char &c : a)
        {
            c = (char)rng();
        }
        for (char &c : b)
        {
            c = (char)rng();
        }
        require(checkDistance(a, b));
    }
}
