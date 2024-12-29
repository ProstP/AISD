/*
* Кузнецов Павел, ПС-23, Microsoft Visual Studio 2019
* 27. Имеется  И-ИЛИ   дерево,   соответствующее   некоторому
множеству  конструкций.  Требуется выдать на экран в наглядном
виде все элементы дерева (14).
*/

#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <fstream>

const char leaf_type = 'l';
const char vert_line = '|';
const char hor_line = '-';
const char space = ' ';
const char point = '.';
const char and_type = '&';
const char or_type = '|';
const int min_depth = 0;

struct node_of_tree
{
	std::string					name;
	char						type;
	int							depth;
	std::vector<node_of_tree*>	links;
};

int count_depth(std::string line)
{
	int depth = 0;

	char ch;
	ch = line[0];
	int i = 0;

	while (ch == point)
	{
		depth++;
		i++;
		ch = line[i];
	}

	return depth;
}

std::string get_name(std::string line)
{
	std::string name = "";

	for (int i = 0; i < line.length(); i++)
	{
		if ((line[i] != point) && (line[i] != space) && (line[i] != and_type) && (line[i] != or_type))
		{
			name += line[i];
		}
	}

	return name;
}

char get_type(std::string line)
{
	char ch = line[line.length() - 1];

	if (ch == and_type)
	{
		return and_type;
	}
	else if (ch == or_type)
	{
		return or_type;
	}
	else
	{
		return leaf_type;
	}
}

node_of_tree build_tree(std::ifstream& file, std::string& line, int depth, bool& all_lines)
{
	node_of_tree node;
	node.depth = depth;
	depth++;
	node.name = get_name(line);
	node.type = get_type(line);

	if (file.eof())
	{
		all_lines = true;
		return node;
	}

	std::getline(file, line);

	if (node.type == leaf_type)
	{
		return node;
	}

	while ((node.depth < count_depth(line)) && (!all_lines))
	{
		node_of_tree* new_node;
		new_node = new node_of_tree;
		*new_node = build_tree(file, line, depth, all_lines);

		node.links.push_back(new_node);
	}

	return node;
}

void print_tree(node_of_tree* node, std::string indent)
{
	std::cout << node->name << "\n";

	for (node_of_tree* link : node->links)
	{
		std::cout << indent + vert_line << "\n" << indent + vert_line + hor_line;
		if (link == node->links.back())
		{
			print_tree(link, indent + space + space);
		}
		else
		{
			print_tree(link, indent + vert_line + space);
		}
	}
}

struct or_node_struct
{
	node_of_tree* node;
	int children;
	int curr_child;
};

std::vector<or_node_struct> find_or_nodes(node_of_tree* node, std::vector<or_node_struct> or_nodes)
{
	if ((node == nullptr) || (node->type == leaf_type))
	{
		return or_nodes;
	}

	for (node_of_tree* link : node->links)
	{
		or_nodes = find_or_nodes(link, or_nodes);
	}

	if (node->type == or_type)
	{
		or_node_struct new_or_node;
		new_or_node.node = node;
		new_or_node.children = node->links.size();
		new_or_node.curr_child = 1;
		or_nodes.push_back(new_or_node);
	}

	return or_nodes;
}

std::vector<int> print_child_of_tree(node_of_tree* node, std::vector<or_node_struct> or_nodes, std::vector<int> using_or_nodes, std::string indent)
{
	std::cout << node->name << "\n";

	if (node->type == or_type)
	{
		int index;
		for (int i = 0; i < or_nodes.size(); i++)
		{
			if (node == or_nodes[i].node)
			{
				index = i;
				break;
			}
		}

		using_or_nodes.push_back(index);

		std::cout << indent + vert_line << "\n" << indent + vert_line + hor_line;
		using_or_nodes = print_child_of_tree(node->links[or_nodes[index].curr_child - 1], or_nodes, using_or_nodes, indent + space + space);
	}
	else
	{
		for (node_of_tree* link : node->links)
		{
			std::cout << indent + vert_line << "\n" << indent + vert_line + hor_line;
			if (link == node->links.back())
			{
				using_or_nodes = print_child_of_tree(link, or_nodes, using_or_nodes, indent + space + space);
			}
			else
			{
				using_or_nodes = print_child_of_tree(link, or_nodes, using_or_nodes, indent + vert_line + space);
			}
		}
	}

	return using_or_nodes;
}

void print_children_of_tree(node_of_tree* root)
{
	std::vector<or_node_struct> or_nodes;

	or_nodes = find_or_nodes(root, or_nodes);

	if (or_nodes.size() == 0)
	{
		print_tree(root, "");
		return;
	}

	bool print = true;

	while (print)
	{
		std::vector<int> using_or_nodes;
		
		std::cout << "\n";
		using_or_nodes = print_child_of_tree(root, or_nodes, using_or_nodes, "");

		int i = using_or_nodes.back();
		using_or_nodes.pop_back();
		while ((or_nodes[i].curr_child == or_nodes[i].children) && (!using_or_nodes.empty()))
		{
			or_nodes[i].curr_child = 1;
			i = using_or_nodes.back();
			using_or_nodes.pop_back();
		}

		if (or_nodes[i].curr_child == or_nodes[i].children)
		{
			print = false;
			return;
		}

		or_nodes[i].curr_child++;
	}
}

void clear_tree(node_of_tree* node)
{
	for (node_of_tree * link : node->links)
	{
		clear_tree(link);
	}

	delete node;
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	std::string file_name;
	std::cin >> file_name;

	std::ifstream in_file(file_name);
	if (!in_file.is_open())
	{
		std::cout << "Файл не найден\n";
		return 0;
	}

	std::string line;
	std::getline(in_file, line);

	node_of_tree* root;
	root = new node_of_tree;
	bool all_lines = in_file.eof();
	*root = build_tree(in_file, line, min_depth, all_lines);

	std::cout << "\n";
	print_tree(root, "");
	std::cout << "\n";
	print_children_of_tree(root);
	clear_tree(root);

	return 0;
}