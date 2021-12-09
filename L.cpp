#include <iostream>
#include <math.h>
#include <vector>


const double PI = std::atan2(1, 1) * 4;
const double EPS = 1e-6;

bool eq(double x, double y)
{
    return std::abs(x - y) < EPS;
}


struct Point
{
    long long x = 0;
    long long y = 0;

    Point() = default;
    Point(long long x, long long y) : x(x), y(y) {}
};

Point operator+(const Point& point1, const Point& point2)
{
    return Point(point1.x + point2.x, point1.y + point2.y);
}

bool operator<(const Point& point1, const Point& point2)
{
    return point1.y > point2.y || (point1.y == point2.y && point1.x < point2.x);
}

bool operator==(const Point& point1, const Point& point2)
{
    return (!(point1 < point2)) && (!(point2 < point1));
}

bool is_between(const Point& point, const Point& left, const Point& second)
{
    bool x_between = ((left.x - point.x) * (point.x - second.x) >= 0);
    bool y_between = ((left.y - point.y) * (point.y - second.y) >= 0);
    return x_between && y_between;
}

long long cross(Point A, Point B, Point C)
{
    return ((A.x - B.x) * (C.y - B.y) - (C.x - B.x) * (A.y - B.y));
}


double angle(Point from, Point to)
{
    double result = std::atan2(to.y - from.y, to.x - from.x);
    return result;
}

double angle(Point A, Point B, Point C)
{
    double dx1 = A.x - B.x;
    double dy1 = A.y - B.y;
    double dx2 = C.x - B.x;
    double dy2 = C.y - B.y;
    return std::atan2(dx1 * dy2 - dy1 * dx2, dx1 * dx2 + dy1 * dy2);
}

int convexity(Point A, Point B, Point C)
{
    long long dx1 = A.x - B.x;
    long long dy1 = A.y - B.y;
    long long dx2 = C.x - B.x;
    long long dy2 = C.y - B.y;
    long long cross_product = dx1 * dy2 - dy1 * dx2;
    if (cross_product == 0)
    {
        return 0;
    }
    return ((cross_product > 0) ? 1 : -1);
}


class Polygon
{
private:
    std::vector<Point> verts;
    int left_point = -1;
    int right_point = -1;

public:
    void add_vert(Point point)
    {
        verts.push_back(point);
        left_point = ((left_point != -1 && verts[left_point].x < point.x) ? left_point : ((int)verts.size() - 1));
        right_point = ((right_point != -1 && verts[right_point].x > point.x) ? right_point : ((int)verts.size() - 1));
    }

    const Point& operator[](int i) const
    {
        i %= (int)verts.size();
        if (i < 0)
        {
            i += (int)verts.size();
        }
        return verts[i];
    }

    Point& operator[](int i)
    {
        i %= (int)verts.size();
        if (i < 0)
        {
            i += (int)verts.size();
        }
        return verts[i];
    }

    int size() const
    {
        return verts.size();
    }

    void reduce()
    {
        left_point = -1;
        right_point = -1;
        std::vector<Point> new_verts;
        for (int i = -1; i < (int)verts.size() - 1; ++i)
        {
            if (!eq(angle((*this)[i], (*this)[i + 1]), angle((*this)[i + 1], (*this)[i + 2])))
            {
                new_verts.push_back(verts[i + 1]);
                left_point = ((left_point != -1 && new_verts[left_point].x < verts[i + 1].x) ? left_point : ((int)new_verts.size() - 1));
                right_point = ((right_point != -1 && new_verts[right_point].x > verts[i + 1].x) ? right_point : ((int)new_verts.size() - 1));
            }
        }
        verts = new_verts;
    }

    void rotate_canonic()
    {
        Point corner = verts[0];
        int index = 0;
        for (int i = 0; i < size(); ++i)
        {
            if (verts[i] < corner)
            {
                index = i;
                corner = verts[i];
            }
        }
        left_point = (left_point + (int)verts.size() - index) % ((int)verts.size());
        right_point = (right_point + (int)verts.size() - index) % ((int)verts.size());
        std::vector<Point> new_verts;
        for (int i = index; i < size() + index; ++i)
        {
            new_verts.push_back(verts[i % size()]);
        }
        verts = new_verts;
    }

    int get_left_index() const
    {
        return left_point;
    }

    int get_right_index() const
    {
        return right_point;
    }

    friend std::ostream& operator<<(std::ostream& os, const Polygon& pol);
};

std::istream& operator>>(std::istream& is, Polygon& pol)
{
    int N = 0;
    is >> N;
    while (N > 0)
    {
        long long x = 0;
        long long y = 0;
        is >> x >> y;
        pol.add_vert(Point(x, y));
        --N;
    }
    pol.reduce();
    pol.rotate_canonic();
    return is;
}
    
std::ostream& operator<<(std::ostream& os, const Polygon& pol)
{
    for (int i = 0; i < (int)pol.verts.size(); ++i)
    {
        os << pol.verts[i].x << " " << pol.verts[i].y << "\n";
    }
    return os;
}


Polygon summ_mink_counterclockwise(const Polygon& pol1, const Polygon& pol2)
{
    Polygon summ;
    int first = 0;
    int second = 0;
    while (first < (int)pol1.size() || second < (int)pol2.size())
    {
        summ.add_vert(pol1[first] + pol2[second]);
        double angle1 = angle(pol1[first], pol1[first + 1]);
        angle1 += (first / pol1.size()) * 2 * PI;
        double angle2 = angle(pol2[second], pol2[second + 1]);
        angle2 += (second / pol2.size()) * 2 * PI;
        if (eq(angle1, angle2))
        {
            ++first;
            ++second;
        }
        else if (angle1 < angle2)
        {
            ++first;
        }
        else
        {
            ++second;
        }
    }
    summ.reduce();
    summ.rotate_canonic();
    return summ;
}

bool is_point_on_segment(Point X, Point A, Point B) 
{
    return convexity(A, X, B) == 0 && ((A.x-X.x)*(B.x-X.x)+(A.y-X.y)*(B.y-X.y) <= 0);
}

int findcover(const Polygon& polygon, Point point, int left, int right, bool left_to_right)
{
    while (left + 1 < right)
    {
        int mid = (left + right) / 2;
        if (left_to_right)
        {
            if (polygon[mid].x <= point.x) 
            {
                left = mid;
            }
            else
            {
                right = mid;
            }
        }
        else
        {
            if (polygon[mid].x >= point.x) 
            {
                left = mid;
            }
            else
            {
                right = mid;
            }
        }
    }
    return left;
}

bool check_inside_convex(const Polygon& pol, Point point)
{
    // if point is too far away
    if (pol[pol.get_left_index()].x > point.x || pol[pol.get_right_index()].x < point.x)
    {
        return false;
    }
    if (pol[pol.get_left_index()].x == point.x)
    {
        bool res = false;
        res = res || is_point_on_segment(point, pol[pol.get_left_index()], pol[pol.get_left_index() + 1]);
        res = res || is_point_on_segment(point, pol[pol.get_left_index()], pol[pol.get_left_index() - 1]);
        return res;
    }
    if (pol[pol.get_right_index()].x == point.x)
    {
        bool res = false;
        res = res || is_point_on_segment(point, pol[pol.get_right_index()], pol[pol.get_right_index() + 1]);
        res = res || is_point_on_segment(point, pol[pol.get_right_index()], pol[pol.get_right_index() - 1]);
        return res;
    }
    // main part
    int left = pol.get_left_index();
    int right = pol.get_right_index();
    if (right < left)
    {
        right += pol.size();
    }
    int lower_border = findcover(pol, point, left, right, true);
    int upper_border = findcover(pol, point, right, left + pol.size(), false);
    return convexity(point, pol[lower_border], pol[lower_border + 1]) * convexity(point, pol[upper_border], pol[upper_border + 1]) >= 0;
}

bool check_inside_convex_2(const Polygon& pol, Point point)
{
    // if point is too far away
    if (pol[pol.get_left_index()].x > point.x || pol[pol.get_right_index()].x < point.x)
    {
        return false;
    }
    // check if point is a vertice
    for (int i = 0; i < pol.size(); ++i)
    {
        if (pol[i] == point)
        {
            return true;
        }
    }
    // check if point is on edge
    for (int i = 0; i < (int)pol.size() - 1; ++i)
    {
        if (!is_between(point, pol[i], pol[i + 1]))
        {
            continue;
        }
        // (x-x1)/(y-y1)=(x-x2)/(y-y2)
        if (eq((point.x - pol[i].x)*(point.y - pol[i+1].y), (point.x - pol[i+1].x)*(point.y - pol[i].y)))
        {
            return true;
        }
    }
    // check other
    double sum = 0;
    for (int i = 0; i < pol.size(); ++i)
    {
        sum += angle(pol[i], point, pol[i + 1]);
    }
    return std::abs(sum) > PI;
}

int main()
{
    std::cin.tie(0); std::ios_base::sync_with_stdio(false);
    Polygon A;
    Polygon B;
    Polygon C;

    std::cin >> A;
    std::cin >> B;
    std::cin >> C;

    Polygon summ = summ_mink_counterclockwise(A, B);
    summ = summ_mink_counterclockwise(summ, C);

    int q = 0;
    std::cin >> q;
    while (q > 0)
    {
        long long x = 0;
        long long y = 0;
        std::cin >> x >> y;
        bool inside = check_inside_convex(summ, Point(3 * x, 3 * y));
        std::cout << (inside ? "YES" : "NO") << "\n";
        --q;
    }
}
