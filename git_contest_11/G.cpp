#include <iostream>
#include <string>
#include <vector>
#include <queue>


class Trie
{
private:
    struct Node
    {
        std::vector<int32_t> indexes;
    	int32_t to[26];
        int32_t height = 0;
    	int32_t link = -1;
        bool mark = false;

	    Node(int height) : height(height), link(-1) 
    	{
	    	for (int i = 0; i < 26; ++i) to[i] = -1;
	    }
    };

	const int ALPHABET_SIZE = 2;
	const int STARTING_SYMBOL = '0';
	std::vector<Node> trie;

public:
	Trie() : trie({ Node(0) }) {}

	void add_word(const std::string& S, int ind)
	{
		int current_node = 0;
		for (const char& c : S)
		{
			if (trie[current_node].to[c - STARTING_SYMBOL] != -1)
			{
				current_node = trie[current_node].to[c - STARTING_SYMBOL];
			}
			else
			{
                trie.emplace_back(trie[current_node].height + 1);
				trie[current_node].to[c - STARTING_SYMBOL] = trie.size() - 1;
				current_node = trie.size() - 1;
			}
		}
		trie[current_node].indexes.push_back(ind);
    }

	void aho_corasick()
	{
		int current_node = 0;
		trie[current_node].link = 0;
		for (int i = 0; i < ALPHABET_SIZE; ++i)
		{
			if (trie[current_node].to[i] == -1)
				trie[current_node].to[i] = 0;
		}

		std::queue<int> processed_nodes;
		processed_nodes.push(current_node);
		while(processed_nodes.size() > 0)
		{
			current_node = processed_nodes.front();
			processed_nodes.pop();
			for (int i = 0; i < ALPHABET_SIZE; ++i)
			{
				int child = trie[current_node].to[i];
				if (child == -1 || trie[child].link != -1) continue;
				
				if (current_node == 0)
					trie[child].link = 0;
				else
					trie[child].link = trie[trie[current_node].link].to[i];
				
				for (int j = 0; j < ALPHABET_SIZE; ++j)
				{
					if (trie[child].to[j] == -1)
						trie[child].to[j] = trie[trie[child].link].to[j];
				}

				processed_nodes.push(child);
			}
		}
	}

	const Node& operator[](int i) const
	{
		return trie[i];
	}

	Node& operator[](int i)
	{
		return trie[i];
	}

	std::ostream& out(std::ostream& os) const
	{
		std::string S;
		for (int i = 0; i < int(trie.size()); ++i)
		{
			os << i << ":" << "{" << trie[i].height << ",";
			os << trie[i].link << "," << "{";
			for (int j = 0; j < ALPHABET_SIZE; ++j)
				if (trie[i].to[j] > 0)
					os << j << ":" << trie[i].to[j] << ",";
			os << "}}" << "\n";
		}
		return os;
	}

    int size() const
    {
        return trie.size();
    }

    int is_infinite()
    {
        // mark all bad nodes (that are nodes and that are reachable)
        std::queue<int> q;
        q.push(0);
        std::vector<int> used(trie.size(), 0);
        while (q.size() > 0)
        {
            if (used[q.front()] == 1)
            {
                q.pop();
                continue;
            }
            used[q.front()] = 1;
            for (int i = 0; i < ALPHABET_SIZE; ++i)
            {
                if (used[trie[q.front()].to[i]] == 0) q.push(trie[q.front()].to[i]);
            }
            if (trie[trie[q.front()].link].mark || trie[q.front()].indexes.size() > 0) trie[q.front()].mark = true;
            q.pop();
        }
        // dfs
        std::vector<std::pair<int, int>> stack;
        stack.push_back({0,0});
        used = std::vector<int>(trie.size(), 0);
        while (stack.size() > 0)
        {
            if (stack.back().second == ALPHABET_SIZE)
            {
                used[stack.back().first] = 2;
                stack.pop_back();
                continue;
            }
            used[stack.back().first] = 1;
            stack.back().second += 1;
            if (trie[trie[stack.back().first].to[stack.back().second-1]].mark) continue;
            if (used[trie[stack.back().first].to[stack.back().second-1]] == 1) return true;
            if (used[trie[stack.back().first].to[stack.back().second-1]] == 2) continue;
            stack.push_back(std::make_pair(trie[stack.back().first].to[stack.back().second-1],0));
        }

        return false;
    }
};

std::ostream& operator<<(std::ostream& os, const Trie& t)
{
	return t.out(os);
}

int main()
{
    Trie t;
    int N = 0;

   std::cin >> N;
    for (int i = 0; i < N; ++i)
	{
		std::string T;
		std::cin >> T;
        t.add_word(T, i);
	}

	t.aho_corasick();
    //std::cout << t;
    if (t.is_infinite())
        std::cout << "TAK";
    else
        std::cout << "NIE";
}

