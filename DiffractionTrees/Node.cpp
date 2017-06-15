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
	queue<Node *> q; // Создаем очередь
	q.push(root); // Вставляем корень в очередь
	int i = 0;
	int l = 0;
	while (!q.empty()) // пока очередь не пуста
	{
		Node* temp = q.front(); // Берем первый элемент в очереди
		temp->index = i;

		q.pop();  // Удаляем первый элемент в очереди
		if (temp->left != nullptr) {
		   q.push(temp->left);  // Вставляем  в очередь левого потомка
		}

		if (temp->right != nullptr){
			q.push(temp->right);  // Вставляем  в очередь правого потомка
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
