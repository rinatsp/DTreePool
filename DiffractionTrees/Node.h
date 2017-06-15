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
	Node(int depth, int h); // �������� ������
	void levelOrderPrint(Node *root); // ������ ������� ������
	int travelse(Node *root, int mod, int thread_id);// mod 0- ���� ������ ������ �� ������ ������������� , 1 ���� �� �����������
	~Node();
};

