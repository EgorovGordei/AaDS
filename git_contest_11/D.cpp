#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>


struct Node
{
	int term = 0;
	int par = 0;
	int symbol = 0;
	std::vector<int> edge;

	/*
	int link = -1;
	std::vector<int> to;
	*/

	Node(int ALPHABET_SIZE, int symbol, int parent) : term(0), par(parent), symbol(symbol), edge(ALPHABET_SIZE, -1)/*, link(-1), to(ALPHABET_SIZE, -1)*/ {}
};


class Trie
{
private:
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
			if (trie[current_node].edge[c - STARTING_SYMBOL] != -1)
			{
				current_node = trie[current_node].edge[c - STARTING_SYMBOL];
			}
			else
			{
				return 0;
			}
		}
		if (trie[current_node].term > 0) return current_node;
		return 0;
	}

	bool remove_word(const std::string& S)
	{
		int current_node = find_word(S);
		if (current_node == 0) return false;

		trie[current_node].term -= 1;
		return true;
	}

	/*
	void aho_corasick()
	{
		int current_node = 0;
		trie[current_node].link = 0;
		for (int i = 0; i < int(trie[current_node].to.size()); ++i)
		{
			if (trie[current_node].edge[i] != -1)
				trie[current_node].to[i] = trie[current_node].edge[i];
			else
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
				int child = trie[current_node].edge[i];
				if (child == -1) continue;
				
				if (current_node == 0)
					trie[child].link = 0;
				else
					trie[child].link = trie[trie[current_node].link].to[i];
				
				for (int j = 0; j < ALPHABET_SIZE; ++j)
				{
					if (trie[child].edge[j] != -1)
					{
						trie[child].to[j] = trie[child].edge[j];
					}
					else
					{
						trie[child].to[j] = trie[trie[child].link].to[j];
					}
				}

				processed_nodes.push(child);
			}
		}
	}
	*/

	std::string get_word_from_index(int i) const
	{
		std::string s = "";
		while (trie[i].par != -1)
		{
			s += trie[i].symbol;
			i = trie[i].par;
		}
		std::reverse(s.begin(), s.end());
		return s;
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
			/*
			os << "}" << "," << trie[i].link << "," << "{";
			for (int j = 0; j < int(trie[i].to.size()); ++j)
				if (trie[i].to[j] > 0)
					os << j << ":" << trie[i].to[j] << ",";
			*/
			os << "}}" << "\n";
		}
		return os;
	}
};

std::ostream& operator<<(std::ostream& os, const Trie& t)
{
	return t.out(os);
}

int main()
{
	std::string S;
	std::cin >> S;
	//S = "aba..abba..aa.aaba.b.....a.a..a..bba.ab.aba.bbaa";

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

	int j = 0;
	for (auto it = t.begin(); it != t.end(); ++it)
	{
		for (int i = 0; i < dots[j]; ++i) std::cout << ".";
		std::cout << t.get_word_from_index(*it);
		j += 1;
	}
	if (int(dots.size()) > j) for (int i = 0; i < dots[j]; ++i) std::cout << ".";
}

