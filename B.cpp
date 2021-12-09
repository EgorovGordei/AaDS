#include <iostream>
#include <vector>


std::vector<int> eratosthenes_sieve(int N)
{
    std::vector<int> sieve(N, 0);
    std::vector<int> primes;
    for (int i = 2; i < N; ++i)
    {
        int ld = sieve[i];
        if (sieve[i] == 0)
        {
            ld = i;
            primes.push_back(i);
        }
        for (int p = 0; p < (int)primes.size() && primes[p] <= ld && i * primes[p] < N; ++p)
        {
            sieve[i * primes[p]] = primes[p];
        }
    }
    return sieve;
}

long long sum_lowest_divisors_composits(const std::vector<int>& vec)
{
    long long ans = 0;
    for (int i = 0; i < (int)vec.size(); ++i)
    {
        if (vec[i] != 0)
        {
            ans += vec[i];
        }
    }
    return ans;
}

int main()
{
    int N = 0;

    std::cin >> N;

    std::vector<int> sieve = eratosthenes_sieve(N + 1);
    std::cout << sum_lowest_divisors_composits(sieve) << "\n";

    return 0;
}

