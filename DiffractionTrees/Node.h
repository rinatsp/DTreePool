#pragma once
#include "Lib.h"
class Node
{
private:
	int level, index;
	Node *left;
	Node *right;
	atomic_int produser[4], consumer[4];
public:
	Node();
	Node(int depth, int h); // создание дерева
	void levelOrderPrint(Node *root); // ставим индексы дереву
	int travelse(Node *root, int mod, int thread_id);// mod 0- если пришёл запрос от потока производителя , 1 если от потребителя
	~Node();
};

