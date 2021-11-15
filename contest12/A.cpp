#include <iostream>
#include <string>
#include <vector>
#include <functional>


void CountSort(std::function<int(int)> element, std::vector<int>& ans, int alphabet_size)
{
    std::vector<int> counter(alphabet_size, 0);
    for (int i = 0; i < (int)ans.size(); ++i)
    {
        ++counter[element(i)];
    }
    for (int i = 1; i < (int)counter.size(); ++i)
    {
        counter[i] += counter[i-1];
    }
    for (int i = (int)ans.size() - 1; i >= 0; --i)
    {
        ans[--counter[element(i)]] = i;
    }
}

std::vector<int> CreateSuffixArray(const std::string& S)
{
    std::vector<int> classeq(S.size(), 0); // classeq[i] = color_of_suffix_from_i
    std::vector<int> ans(S.size(), 0); // ans[0] = first_suffix_in_alphabetical_order
    // set up classes of equivalence
    auto value_function = [&S](int i) { return S[i]; };
    CountSort(value_function, ans, 256);
    for (int i = 1; i < (int)S.size(); ++i)
    {
        classeq[ans[i]] = classeq[ans[i - 1]];
        if (S[ans[i]] != S[ans[i - 1]])
        {
            ++classeq[ans[i]];
        }
    }
    // do loop log2(S.size()) times
    int size = 0;
    while ((1 << size) < (int)S.size())
    {
        std::vector<int> new_ans(S.size(), 0);
        std::vector<int> new_classeq(S.size(), 0);
        // magic sort by substracting 2^k
        for (int i = 0; i < (int)ans.size(); ++i)
        {
            new_ans[i] = (ans[i] - (1 << size) + S.size()) % S.size();
        }
        auto compare_function = [&classeq,&new_ans](int i) { return classeq[new_ans[i]]; };
        CountSort(compare_function, ans, classeq[ans[S.size() - 1]] + 1);
        for (int i = 0; i < (int)ans.size(); ++i)
        {
            ans[i] = new_ans[ans[i]];
        }
        // same class if both halves are the same
        for (int i = 1; i < (int)S.size(); ++i)
        {
            new_classeq[ans[i]] = new_classeq[ans[i-1]];
            int big_index_i = (ans[i] + (1 << size)) % S.size();
            int big_index_i_1 = (ans[i - 1] + (1 << size)) % S.size();
            if (classeq[ans[i]] != classeq[ans[i - 1]] || classeq[big_index_i] != classeq[big_index_i_1])
            {
                ++new_classeq[ans[i]];
            }
        }
        // next
        std::swap(classeq, new_classeq);
        ++size;
    }
    return ans;
}


int main()
{
    std::string S;
    std::cin >> S;

    std::vector<int> sufarr = CreateSuffixArray(S + "\1");

    for (int i = 1; i < (int)sufarr.size(); ++i) std::cout << sufarr[i] + 1 << " ";

    return 0;
}
