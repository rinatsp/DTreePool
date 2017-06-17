#include "Node.h"

//thread_local int produser[4], consumer[4];

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
			left =  nullptr;
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
		if (temp->left == nullptr || temp->right == nullptr)
		{
			temp->index = l;
			l++;
		}
		else
		temp->index = i;
		//cout << temp->index <<" " <<temp->level << endl;
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
	while (tmp->right != nullptr || tmp->left != nullptr)
	{
		if (mod == 0)
		{
			if (tmp->produser[thread_id % 4] == 0)
			{
				tmp->produser[thread_id % 4].fetch_xor(1);
				//produser[thread_id % 4] = 1;
				tmp = tmp->right;
				//cout << produser[thread_id % 4] << endl;
			}
			else
			{
				tmp->produser[thread_id % 4].fetch_xor(1);
				//produser[thread_id % 4] = 0;
				tmp = tmp->left;
				//cout << produser[thread_id % 4] << endl;
			}
		}
		else
		{
			if (mod == 1)
			{
				if (tmp->consumer[thread_id % 4] == 0)
				{
					tmp->consumer[thread_id % 4].fetch_xor(1);
					//consumer[thread_id % 4] = 1;
					tmp = tmp->right;
				}
				else
				{
					tmp->consumer[thread_id % 4].fetch_xor(1);
					//consumer[thread_id % 4] = 0;
					tmp = tmp->left;
				}
			}
		}
	}
	return tmp->index;
}

#define COUNT 10
void Node::print2DUtil(Node *root, int space)
{
	// Base case
	if (root == nullptr)
		return;

	// Increase distance between levels
	space += COUNT;

	// Process right child first
	print2DUtil(root->right, space);

	// Print current node after space
	// count
	printf("\n");
	for (int i = COUNT; i < space; i++)
		printf(" ");
	printf("%d\n", root->index);

	// Process left child
	print2DUtil(root->left, space);
}

Node::~Node()
{
}
