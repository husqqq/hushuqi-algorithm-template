#ifndef TEMPLATE_INLINE_EB9C44BDD08C0EDBA646
#define TEMPLATE_INLINE_EB9C44BDD08C0EDBA646

#ifndef TEMPLATE_INLINE_7A1D680384C339B9E7F1
#define TEMPLATE_INLINE_7A1D680384C339B9E7F1

#include <bits/stdc++.h>

using namespace std;

template <class T> struct Point2 {
    T x{}, y{};

    Point2 operator+(const Point2& other) const { return {x + other.x, y + other.y}; }
    Point2 operator-(const Point2& other) const { return {x - other.x, y - other.y}; }
    Point2 operator*(const T& factor) const { return {x * factor, y * factor}; }
    Point2& operator+=(const Point2& other) { return *this = *this + other; }
    Point2& operator-=(const Point2& other) { return *this = *this - other; }
    auto operator<=>(const Point2&) const = default;

    auto squaredLength() const { return x * x + y * y; }
};

template <class T> auto dot(Point2<T> first, Point2<T> second) {
    return first.x * second.x + first.y * second.y;
}

template <class T> auto cross(Point2<T> first, Point2<T> second) {
    return first.x * second.y - first.y * second.x;
}

template <class T> auto cross(Point2<T> origin, Point2<T> first, Point2<T> second) {
    return cross(first - origin, second - origin);
}
#endif


class MinimumEnclosingCircleSupport {
  public:
    using Point = Point2<long long>;

    enum class Position { outside = -1, boundary = 0, inside = 1 };

  private:
    vector<Point> support;

    static long long squaredDistance(Point first, Point second) { return (first - second).squaredLength(); }

    static Position diameterPosition(Point first, Point second, Point point) {
        Point doubled{2 * point.x - first.x - second.x, 2 * point.y - first.y - second.y};
        long long left = doubled.squaredLength();
        long long right = squaredDistance(first, second);
        if (left < right) return Position::inside;
        if (left == right) return Position::boundary;
        return Position::outside;
    }

    static long long incircleDeterminant(Point first, Point second, Point third, Point point) {
        first -= point;
        second -= point;
        third -= point;
        long long firstNorm = first.squaredLength();
        long long secondNorm = second.squaredLength();
        long long thirdNorm = third.squaredLength();
        return first.x * (second.y * thirdNorm - third.y * secondNorm) -
               first.y * (second.x * thirdNorm - third.x * secondNorm) + firstNorm * cross(second, third);
    }

    static Position circumcirclePosition(Point first, Point second, Point third, Point point) {
        long long orientation = cross(first, second, third);
        if (orientation == 0) {
            long long firstSecond = squaredDistance(first, second);
            long long secondThird = squaredDistance(second, third);
            long long firstThird = squaredDistance(first, third);
            if (firstSecond >= secondThird && firstSecond >= firstThird) return diameterPosition(first, second, point);
            if (secondThird >= firstSecond && secondThird >= firstThird) return diameterPosition(second, third, point);
            return diameterPosition(first, third, point);
        }
        long long determinant = incircleDeterminant(first, second, third, point);
        if (orientation < 0) determinant = -determinant;
        if (determinant > 0) return Position::inside;
        if (determinant == 0) return Position::boundary;
        return Position::outside;
    }

    void reduceCollinearSupport() {
        if (support.size() != 3 || cross(support[0], support[1], support[2]) != 0) return;
        array<pair<long long, pair<int, int>>, 3> pairs{{
            {squaredDistance(support[0], support[1]), {0, 1}},
            {squaredDistance(support[1], support[2]), {1, 2}},
            {squaredDistance(support[0], support[2]), {0, 2}},
        }};
        auto [distance, indices] = *max_element(pairs.begin(), pairs.end());
        support = {support[indices.first], support[indices.second]};
    }

  public:
    Position position(Point point) const {
        if (support.empty()) return Position::outside;
        if (support.size() == 1) return point == support[0] ? Position::boundary : Position::outside;
        if (support.size() == 2) return diameterPosition(support[0], support[1], point);
        return circumcirclePosition(support[0], support[1], support[2], point);
    }

    static MinimumEnclosingCircleSupport build(vector<Point> points) {
        mt19937_64 random(0x6a09e667f3bcc909ULL);
        shuffle(points.begin(), points.end(), random);
        MinimumEnclosingCircleSupport circle;
        for (int i = 0; i < int(points.size()); ++i) {
            if (circle.position(points[i]) != Position::outside) continue;
            circle.support = {points[i]};
            for (int j = 0; j < i; ++j) {
                if (circle.position(points[j]) != Position::outside) continue;
                circle.support = {points[i], points[j]};
                for (int k = 0; k < j; ++k) {
                    if (circle.position(points[k]) != Position::outside) continue;
                    circle.support = {points[i], points[j], points[k]};
                    circle.reduceCollinearSupport();
                }
            }
        }
        return circle;
    }
};
#endif


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<MinimumEnclosingCircleSupport::Point> points(n);
    for (auto& [x, y] : points) cin >> x >> y;
    auto circle = MinimumEnclosingCircleSupport::build(points);
    for (auto point : points) cout << (circle.position(point) == MinimumEnclosingCircleSupport::Position::boundary);
    cout << '\n';
}
