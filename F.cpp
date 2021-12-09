#include <algorithm>
#include <iostream>
#include <math.h>
#include <vector>


int sign(long long x)
{
    if (x == 0)
    {
        return 0;
    }
    if (x > 0)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}



struct Point
{
    long long x = 0;
    long long y = 0;

    Point() : x(0), y(0) {}
    Point(long long x, long long y) : x(x), y(y) {}
};

std::ostream& operator<<(std::ostream& os, const Point& point)
{
    return os << point.x << " " << point.y;
}

bool operator==(const Point& a, const Point& b)
{
    return a.x == b.x && a.y == b.y;
}



struct Vector
{
    long long x = 0;
    long long y = 0;

    Vector(long long x, long long y) : x(x), y(y) {}

    Vector(const Point& a, const Point& b) : x(b.x - a.x), y(b.y - a.y) {}

    double abs() const
    {
        return std::sqrt(x * x + y * y);
    }
};

long long scalar(const Vector& vec1, const Vector& vec2)
{
    return vec1.x * vec2.x + vec1.y * vec2.y;
}

long long cross(const Vector& vec1, const Vector& vec2)
{
    return vec1.x * vec2.y - vec1.y * vec2.x;
}

Vector operator+(const Vector& vec1, const Vector& vec2)
{
    return Vector(vec1.x + vec2.x, vec1.y + vec2.y);
}

std::ostream& operator<<(std::ostream& os, const Vector& vec)
{
    return os << vec.x << " " << vec.y;
}



int convexity(const Point& left, const Point& middle, const Point& right)
{
    return sign(cross(Vector(middle, left), Vector(middle, right)));
}

std::vector<Point> get_outer(std::vector<Point> points)
{
    auto cmp = [](const Point& a, const Point& b){ return (a.x < b.x) || (a.x == b.x && a.y < b.y); };
    std::sort(points.begin(), points.end(), cmp);

    Point leftdown(points[0]);
    Point rightup(points[points.size() - 1]);
    std::vector<Point> top(1, leftdown);
    std::vector<Point> bot(1, leftdown);

    for (int i = 1; i < (int)points.size(); ++i)
    {
        // update bottom part
        if (i == (int)points.size() - 1 || convexity(leftdown, points[i], rightup) < 0)
        {
            while (bot.size() > 1 && convexity(bot[bot.size() - 2], bot[bot.size() - 1], points[i]) >= 0)
            {
                bot.pop_back();
            }
            bot.push_back(points[i]);
        }
        // update top part
        if (i == (int)points.size() - 1 || convexity(leftdown, points[i], rightup) > 0)
        {
            while (top.size() > 1 && convexity(top[top.size() - 2], top[top.size() - 1], points[i]) <= 0)
            {
                top.pop_back();
            }
            top.push_back(points[i]);
        }
    }

    std::vector<Point> ans;
    ans.insert(ans.end(), bot.begin(), bot.end());
    std::reverse(top.begin(), top.end());
    ans.insert(ans.end(), top.begin() + 1, top.end() - 1);

    std::vector<Point> ans_cut;
    ans_cut.push_back(ans[0]);
    ans_cut.push_back(ans[1]);
    for (int i = 2; i < (int)ans.size(); ++i)
    {
        if (convexity(ans_cut[ans_cut.size() - 2], ans_cut[ans_cut.size() - 1], ans[i]) == 0)
        {
            ans_cut[ans_cut.size() - 1] = ans[i];
        }
        else
        {
             ans_cut.push_back(ans[i]);
        }
    }

    if (convexity(ans_cut[ans_cut.size() - 1], ans_cut[0], ans_cut[1]) == 0)
    {
        ans_cut.erase(ans_cut.begin());
    }
    if (convexity(ans_cut[ans_cut.size() - 2], ans_cut[ans_cut.size() - 1], ans_cut[0]) == 0)
    {
        ans_cut.pop_back();
    }

    return ans_cut;
}

int main()
{
    int N = 0;
    long long x = 0;
    long long y = 0;

    std::cin >> N;
    std::vector<Point> points(N);
    for (int i = 0; i < N; ++i)
    {
        std::cin >> x >> y;
        points[i] = Point(x, y);
    }

    std::vector<Point> outer = get_outer(points);
    std::cout << outer.size() << "\n";
    for (int i = 0; i < (int)outer.size(); ++i)
    {
        std::cout << outer[i] << "\n";
    }
}

