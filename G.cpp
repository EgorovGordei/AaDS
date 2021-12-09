#include <cmath>
#include <complex>
#include <iostream>
#include <vector>


const double PI = 3.14159265358979323846;
int kmax = 17;

void fft (std::vector<std::complex<double>>& a, bool reversed) {
    if (a.size() == 1)
    {
        return;
    }

    int half_size = (int)(a.size() >> 1);

    // recursion
    std::vector<std::complex<double>> first_half(half_size);
    std::vector<std::complex<double>> second_half(half_size);
    for (int i = 0, j = 0; i < (int)a.size(); i += 2, ++j)
    {
        first_half[j] = a[i];
        second_half[j] = a[i + 1];
    }
    fft(first_half, reversed);
    fft(second_half, reversed);
 
    // combine
    double angle = PI / (half_size);
    if (reversed) angle = -angle;
    std::complex<double> w(cos(angle), sin(angle));
    std::complex<double> power(1, 0);
    for (int i = 0; i < half_size; ++i)
    {
        a[i]             = first_half[i] + power * second_half[i];
        a[i + half_size] = first_half[i] - power * second_half[i];
        power *= w;
    }
    if (reversed)
    {
        for (int i = 0; i < half_size; ++i)
        {
            a[i] /= 2;
            a[i + half_size] /= 2;
        }
    }
}

const double EPS = 0.1;

std::vector<int> fast_polynom_multiply(const std::vector<int>& pola, const std::vector<int> polb)
{
    // reverse, make complex and resize to be (1 << kmax)
    std::vector<std::complex<double>> a_complex(1 << kmax);
    for (int i = 0; i < (int)pola.size(); ++i)
    {
        a_complex[i] = std::complex<double>(pola[(int)pola.size() - i - 1], 0.0);
    }
    std::vector<std::complex<double>> b_complex(1 << kmax);
    for (int i = 0; i < (int)polb.size(); ++i)
    {
        b_complex[i] = std::complex<double>(polb[(int)polb.size() - i - 1], 0.0);
    }

    // A * B = fft^-1(fft(A) * fft(B))
    fft(a_complex, false);
    fft(b_complex, false);
    std::vector<std::complex<double>> result(1 << kmax);
    for (int i = 0; i < (1 << kmax); ++i)
    {
        result[i] = a_complex[i] * b_complex[i];
    }
    fft(result, true);

    // result to int without leading zeros
    int zeros = 0;
    for (int i = 0; i < (int)result.size(); ++i)
    {
        if (norm(result[i]) >= EPS)
        {
            zeros = i;
        }
    }
    std::vector<int> int_result(zeros + 1);
    for (int i = 0; i < zeros + 1; ++i)
    {
        int_result[zeros - i] = std::lround(real(result[i]));
    }

    return int_result;
}

int main()
{
    int n = 0;
    int m = 0;
    std::vector<int> a;
    std::vector<int> b;

    std::cin >> n;
    a.resize(n + 1);
    for (int i = 0; i < n + 1; ++i)
    {
        std::cin >> a[i];
    }
    std::cin >> m;
    b.resize(m + 1);
    for (int i = 0; i < m + 1; ++i)
    {
        std::cin >> b[i];
    }

    std::vector<int> ans = fast_polynom_multiply(a, b);

    std::cout << ans.size() - 1 << " ";
    for (int i = 0; i < (int)ans.size(); ++i)
    {
        std::cout << ans[i] << " ";
    }

    return 0;
}

