#pragma once
class Node
{
	Node *left;
	Node *right;
	int depth;
public:
	Node();
	Node(int depth);
	~Node();
};

