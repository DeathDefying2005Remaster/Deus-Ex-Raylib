#pragma once
#include <raylib-cpp.hpp>
using namespace std;

template <typename T> class RedBlackTree 
{
	public:
	
	enum Colour { red, black };

	struct Node
	{
		T value;
		Colour colour;
		Node* parent;
		Node* left;
		Node* right;

		Node(T _value, Colour _colour)
		{
			value = _value;
			colour = _colour;
			parent = nullptr;
			left = nullptr;
			right = nullptr;
		}
	};

	Node* root;

	RedBlackTree()
	{
		root = nullptr;
	}

	void Insert(T value)
	{
		if (root == nullptr)
		{
			root = new Node(value, black);
		}
		else
		{
			Node* current = root;
			while (current != nullptr)
			{
				if (value == current->value) 
				{ 
					return; 
				}
				else if (value < current->value)
				{
					current->left == nullptr ? SetLeaf(current->left) : current = current->left;
				}
				else if (value > current->value)
				{
					current->right == nullptr ? SetLeaf(current->right) : current = current->right;
				}
			}
		}
	}
	void SetLeaf(Node* leaf)
	{

	}

	void Draw()
	{
		Node* current = root;
		while (current->left != nullptr && current->right != nullptr)
		{
			rand() < 0.5 ? current = current->left : current = current->right;
		}
	}
};