namespace holdem {
    enum Suit {
        HighCard,           // 高牌
        Pair,               // 对子
        TwoPair,            // 两对
        ThreeOfAKind,       // 三条
        Straight,           // 顺子
        Flush,              // 同花
        FullHouse,          // 葫芦
        FourOfAKind,        // 铁支
        StraightFlush,      // 同花顺
        RoyalStraightFlush  // 同花大顺
    };

    enum CardType {
        Two = 0,
        Three = 1,
        Four = 2,
        Five = 3,
        Six = 4,
        Seven = 5,
        Eight = 6,
        Nine = 7,
        Ten = 8,
        Jack = 9,
        Queen = 10,
        King = 11,
        Ace = 12
    };

    enum Color {
        Heart = 0,    // ♥
        Spade = 1,    // ♠
        Club = 2,     // ♣
        Diamond = 3   // ♦
    };

    // 一张卡牌，用类型和颜色表示
    using Card = std::pair<CardType, Color>;

    bool operator<(const Card &lhs, const Card &rhs) {
        return lhs.first < rhs.first;
    }

    // 手牌分数，用牌型和分数判断，分数只对相同牌型有效
    using Score = std::pair<Suit, int>;

    bool operator<(const Score &lhs, const Score &rhs) {
        return lhs.first < rhs.first or (lhs.first == rhs.first and lhs.second < rhs.second);
    }

    // 手牌
    struct Hand {
        // 颜色，用位表示每张手牌是否存在
        std::array<short, 4> color{};
        // 数目，存放每种卡牌的数目
        std::array<short, 13> type{};
        void clear(){
            std::fill(color.begin(), color.end(), 0);
            std::fill(type.begin(), type.end(), 0);
        }
        void addCard(const Card& c){
            color[c.second] += (1 << c.first);
            type[c.first]++;
        }
        void removeCard(const Card& c){
            color[c.second] -= (1 << c.first);
            type[c.first]--;
        }
    };

    Score getType(const Hand &hand) {

        auto& colors = hand.color;
        auto& types = hand.type;
        // is RoyalStraightFlush
        for (short i = 0; i < 4; ++i) {
            if ((colors[i] & 0b111110000000000) == 0b111110000000000) {
                return {RoyalStraightFlush, 0};
            }
        }
        // is StraightFlush
        {
            int tv = 0b111110000000000;
            for (short i = Ace; i >= Five; --i) {
                for(int j = 0; j < 4; ++j){
                    if((colors[j] & tv) == tv){
                        return {StraightFlush, i};
                    }
                }
                tv >>= 1;
                if(__builtin_popcount(tv) < 5) tv |= (1 << 12);
            }
        }
        // is FourOfAKind
        {
            for (short i = Ace; i >= Two; --i) {
                if (types[i] >= 4) {
                    for (short j = Ace; j >= Two; --j) {
                        if (j == i) continue;
                        if (types[j] > 0) {
                            return {FourOfAKind, i * 13 + j};
                        }
                    }
                }
            }
        }
        // is FullHouse
        {
            for (short i = Ace; i >= Two; --i) {
                if (types[i] >= 3) {
                    for (short j = Ace; j >= Two; --j) {
                        if (j == i) continue;
                        if (types[j] >= 2) {
                            return {FullHouse, i * 13 + j};
                        }
                    }
                }
            }
        }
        // is Flush
        {
            int score = 0;
            for (short c = 0; c < 4; ++c) {
                if (__builtin_popcount(colors[c]) >= 5) {
                    int tScore = 0;
                    for (int j = Ace; j >= Two and __builtin_popcount(tScore) < 5; --j) {
                        tScore |= (colors[c] & (1 << j));
                    }
                    score = std::max(score, tScore);
                }
            }
            if (score) return {Flush, score};
        }
        // is Straight
        {
            for (int i = Ace; i >= Five; --i) {
                bool ok = true;
                for (int j = 0; j < 5 and ok; ++j) {
                    int p = (i + 13 - j) % 13;
                    if (types[p] == 0) {
                        ok = false;
                    }
                }
                if (ok) {
                    return {Straight, i};
                }
            }
        }
        // is ThreeOfAKind
        {
            for (int i = Ace; i >= Two; --i) {
                if (types[i] >= 3) {
                    std::vector<int> others;
                    for (int j = Ace; j >= Two and others.size() < 2; --j) {
                        if (types[j] > 0) {
                            others.push_back(j);
                        }
                    }
                    return {ThreeOfAKind, i * 169 + others[0] * 13 + others[1]};
                }
            }
        }
        // is TwoPair
        {
            std::vector<int> pairs;
            for (int i = Ace; i >= Two and pairs.size() < 2; --i) {
                if (types[i] >= 2) {
                    pairs.push_back(i);
                }
            }
            if (pairs.size() >= 2) {
                for (int i = Ace; i >= Two; --i) {
                    if (types[i] > 0) {
                        return {TwoPair, pairs[0] * 169 + pairs[1] * 13 + i};
                    }
                }
            }
        }
        // is Pair
        {
            for (int i = Ace; i >= Two; --i) {
                if (types[i] >= 2) {
                    int score = i;
                    for (int j = Ace, cnt = 0; j >= Two and cnt < 3; --j) {
                        if (j == i) continue;
                        if (types[j] > 0) {
                            ++cnt;
                            score = score * 13 + j;
                        }
                    }
                    return {Pair, score};
                }
            }
        }
        // HighCard
        int highScore = 0;
        for (int i = Ace, cnt = 0; i >= Two and cnt < 5; --i) {
            if (types[i]) {
                highScore = highScore * 13 + i;
                ++cnt;
            }
        }
        return {HighCard, highScore};
    }

    char cardType2char(CardType ct) {
        return "23456789TJQKA"[ct];
    }

    CardType char2cardType(char c) {
        switch (c) {
            case '2':
                return Two;
            case '3':
                return Three;
            case '4':
                return Four;
            case '5':
                return Five;
            case '6':
                return Six;
            case '7':
                return Seven;
            case '8':
                return Eight;
            case '9':
                return Nine;
            case 'T':
                return Ten;
            case 'J':
                return Jack;
            case 'Q':
                return Queen;
            case 'K':
                return King;
            default:
                return Ace;
        }
    }
}
