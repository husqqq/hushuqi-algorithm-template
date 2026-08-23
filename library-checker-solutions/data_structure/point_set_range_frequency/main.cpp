#ifndef TEMPLATE_INLINE_855FEFA704213503EFC8
#define TEMPLATE_INLINE_855FEFA704213503EFC8

#include <bits/stdc++.h>

using namespace std;

class DynamicFrequencyIndex {
    vector<int> values;
    unordered_map<int, int> identifier;
    vector<vector<int>> positions;
    vector<vector<int>> fenwick;

    void add(int value, int position, int delta) {
        int id = identifier.at(value);
        int index = lower_bound(positions[id].begin(), positions[id].end(), position) - positions[id].begin() + 1;
        for (int i = index; i < int(fenwick[id].size()); i += i & -i) fenwick[id][i] += delta;
    }

    int prefixFrequency(int id, int position) const {
        int index = lower_bound(positions[id].begin(), positions[id].end(), position) - positions[id].begin();
        int answer = 0;
        for (int i = index; i > 0; i -= i & -i) answer += fenwick[id][i];
        return answer;
    }

  public:
    DynamicFrequencyIndex(vector<int> initial, const vector<pair<int, int>>& possibleAssignments)
        : values(move(initial)) {
        vector<pair<int, int>> occurrences = possibleAssignments;
        occurrences.reserve(occurrences.size() + values.size());
        for (int position = 0; position < int(values.size()); ++position) {
            occurrences.emplace_back(position, values[position]);
        }
        for (auto [position, value] : occurrences) {
            auto [iterator, inserted] = identifier.emplace(value, identifier.size());
            if (inserted) positions.emplace_back();
            positions[iterator->second].push_back(position);
        }
        fenwick.resize(positions.size());
        for (int id = 0; id < int(positions.size()); ++id) {
            auto& indices = positions[id];
            sort(indices.begin(), indices.end());
            indices.erase(unique(indices.begin(), indices.end()), indices.end());
            fenwick[id].assign(indices.size() + 1, 0);
        }
        for (int position = 0; position < int(values.size()); ++position) add(values[position], position, 1);
    }

    void setValue(int position, int value) {
        add(values[position], position, -1);
        values[position] = value;
        add(value, position, 1);
    }

    int frequency(int left, int right, int value) const {
        auto iterator = identifier.find(value);
        if (iterator == identifier.end()) return 0;
        int id = iterator->second;
        return prefixFrequency(id, right) - prefixFrequency(id, left);
    }
};
#endif


struct FrequencyQuery {
    int type, first, second, value;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (auto& x : a) cin >> x;
    vector<FrequencyQuery> queries(q);
    vector<pair<int, int>> assignments;
    for (auto& query : queries) {
        cin >> query.type >> query.first >> query.second;
        if (query.type == 0) assignments.emplace_back(query.first, query.second);
        else cin >> query.value;
    }
    DynamicFrequencyIndex index(move(a), assignments);
    for (const auto& query : queries) {
        if (query.type == 0) index.setValue(query.first, query.second);
        else cout << index.frequency(query.first, query.second, query.value) << '\n';
    }
}
