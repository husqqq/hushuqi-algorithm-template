#include <bits/stdc++.h>
using namespace std;

constexpr int MOD = 1000000007;

long long addMod(long long a, long long b)
{
    a += b;
    if (a >= MOD)
    {
        a -= MOD;
    }
    return a;
}

class Min25Xor
{
    long long n;
    int squareRoot;
    vector<int> primes;
    vector<int> composite;
    vector<long long> quotient;
    vector<int> smallId, largeId;
    vector<long long> primeCount, primeSum;

    int id(long long x) const
    {
        return x <= squareRoot ? smallId[x] : largeId[n / x];
    }

    long long primeContribution(int index) const
    {
        if (quotient[index] < 2)
        {
            return 0;
        }
        return (2 + primeSum[index] - primeCount[index] + MOD) % MOD;
    }

    long long enumerate(long long limit, int previousPrime) const
    {
        if (previousPrime >= (int)primes.size() ||
            (previousPrime >= 0 && limit <= primes[previousPrime]))
        {
            return 0;
        }
        long long before = previousPrime < 0 ? 0 : primeContribution(id(primes[previousPrime]));
        long long result = (primeContribution(id(limit)) - before + MOD) % MOD;
        for (int i = previousPrime + 1; i < (int)primes.size() && primes[i] <= limit / primes[i]; i++)
        {
            long long power = primes[i];
            for (int exponent = 1; power <= limit / primes[i]; exponent++)
            {
                long long current = primes[i] ^ exponent;
                long long next = primes[i] ^ (exponent + 1);
                result = (result + enumerate(limit / power, i) * current + next) % MOD;
                if (power > limit / primes[i])
                {
                    break;
                }
                power *= primes[i];
            }
        }
        return result;
    }

  public:
    explicit Min25Xor(long long upper) : n(upper), squareRoot(sqrtl(upper)), composite(squareRoot + 1),
                                        smallId(squareRoot + 1), largeId(squareRoot + 1)
    {
        while ((long long)(squareRoot + 1) * (squareRoot + 1) <= n)
        {
            squareRoot++;
        }
        while ((long long)squareRoot * squareRoot > n)
        {
            squareRoot--;
        }
        for (int i = 2; i <= squareRoot; i++)
        {
            if (!composite[i])
            {
                primes.push_back(i);
            }
            for (int p : primes)
            {
                if ((long long)i * p > squareRoot)
                {
                    break;
                }
                composite[i * p] = true;
                if (i % p == 0)
                {
                    break;
                }
            }
        }
        for (long long left = 1, right; left <= n; left = right + 1)
        {
            long long value = n / left;
            right = n / value;
            int index = quotient.size();
            quotient.push_back(value);
            (value <= squareRoot ? smallId[value] : largeId[n / value]) = index;
            primeCount.push_back((value - 1) % MOD);
            primeSum.push_back(((unsigned __int128)value * (value + 1) / 2 - 1) % MOD);
        }
        for (int j = 0; j < (int)primes.size(); j++)
        {
            int p = primes[j];
            for (int i = 0; i < (int)quotient.size() && quotient[i] >= (long long)p * p; i++)
            {
                int reduced = id(quotient[i] / p);
                primeCount[i] = (primeCount[i] - primeCount[reduced] + j + MOD) % MOD;
                long long before = j ? primeSum[id(primes[j - 1])] : 0;
                primeSum[i] = (primeSum[i] - (long long)p * ((primeSum[reduced] - before + MOD) % MOD)) % MOD;
                if (primeSum[i] < 0)
                {
                    primeSum[i] += MOD;
                }
            }
        }
    }

    long long solve() const
    {
        if (n == 1)
        {
            return 1;
        }
        return (1 + enumerate(n, -1)) % MOD;
    }
};

int main()
{
    long long n;
    cin >> n;
    cout << Min25Xor(n).solve() << '\n';
}
