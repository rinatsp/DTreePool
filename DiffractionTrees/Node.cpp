#include "Node.h"



Node::Node()
{
}

Node::Node(int depth)
{
	if (depth > 1)
	{
		left = new Node(--depth);
		right = new Node(--depth);
	}
	else
	{
		left = nullptr;
		right = nullptr;
	}
}


Node::~Node()
{
}
