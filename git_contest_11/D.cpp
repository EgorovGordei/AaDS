#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>



class Trie
{
private:
    struct Node
    {
	    int term = 0;
    	int par = 0;
	    int symbol = 0;
    	std::vector<int> edge;

	    Node(int ALPHABET_SIZE, int symbol, int parent) : par(parent), symbol(symbol), edge(ALPHABET_SIZE, -1) {}
    };

    const int ALPHABET_SIZE = 26;
	const int STARTING_SYMBOL = 'a';
	std::vector<Node> trie;

public:
   	struct iterator
	{
		int index = -1;
		int n = 1;
		Trie* tr;

		iterator(Trie* tr, int node) : index(node), tr(tr) {}
		
		iterator& operator++()
		{
			if (n < tr->trie[index].term)
			{
				++n;
				return *this;
			}
			n = 1;
			int current_node = index;
			bool first = true;
			int symbol = 0;
			while (true)
			{
				if (current_node == -1)
				{
					current_node = tr->trie.size();
					break;
				}
				if (!first && tr->trie[current_node].term > 0)
				{
					break;
				}
				first = false;

				int next_node = -1;
				for (int i = symbol; i < int(tr->trie[current_node].edge.size()); ++i)
				{
					if (tr->trie[current_node].edge[i] != -1)
					{
						next_node = tr->trie[current_node].edge[i];
						break;
					}
				}
				symbol = 0;
				if (next_node != -1)
				{
					current_node = next_node;
					continue;
				}
	
				symbol = tr->trie[current_node].symbol - tr->STARTING_SYMBOL + 1;
				current_node = tr->trie[current_node].par;
				first = true;
			}
			index = current_node;
			return *this;
		}

		iterator operator++(int)
		{
			iterator it(*this);
			++it;
			return it;
		}

		int operator*()
		{
			return index;
		}

		bool operator==(const iterator& it2) const
		{
			return index == it2.index && n == it2.n;
		}

		bool operator!=(const iterator& it2) const
		{
			return !(*this == it2);
		}

	};

	Trie() : trie({ Node(ALPHABET_SIZE, -1, -1) }) {}

	void add_word(const std::string& S)
	{
		int current_node = 0;
		for (const char& c : S)
		{
			if (trie[current_node].edge[c - STARTING_SYMBOL] != -1)
			{
				current_node = trie[current_node].edge[c - STARTING_SYMBOL];
			}
			else
			{
				trie.emplace_back(ALPHABET_SIZE, c, current_node);
				trie[current_node].edge[c - STARTING_SYMBOL] = trie.size() - 1;
				current_node = trie.size() - 1;
			}
		}
		trie[current_node].term += 1;
	}

	int find_word(const std::string& S) const
	{
		int current_node = 0;
		for (const char& c : S)
		{
			if (trie[current_node].edge[c - STARTING_SYMBOL] == -1) return 0;
			current_node = trie[current_node].edge[c - STARTING_SYMBOL];
		}
		return (trie[current_node].term > 0) ? current_node : 0;
	}

	bool remove_word(const std::string& S)
	{
		int current_node = find_word(S);
		if (current_node == 0) return false;

		trie[current_node].term -= 1;
		return true;
	}

	std::string get_word_from_index(int i) const
	{
		std::string word = "";
		while (trie[i].par != -1)
		{
			word += trie[i].symbol;
			i = trie[i].par;
		}
		std::reverse(word.begin(), word.end());
		return word;
	}

	iterator begin()
	{
		return iterator(this, 0)++;
	}

	iterator end()
	{
		return iterator(this, trie.size());
	}

	std::ostream& out(std::ostream& os) const
	{
		std::string S;
		for (int i = 0; i < int(trie.size()); ++i)
		{
			os << i << ":" << "{" << trie[i].term << "," << trie[i].par << "," << "{";
			for (int j = 0; j < int(trie[i].edge.size()); ++j)
				if (trie[i].edge[j] != -1)
					os << j << ":" << trie[i].edge[j] << ",";
			os << "}}" << "\n";
		}
		return os;
	}
};

std::ostream& operator<<(std::ostream& os, const Trie& t)
{
	return t.out(os);
}

std::string solve(const std::string& S)
{
    std::vector<int> dots(1, 0);
	std::string word;
	Trie t;
	for (auto c : S)
	{
		if (c == '.')
		{
			if (word != "")
			{
				t.add_word(word);
				word = "";
				dots.push_back(1);
			}
			else
			{
				dots[dots.size() - 1] += 1;
			}
		}
		else
		{
			word += c;
		}
	}
	if (word != "") t.add_word(word);

    std::string ans;
	int j = 0;
	for (auto it = t.begin(); it != t.end(); ++it)
	{
		for (int i = 0; i < dots[j]; ++i) ans += ".";
		ans += t.get_word_from_index(*it);
		j += 1;
	}
	if (int(dots.size()) > j) for (int i = 0; i < dots[j]; ++i) ans += ".";

    return ans;
}

int main()
{
	std::string S;
	std::cin >> S;

	std::cout << solve(S);
}

