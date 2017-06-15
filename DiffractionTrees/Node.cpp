#include "Node.h"



Node::Node()
{
}

Node::Node(int depth, int h)
{
		int l = depth;
		level = l;
		for (int i = 0; i < 4; i++)
		{
			if (i % 2 == 0) {
				produser[i] = 0;
				consumer[i] = 0;
			}
			else{
				produser[i] = 1;
				consumer[i] = 1;
			}
		}
		if (depth < (h-1))
		{	
			left = new Node(depth + 1 , h);
			right = new Node(depth + 1, h);
		}
		else
		{
			left = nullptr;
			right = nullptr;
		}
}



void Node::levelOrderPrint(Node *root) {
	if (root == nullptr)
	{
		return;
	}
	queue<Node *> q; // ������� �������
	q.push(root); // ��������� ������ � �������
	int i = 0;
	int l = 0;
	while (!q.empty()) // ���� ������� �� �����
	{
		Node* temp = q.front(); // ����� ������ ������� � �������
		temp->index = i;

		q.pop();  // ������� ������ ������� � �������
		if (temp->left != nullptr) {
		   q.push(temp->left);  // ���������  � ������� ������ �������
		}

		if (temp->right != nullptr){
			q.push(temp->right);  // ���������  � ������� ������� �������
		}
		i++;
	}
}

int Node::travelse(Node * root, int mod, int thread_id)
{
	Node *tmp = root;
	while (tmp->right == nullptr || tmp->left== nullptr)
	{
		if (mod == 0)
		{
			if (produser[thread_id % 4] == 0)
			{
				produser[thread_id % 4].fetch_xor(1);
				tmp = tmp->right;
			}
			else
			{
				produser[thread_id % 4].fetch_xor(1);
				tmp = tmp->left;
			}
		}
		else
		{
			if (mod == 0)
			{
				if (consumer[thread_id % 4] == 0)
				{
					consumer[thread_id % 4].fetch_xor(1);
					tmp = tmp->right;
				}
				else
				{
					consumer[thread_id % 4].fetch_xor(1);
					tmp = tmp->left;
				}
			}
		}
	}
	return tmp->index;
}



Node::~Node()
{
}
