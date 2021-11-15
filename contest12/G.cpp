#include <iostream>
#include <vector>
#include <queue>


long long INFINITY = 1'000'000'000;

struct Request
{
    int start_index = 0;
    std::vector<char> vector_string;

    Request(long long start_index, std::vector<char> vector_string) : start_index(start_index), vector_string(vector_string) {}
};

class SuffixTree
{
public:
    class Node
    {
    private:
        std::vector<int> to;
        int parent = -1;
        int substr_l = -1;
        int substr_r = -1;
        int link = -1;
        int height = 0;
        int terminals_below = 0;

    public:
        Node(long long alphabet_size, long long parent = -1,
             long long substr_l = 0, long long substr_r = 0, long long link = 0, long long height = 0) : to(alphabet_size, 0), parent(parent), substr_l(substr_l), substr_r(substr_r),
                                                                                  link(link), height(height) {}

        long long get_alphabet_size() const { return to.size(); }

        long long get_substr_l() const { return substr_l; }
        long long get_substr_r() const { return substr_r; }
        long long get_size() const { return substr_r - substr_l; }
        long long get_parent() const { return parent; }
        long long get_link() const { return link; }
        long long get_height() const { return height; }
        long long get_terminals_below() const { return terminals_below; }

        Node& set_substr(long long new_l, long long new_r) { substr_l = new_l; substr_r = new_r; return *this; }
        Node& set_parent(long long new_parent) { parent = new_parent; return *this; }
        Node& set_link(long long new_link) { link = new_link; return *this; }
        Node& set_height(long long new_height) { height = new_height; return *this; }
        Node& set_terminals_below(long long new_terminals_below) { terminals_below = new_terminals_below; return *this; }

        long long operator[](long long i) const { return to[i]; }
        int& operator[](long long i) { return to[i]; }

        void print(std::ostream& os) const
        {
            os << "parent:" << parent << " substr:[" << substr_l << "-" << substr_r << "] link:" << link;
            os << " height:" << height << " terminals_below:" << terminals_below;
            os << " [";
            for (long long i = 0; i < (long long)to.size(); ++i)
            {
                if (to[i] != 0)
                {
                    os << "(" << i << ":" << to[i] << ")";
                }
            }
            os << "]";
        }
    };

private:
    std::vector<Node> nodes;
    long long min_s = 0;
    long long max_s = 0;
    long long cur = 0;
    long long cur_bias = 0;

public:
    SuffixTree(std::vector<long long> arr, const std::vector<Request>& requests, std::vector<bool>& results, long long min_s = '0', long long max_s = '9' + 1) : min_s(min_s), max_s(max_s)
    {
        INFINITY = arr.size();
        for (long long i = 0; i < (long long)arr.size(); ++i)
        {
            arr[i] -= min_s;
        }
        nodes.push_back(Node(max_s-min_s));
        nodes[0].set_substr(0, INFINITY);
        int request_index = 0;
        for (long long i = 0; i < (long long)arr.size(); ++i)
        {
            while (request_index < (int)requests.size() && requests[request_index].start_index == i)
            {
                results[request_index] = check_word(arr, requests[request_index].vector_string, i);
                ++request_index;
            }
            Extend(arr, i);
        }
        while (request_index < (int)requests.size())
        {
            results[request_index] = check_word(arr, requests[request_index].vector_string, arr.size());
            ++request_index;
        } 
    }

    bool check_word(const std::vector<long long>& arr, const std::vector<char>& word, long long already_constructed)
    {
        long long cur_node = 0;
        long long cur_bias = 0;
        long long cur_char = 0;
        while (cur_char != (int)word.size())
        {
            if (cur_bias == 0)
            {
                long long next_node = nodes[cur_node][word[cur_char] - min_s];
                if (next_node == 0) return false;
                ++cur_char;
                cur_node = next_node;
                cur_bias = 1;
            }
            else
            {
                long long next_char_index = nodes[cur_node].get_substr_l() + cur_bias;
                if (next_char_index >= already_constructed) return false;
                if (next_char_index >= nodes[cur_node].get_substr_r())
                {
                    cur_bias = 0;
                    continue;
                }
                if (word[cur_char] - min_s != arr[next_char_index]) return false;
                ++cur_bias;
                ++cur_char;
            }
        }
        return true;
    }

    void Extend(const std::vector<long long>& source, long long pos)
    {
        long long symbol = source[pos];
        ++cur_bias;
        long long last = 0;
        while (cur_bias > 0)
        {
            while (cur_bias > nodes[nodes[cur][source[pos - cur_bias + 1]]].get_size())
            {
                cur = nodes[cur][source[pos - cur_bias + 1]];
                cur_bias -= nodes[cur].get_size();
            }
            long long edge = source[pos - cur_bias + 1];
            int& v = nodes[cur][edge];
            long long t = source[nodes[v].get_substr_l() + cur_bias - 1];
            if(v == 0)
            {
                nodes.push_back(Node(nodes[0].get_alphabet_size(), cur, pos - cur_bias + 1, INFINITY));
                v = (long long)nodes.size() - 1;
                nodes[last].set_link(cur);
                last = 0;
            }
            else if(t == symbol)
            {
                nodes[last].set_link(cur);
                break;
            }
            else
            {
                nodes.push_back(Node(nodes[0].get_alphabet_size(), cur, nodes[v].get_substr_l(), nodes[v].get_substr_l() + cur_bias - 1));
                long long u = (long long)nodes.size() - 1;
                nodes.push_back(Node(nodes[0].get_alphabet_size(), u, pos, INFINITY));
                nodes[u][symbol] = (long long)nodes.size() - 1;
                nodes[u][t] = v;
                nodes[v].set_parent(u);
                nodes[v].set_substr(nodes[v].get_substr_l() + (cur_bias - 1), nodes[v].get_substr_r());
                v = u;
                nodes[last].set_link(u);
                last = u;
            }
            if(cur == 0)
                --cur_bias;
            else
                cur = nodes[cur].get_link();
        }
    }

    void print(std::ostream& os) const;
};

std::ostream& operator<<(std::ostream& os, const SuffixTree& st)
{
    st.print(os);
    return os;
}

std::ostream& operator<<(std::ostream& os, const SuffixTree::Node& node)
{
    node.print(os);
    return os;
}

void SuffixTree::print(std::ostream& os) const
{
    os << "Tree:\n";
    os << "min_s:" << min_s << " max_s:" << max_s << " cur:" << cur << "\nnodes\n{\n";
    for (long long i = 0; i < (long long)nodes.size(); ++i)
    {
        os << i << "<" << nodes[i] << ">\n";
    }
    os << "}\n";
}

int main()
{
    std::cin.tie(0); std::cout.tie(0);
    std::ios_base::sync_with_stdio(false);
    char command = 0;
    std::string S;
    std::vector<long long> vector_string;
    std::vector<Request> requests;
    while (std::cin >> command)
    {
        std::cin >> S;
        if (command == '?')
        {
            requests.push_back(Request(vector_string.size(), {}));
        }
        for (char c : S)
        {
            if (c >= 'A' && c <= 'Z')
            {
                c = c - 'A' + 'a';
            }
            if (command == 'A')
            {
                vector_string.push_back(c);
            }
            else
            {
                requests[requests.size() - 1].vector_string.push_back(c);
            }
        }
    }

    std::vector<bool> result(requests.size(), false);
    SuffixTree st(vector_string, requests, result, 'a', 'z' + 1);

    for (int i = 0; i < (int)result.size(); ++i)
    {
        std::cout << (result[i] ? "YES" : "NO") << "\n";
    }

    return 0;
}
