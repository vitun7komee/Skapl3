#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include<stack>
using TInfo = int;

struct NODE
{
	TInfo info;
	NODE* left, * right, * prev;
	NODE(TInfo info, NODE* left = nullptr, NODE* right = nullptr, NODE* prev = nullptr) : info(info), left(left), right(right), prev(prev) {}
	~NODE()
	{
		left = nullptr;
		right = nullptr;
		prev = nullptr;
	}
	bool f = true;
};


using Tree = NODE*;

void Clear(Tree& t)
{
	if (t)
	{
		Clear(t->left);
		Clear(t->right);
		delete t;
		t = nullptr;
	}
}
void Print(Tree t, int level = 0)
{
	if (t)
	{
		Print(t->right, level + 1);
		for (int i = 0; i < level; i++)
			std::cout << "    ";
		std::cout << t->info << '\n';
		Print(t->left, level + 1);
	}

}

using Tree = NODE*;
Tree Build_Balance(std::ifstream& file, int count/*, int level = 0*/)
{
	Tree result = nullptr;
	if (count)
	{
		int count_left = count / 2;
		int count_right = count - count_left - 1;
		int x;
		file >> x;
		result = new NODE(x);
		result->left = Build_Balance(file, count_left/*,level+1*/);
		result->right = Build_Balance(file, count_right/*, level + 1*/);
	}
	return result;
}

using SInfo = Tree;

bool is_leaf(Tree p)
{
	if (p != nullptr && p->left == nullptr && p->right == nullptr)
		return true;
	return false;
}

Tree find_max(Tree root, Tree& max_leaf, Tree& max_leaf_root)
{
	if (root->right || root->left)
	{
		if (root->left && is_leaf(root->left))
		{
			if (!max_leaf || root->left->info > max_leaf->info)
			{
				max_leaf = root->left;
				max_leaf_root = root;
			}
		}
		else if (root->left)
			find_max(root->left, max_leaf, max_leaf_root);


		if (root->right && is_leaf(root->right))
		{
			if (!max_leaf || root->right->info > max_leaf->info)
			{
				max_leaf = root->right;
				max_leaf_root = root;
			}
		}
		else if (root->right)
			find_max(root->right, max_leaf, max_leaf_root);

	}
	else
	{
		if (max_leaf && max_leaf->info < root->info)
		{
			max_leaf = root;
		}
		if (!max_leaf)
		{
			max_leaf = root;
		}

	}
	return max_leaf_root;
}


void swap(Tree root, Tree& max_leaf, Tree& min_leaf, Tree& min_leaf_root, Tree& max_leaf_root)
{

	if (max_leaf_root == min_leaf_root)
	{
		Tree tmp = max_leaf_root->left;
		max_leaf_root->left = max_leaf_root->right;
		max_leaf_root->right = tmp;

	}
	else
	{

		if (min_leaf_root->left->info == min_leaf->info)
		{
			if (max_leaf_root->left->info == max_leaf->info)
			{
				Tree tmp = min_leaf_root->left;
				min_leaf_root->left = max_leaf_root->left;
				max_leaf_root->left = tmp;
			}
			else if (max_leaf_root->right->info == max_leaf->info)
			{
				Tree tmp = min_leaf_root->left;
				min_leaf_root->left = max_leaf_root->right;
				max_leaf_root->right = tmp;

			}
		}

		else if (min_leaf_root->right->info == min_leaf->info)
		{

			if (max_leaf_root->left->info == max_leaf->info)
			{
				Tree tmp = min_leaf_root->right;
				min_leaf_root->right = max_leaf_root->left;
				max_leaf_root->left = tmp;
			}
			else if (max_leaf_root->right->info == max_leaf->info)
			{
				Tree tmp = min_leaf_root->right;
				min_leaf_root->right = max_leaf_root->right;
				max_leaf_root->right = tmp;
			}
		}
	}
}


int main()
{
	setlocale(0, "");

	std::ifstream file("input.txt");
	Tree root;
	int count;

	file >> count;
	root = Build_Balance(file, count);

	Print(root);

	std::cout << std::endl << std::endl;

	bool tmp_max = false, tmp_min = false;
	Tree max_leaf = nullptr, min_leaf = nullptr, max_leaf_root = nullptr, min_leaf_root = nullptr;
	find_max(root, max_leaf, max_leaf_root);

	if (find_max(root, max_leaf, max_leaf_root)->right->info == max_leaf->info)
		max_leaf_root->right = nullptr;
	if (find_max(root, max_leaf, max_leaf_root)->left->info == max_leaf->info)
		max_leaf_root->left = nullptr;

	Print(root);
}

