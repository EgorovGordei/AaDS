#include <iostream>
#include <string>
#include <vector>


std::vector<int> CreateZFunction(const std::string& text)
{
    std::vector<int> z_function = std::vector<int>(text.size(), 0);

    int lu = 0;
    int ru = 1;
    for (int i = 1; i < text.size(); ++i)
    {
        if (i >= ru)
        {
            while (i + z_function[i] < text.size() && text[i + z_function[i]] == text[z_function[i]])
            {
                ++z_function[i];
            }
            lu = i;
            ru = i + z_function[i];
        }
        else
        {
            z_function[i] = std::min(z_function[i - lu], ru - i);
            while (i + z_function[i] < text.size() && S[i + z_function[i]] == text[z_function[i]])
            {
                ++z_function[i];
            }
            if (i + z_function[i] > ru)
            {
                lu = i;
                ru = i + z_function[i];
            }
        }
    }

    return z_function;  
}

int main()
{
    std::string S;
    std::cin >> S;

    std::vector<int> z_function = CreateZFunction(S);

    for (int i = 0; i < int(S.size()); ++i)
    {
        std::cout << z_function[i] << " ";
    }

    return 0;
}

