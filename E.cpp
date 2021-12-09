#include <iostream>
#include <math.h>
#include <vector>


const double PI = 3.141592653589793;

const double EPS = 1e-10;

bool eq(double x, double y, double eps = EPS)
{
    return std::abs(x - y) < eps;
}

int sign(double x, double eps = EPS)
{
    if (eq(x, 0, eps)) return 0;
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
    double x = 0;
    double y = 0;

    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}
};

std::ostream& operator<<(std::ostream& os, const Point& point)
{
    return os << point.x << " " << point.y;
}

bool operator==(const Point& first, const Point& second)
{
    return eq(first.x, second.x) && eq(first.y, second.y);
}

bool is_between(const Point& point, const Point& first, const Point& second)
{
    bool x_between = (sign(first.x - point.x) * sign(point.x - second.x) >= 0);
    bool y_between = (sign(first.y - point.y) * sign(point.y - second.y) >= 0);
    return x_between && y_between;
}




struct Vector
{
    double x = 0;
    double y = 0;

    Vector(double x, double y) : x(x), y(y) {}

    Vector(const Point& a, const Point& b) : x(b.x - a.x), y(b.y - a.y) {}

    double abs() const
    {
        return std::sqrt(x * x + y * y);
    }
};

double scalar(const Vector& vec1, const Vector& vec2)
{
    return vec1.x * vec2.x + vec1.y * vec2.y;
}

double cross(const Vector& vec1, const Vector& vec2)
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



struct Line
{
    double a = 0;
    double b = 0;
    double c = 0;

    Line(double a, double b, double c) : a(a), b(b), c(c) {}
    Line(const Point& p, const Point& q) : a(q.y - p.y), b(p.x - q.x), c(p.y * q.x - p.x * q.y) {}

    Vector direction() const
    {
        return Vector(-b, a);
    }
};

Point intersect(const Line& line1, const Line& line2)
{
    double angle = cross(line1.direction(), line2.direction());
    return Point((line1.c * line2.b - line1.b * line2.c) / -angle,
                 (line1.c * line2.a - line1.a * line2.c) / angle);
}

double distance(const Line& line, const Point& point)
{
    return std::abs(line.a * point.x + line.b * point.y + line.c) / std::sqrt(line.a * line.a + line.b * line.b);
}

double distance(const Line& line1, const Line& line2)
{
    Point point;
    if (std::abs(line1.a) > std::abs(line1.b))
    {
        point.y = 1;
        point.x = (-line1.c - line1.b) / line1.a;
    }
    else
    {   point.x = 1;
        point.y = (-line1.c - line1.a) / line1.b;
    }
    return distance(line2, point);
}

bool is_parallel(const Line& line1, const Line& line2)
{
    return eq(cross(line1.direction(), line2.direction()), 0);
}

std::ostream& operator<<(std::ostream& os, const Line& line)
{
    return os << line.a << " " << line.b << " "<< line.c;
}



bool does_intersect(const Point& a1, const Point& a2, const Point& b1, const Point& b2)
{
    Line line1(a1, a2);
    Line line2(b2, b1);
    if (is_parallel(line1, line2))
    {
        return is_between(a1, b1, b2) || is_between(a2, b1, b2) ||
               is_between(b1, a1, a2) || is_between(b2, a1, a2);
    }
    else
    {
        Point intersection = intersect(line1, line2);
        return is_between(intersection, a1, a2) && is_between(intersection, b1, b2);
    }
}

bool is_convex(std::vector<Point> polygon)
{
    polygon.push_back(polygon[0]);
    polygon.push_back(polygon[1]);
    int target = 0;
    int i = 0;
    while (i < (int)polygon.size() - 2)
    {
        target = sign(cross({polygon[i], polygon[i + 1]}, {polygon[i + 1], polygon[i + 2]}));
        if (target != 0)
        {
            break;
        }
        ++i;
    }
    if (i == (int)polygon.size() - 2) return false;
    for (int i = 0; i < (int)polygon.size() - 2; ++i)
    {
        int cur_sign = sign(cross({polygon[i], polygon[i + 1]}, {polygon[i + 1], polygon[i + 2]}));
        if (target != cur_sign && cur_sign != 0)
        {
            return false;
        }
    }
    return true;
}

bool is_inside(const Point& point, std::vector<Point> polygon)
{
    // check if point is a vertice
    for (int i = 0; i < (int)polygon.size(); ++i)
    {
        if (polygon[i] == point)
        {
            return true;
        }
    }
    polygon.push_back(polygon[0]);
    // check if point is on edge
    for (int i = 0; i < (int)polygon.size() - 1; ++i)
    {
        if (!is_between(point, polygon[i], polygon[i + 1]))
        {
            continue;
        }
        // (x-x1)/(y-y1)=(x-x2)/(y-y2)
        double val1 = (point.x - polygon[i].x) * (point.y - polygon[i + 1].y);
        double val2 = (point.x - polygon[i + 1].x) * (point.y - polygon[i].y);
        if (eq(val1, val2))
        {
            return true;
        }
    }
    // check other
    double angle = 0;
    for (int i = 0; i < (int)polygon.size() - 1; ++i)
    {
        Vector first(point, polygon[i]);
        Vector second(point, polygon[i + 1]);
        double da = std::atan2(cross(first, second) / first.abs() / second.abs(),
                               scalar(first, second) / first.abs() / second.abs());
        angle += da;
    }
    return std::abs(angle) > PI;
}



int main()
{
    int x = 0;
    int y = 0;
    int N = 0;

    std::cin >> N;
    std::cin >> x >> y;
    Point point(x, y);
    std::vector<Point> polygon(N);
    for (int i = 0; i < N; ++i)
    {
        std::cin >> x >> y;
        polygon[i] = Point(x, y);
    }
    if (is_inside(point, polygon))
    {
        std::cout << "YES\n";
    }
    else
    {
        std::cout << "NO\n";
    }
}

