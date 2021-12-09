#include <iostream>


const long long PRIME = 1'000'000'007;
const long long MAX_N = 300'000;


template <long long MOD>
class Field
{
private:
    long long value = 0;

public:
    Field() = default;
    explicit Field(long long value) : value(mod(value)) {}
    Field(const Field& other) : value(other.value) {}
    const Field& operator=(const Field& other)
    {
        value = other.value;
        return *this;
    }

    long long static mod(long long x)
    {
        long long remainder = x % MOD;
        if (x < 0)
        {
            remainder += MOD;
        }
        return remainder;
    }

    Field& operator++()
    {
        *this += Field(1);
        return *this;
    }

    Field operator++(int)
    {
        return Field(*this) + 1;
    }

    void operator+=(const Field& other)
    {
        value = mod(value + other.value);
    }

    void operator-=(const Field& other)
    {
        value = mod(value - other.value);
    }

    void operator*=(const Field& other)
    {
        value = mod(value * other.value);
    }

    Field static pow(const Field& field, long long n)
    {
        Field result(1);
        Field power(field);
        while (n != 0)
        {
            if(n & 1)
            {
                result *= power;
            }
            power *= power;
            n >>= 1;
        }
        return result;
    }

    explicit operator long long()
    {
        return value;
    }
};

template <long long N>
Field<N> operator+(const Field<N>& first, const Field<N>& second)
{
    Field<N> result(first);
    result += second;
    return result;
}

template <long long N>
Field<N> operator-(const Field<N>& first, const Field<N>& second)
{
    Field<N> result(first);
    result -= second;
    return result;
}

template <long long N>
Field<N> operator*(const Field<N>& first, const Field<N>& second)
{
    Field<N> result(first);
    result *= second;
    return result;
}

using Number = Field<PRIME>;


class FactorialSingleton
{
public:
    Number factorial[MAX_N];
    Number inversed_factorial[MAX_N];

private:
    static FactorialSingleton* instance;

    FactorialSingleton()
    {
        factorial[0] = Number(1);
        for (int i = 1; i < MAX_N; ++i)
        {
            factorial[i] = factorial[i - 1] * Number(i);
        }

        inversed_factorial[0] = Number(1);
        for (long long i = 0; i < MAX_N; ++i)
        {
            inversed_factorial[i] = Number::pow(factorial[i], PRIME - 2);
        }
    }
public:
    static FactorialSingleton* GetInstance()
    {
        if (!instance)
        {
            instance = new FactorialSingleton();
        }
        return instance;
    }
};

FactorialSingleton* FactorialSingleton::instance = nullptr;




Number getCnk(long long n, long long k)
{
    if(k > n || n < 0)
    {
        return Number(0);
    }

    FactorialSingleton* fact = FactorialSingleton::GetInstance();
    return fact->factorial[n] * fact->inversed_factorial[n - k] * fact->inversed_factorial[k];
}

Number getSnk(long long n, long long k)
{
    if(n * k == 0)
    {
        if (k == 0 && n == 0)
        {
            return Number(1);
        }
        else
        {
            return Number(0);
        }
    }

    Number ans;
    for (int cnt = 0; cnt < k; ++cnt)
    {
        if ((cnt & 1) == 0)
        {
            ans += getCnk(k, cnt) * Number::pow(Number(k - cnt), n);
        }
        else
        {
            ans -= getCnk(k, cnt) * Number::pow(Number(k - cnt), n);
        }
    }
    return FactorialSingleton::GetInstance()->inversed_factorial[k] * ans;
}

Number find_weights_sum(long long amount, long long coef, Number* arr)
{
    Number sum;
    for (int i = 0; i < amount; ++i)
    {
        sum += arr[i];
    }

    return sum * (getSnk(amount, coef) + Number(amount - 1) * getSnk(amount - 1, coef));
}

int main() {
    Number a[MAX_N];
    long long n = 0;
    long long k = 0;

    std::cin >> n >> k;
    for (int i = 0; i < n; ++i)
    {
        long long cur = 0;
        std::cin >> cur;
        a[i] = Number(cur);
    }

    std::cout << (long long)find_weights_sum(n, k, a) << "\n";
}

