#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "../topic_common.hpp"

class SuffixTree
{
  public:
    struct Node
    {
        // [left,right) 是父边在原串中的标签区间，parent 是父节点，suffixLink 是后缀链接。
        int left = 0, right = 0, parent = -1, suffixLink = -1;
        // children 把出边首字符映射到儿子节点。
        map<char, int> children;
        int length() const
        {
            // 返回当前压缩边长度。
            return right - left;
        }
    };

  private:
    struct State
    {
        // node 是当前边的下端节点，position 是从该边起点走过的字符数。
        int node, position;
    };

    // text 是建树所用原串，nodes 是后缀树节点池。
    string text;
    vector<Node> nodes;
    State active{0, 0};

    State go(State state, int left, int right)
    {
        // state 是活动点，[left,right) 是待匹配文本；返回匹配后的活动点，失败返回 {-1,-1}。
        while (left < right)
        {
            if (state.position == nodes[state.node].length())
            {
                auto it = nodes[state.node].children.find(text[left]);
                if (it == nodes[state.node].children.end())
                {
                    return {-1, -1};
                }
                state = {it->second, 0};
            }
            else
            {
                if (text[nodes[state.node].left + state.position] != text[left])
                {
                    return {-1, -1};
                }
                int length = min(right - left, nodes[state.node].length() - state.position);
                state.position += length;
                left += length;
            }
        }
        return state;
    }

    int split(State state)
    {
        // state 是活动点；若它在边内则分裂该边，返回活动点对应的显式节点。
        if (state.position == nodes[state.node].length())
        {
            return state.node;
        }
        if (!state.position)
        {
            return nodes[state.node].parent;
        }
        Node original = nodes[state.node];
        int middle = nodes.size();
        nodes.push_back({original.left, original.left + state.position, original.parent, -1, {}});
        nodes[original.parent].children[text[original.left]] = middle;
        nodes[middle].children[text[original.left + state.position]] = state.node;
        nodes[state.node].parent = middle;
        nodes[state.node].left += state.position;
        return middle;
    }

    int suffixLink(int node)
    {
        // node 是显式后缀树节点；惰性计算并返回它的后缀链接。
        if (nodes[node].suffixLink != -1)
        {
            return nodes[node].suffixLink;
        }
        if (nodes[node].parent == -1)
        {
            return 0;
        }
        int parLink = suffixLink(nodes[node].parent);
        int skip = nodes[node].parent == 0;
        return nodes[node].suffixLink =
                   split(go({parLink, nodes[parLink].length()}, nodes[node].left + skip, nodes[node].right));
    }

    void extend(int position)
    {
        // position 是本次追加字符在 text 中的下标；补齐所有新后缀。
        while (true)
        {
            State next = go(active, position, position + 1);
            if (next.node != -1)
            {
                active = next;
                return;
            }
            int middle = split(active), leaf = nodes.size();
            nodes.push_back({position, (int)text.size(), middle, -1, {}});
            nodes[middle].children[text[position]] = leaf;
            active.node = suffixLink(middle);
            active.position = nodes[active.node].length();
            if (!middle)
            {
                break;
            }
        }
    }

  public:
    // 需要显式叶子时，在末尾加入一个小于字母表中所有字符的哨兵。
    explicit SuffixTree(string text) : text(move(text))
    {
        // text 是原字符串；按顺序完成 Ukkonen 在线构造。
        nodes.reserve(this->text.size() * 2 + 1);
        nodes.push_back({0, 0, -1, 0, {}});
        for (int i = 0; i < this->text.size(); ++i)
        {
            extend(i);
        }
    }

    const vector<Node> &getNodes() const
    {
        // 返回后缀树节点池的只读引用，顺序与节点编号一致。
        return nodes;
    }
    const string &getText() const
    {
        // 返回建树所用的完整文本。
        return text;
    }
};
