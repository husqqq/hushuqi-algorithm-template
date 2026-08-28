#include <bits/stdc++.h>
using namespace std;

struct GeneralizedSuffixAutomaton
{
    struct State
    {
        array<int, 26> next{};
        int link = -1;
        int length = 0;
    };

    vector<State> state{{}};
    int last = 0;

    GeneralizedSuffixAutomaton()
    {
        state.reserve(2000005);
    }

    void beginString()
    {
        last = 0;
    }

    void extend(char character)
    {
        int c = character - 'a';
        int current = state.size();
        state.push_back({});
        state[current].length = state[last].length + 1;
        int p = last;
        last = current;
        while (p != -1 && !state[p].next[c])
        {
            state[p].next[c] = current;
            p = state[p].link;
        }
        if (p == -1)
        {
            state[current].link = 0;
            return;
        }
        int q = state[p].next[c];
        if (state[q].length == state[p].length + 1)
        {
            state[current].link = q;
            return;
        }
        int clone = state.size();
        state.push_back(state[q]);
        state[clone].length = state[p].length + 1;
        while (p != -1 && state[p].next[c] == q)
        {
            state[p].next[c] = clone;
            p = state[p].link;
        }
        state[q].link = state[current].link = clone;
    }

    long long distinctSubstrings() const
    {
        long long answer = 0;
        for (int i = 1; i < (int)state.size(); i++)
        {
            answer += state[i].length - state[state[i].link].length;
        }
        return answer;
    }
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int count;
    cin >> count;
    GeneralizedSuffixAutomaton sam;
    while (count--)
    {
        string s;
        cin >> s;
        sam.beginString();
        for (char c : s)
        {
            sam.extend(c);
        }
    }
    cout << sam.distinctSubstrings() << '\n';
}
