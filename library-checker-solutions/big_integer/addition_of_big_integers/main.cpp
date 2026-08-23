#include <bits/stdc++.h>

using namespace std;

string normalize(string value) {
    int start = 0;
    while (start + 1 < (int)value.size() && value[start] == '0') {
        start++;
    }
    return value.substr(start);
}

int compare(const string& a, const string& b) {
    if (a.size() != b.size()) {
        return a.size() < b.size() ? -1 : 1;
    }
    if (a == b) {
        return 0;
    }
    return a < b ? -1 : 1;
}

string addAbsolute(string a, string b) {
    reverse(a.begin(), a.end());
    reverse(b.begin(), b.end());
    string answer;
    int carry = 0;
    for (int i = 0; i < (int)max(a.size(), b.size()) || carry; i++) {
        int value = carry;
        if (i < (int)a.size()) {
            value += a[i] - '0';
        }
        if (i < (int)b.size()) {
            value += b[i] - '0';
        }
        answer.push_back(char('0' + value % 10));
        carry = value / 10;
    }
    reverse(answer.begin(), answer.end());
    return answer;
}

string subtractAbsolute(string a, string b) {
    reverse(a.begin(), a.end());
    reverse(b.begin(), b.end());
    string answer;
    int borrow = 0;
    for (int i = 0; i < (int)a.size(); i++) {
        int value = a[i] - '0' - borrow - (i < (int)b.size() ? b[i] - '0' : 0);
        if (value < 0) {
            value += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        answer.push_back(char('0' + value));
    }
    while (answer.size() > 1 && answer.back() == '0') {
        answer.pop_back();
    }
    reverse(answer.begin(), answer.end());
    return answer;
}

string addBigInteger(string a, string b) {
    bool negativeA = !a.empty() && a[0] == '-';
    bool negativeB = !b.empty() && b[0] == '-';
    if (negativeA) {
        a.erase(a.begin());
    }
    if (negativeB) {
        b.erase(b.begin());
    }
    a = normalize(a);
    b = normalize(b);

    string answer;
    bool negative = false;
    if (negativeA == negativeB) {
        answer = addAbsolute(a, b);
        negative = negativeA;
    } else {
        int order = compare(a, b);
        if (order >= 0) {
            answer = subtractAbsolute(a, b);
            negative = negativeA;
        } else {
            answer = subtractAbsolute(b, a);
            negative = negativeB;
        }
    }
    if (negative && answer != "0") {
        answer.insert(answer.begin(), '-');
    }
    return answer;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tests;
    cin >> tests;
    while (tests--) {
        string a, b;
        cin >> a >> b;
        cout << addBigInteger(a, b) << '\n';
    }
    return 0;
}
