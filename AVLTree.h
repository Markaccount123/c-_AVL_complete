#pragma once

#include<iostream>
#include<assert.h>
using namespace std;

template<class K,class V>
struct AVLTreeNode
{
	//ʹ��������,����Ҳ�Ƚϱ����ҵ�parent�ڵ�
	AVLTreeNode<K,V>* _left;
	AVLTreeNode<K, V>* _right;
	AVLTreeNode<K, V>* _parent;
	pair<K, V> _kv;

	int _bf;//ƽ������  ���Ҹ߶Ȳ� ��������������Ƿ�ƽ��

	AVLTreeNode(const pair<K, V>& kv)
		:_left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		, _kv(kv)
		, _bf(0) //�մ��������Ľڵ������������ǿգ�����ƽ�����Ӹ�0
	{}
};

template<class K, class V>
class AVLTree
{
	typedef AVLTreeNode<K,V> Node;
public:
	//����AVL��Ҳ�޷Ǿ�����ɾ�飬���ǲ��ܸ�
	//1.����Ҫ����ƽ�����ӣ���Ϊ�����һ���µĽڵ㣬��ô�������ȵ�ƽ������Ҳ���ܵ�Ӱ�죩
	//2.������¹�����|bf| > 1,��˵����ʱ�Ѿ�����ƽ�⣬��Ҫ��ת
	AVLTree() = default;
	AVLTree(const AVLTree<K, V>& t);
	AVLTree<K, V>& operator=(AVLTree<K, V> t);
	//~AVLTree();

	pair<Node*, bool> Insert(const pair<K, V>& kv)
	{
		if (_root == nullptr)
		{
			_root = new Node(kv);
			return make_pair(_root, true);
		}
		Node* parent = nullptr;
		Node* cur = _root;
		while (cur)
		{
			//��ô˵��������ֵ�ȸ��ڵ��ֵҪС�������ȥ����
			if (cur->_kv.first > kv.first)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (cur->_kv.first < kv.first) //���ұ߲���
			{
				parent = cur;
				cur = cur->_right;
			}
			else
			{
				//��ʱ˵����ʱ˵������ֵ����ˣ��ǾͲ��ܽ��в�����
				return make_pair(cur, false);
			}
		}

		cur = new Node(kv);
		//��ʱ˵���Ѿ��ҵ���λ�ã�����Ҫ���в���
		if (parent->_kv.first < kv.first)
		{
			//˵����ʱӦ�������ڸ��ڵ���ұ�
			parent->_right = cur;
			cur->_parent = parent;
		}
		else
		{
			parent->_left = cur;
			cur->_parent = parent;
		}


		Node* newnode = cur;
		//���������ɹ��ˣ���������Ӱ�����ȵ�ƽ�����ӣ���������Ҳ�ǺͶ����������Ĳ�ͬ�͹ؼ�����
		//1.����ƽ������
		while (parent)
		{
			if (cur == parent->_right)
			{
				parent->_bf++;
			}
			else
			{
				parent->_bf--;
			}

			if (parent->_bf == 0)
			{
				break;//˵��parent���������ĸ߶Ȳ��䣨û�и���ǰ��parent->_bf��1����-1����һ��ߣ����ڱ����0��˵���Ѱ���
				//�Ǳ߸������ˣ��������ٶ��ϲ�·���ϵĽ�㹹��Ӱ�죬���Ը��½�����
			}
			else if (abs(parent->_bf) == 1)
			{
				//˵��parent���ڵ������ĸ߶ȸı��ˣ�����һ������Ӱ��ģ���Ҫ�������ϸ���
				cur = parent;
				parent = parent->_parent;//������һ��
			}
			else if (abs(parent->_bf) == 2)
			{
				//˵��parent���ڵ������Ѿ�����ƽ���ˣ���Ҫ��ת����
				//��ʱ������ת������
				//1.�ҵ���
				//2.����
				//3.���������ҵ���
				//4.���ҵ���������
				if (parent->_bf == -2)
				{
					if (cur->_bf == -1)
					{
						RotateR(parent);
					}
					else //cur->_bf == 1
					{
						RotateLR(parent);
					}
				}
				else
				{
					if (cur->_bf == 1)
					{
						RotateL(parent);
					}
					else //cur->_bf == -1
					{
						RotateRL(parent);
					}
				}
				break;//��ת���֮�󣬾��Ѿ��ָ���ԭ����ƽ�����������
			}
			else
			{
				//�Ͳ������ߵ��������ߵ��ˣ�˵����ƽ������Ϊ2��ʱ�򣬾�û����ȷ�Ĵ���
				assert(false);
			}
		}

		//�����cur���ܻ�ı䣬��Ϊ�����Ͻ��и���,���Կ�������ı�������
		make_pair(newnode, true);
	}

	void RotateR(Node* parent)
	{
		Node* subL = parent->_left; 
		Node* subLR = subL->_right;

		parent->_left = subLR;
		//����subLR�п����ǿյģ���ô�����������ͻ��
		if (subLR)
			subLR->_parent = parent;

		Node* parentParent = parent->_parent;

		subL->_right = parent;  
		parent->_parent = subL;


		//��ʱ����Ҫ���һ�����Ѹ���㻻��
		if (parent == _root)
		{
			_root = subL;
			_root->_parent = nullptr;
		}
		else
		{
			//��ʱ����Ҫ��30����������ס������Ӧ����������һ�߻���Ҫ�����ж�
			if (parentParent->_left == parent)
			{
				parentParent->_left = subL;
			}
			else
			{
				parentParent->_right = subL;
			}
			subL->_parent = parentParent;
		}

		//����ƽ�����ӵĸ���
		subL->_bf = parent->_bf = 0;
	}

	void RotateL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		parent->_right = subRL;
		if (subRL)
			subRL->_parent = parent;
		
		Node* parentParent = parent->_parent;//�ȱ�����������Ϊ�����ı���������Ҳ����ʼ�ĸ��ڵ���
		
		subR->_left = parent;
		parent->_parent = subR;

		if (parent == _root)
		{
			_root = subR;
			subR->_parent = nullptr;
		}
		else
		{
			//��Ϊ������һ����
			if (parentParent->_left == parent)
			{
				parentParent->_left = subR;
			}
			else
			{
				parentParent->_right = subR;
			}
			subR->_parent = parentParent;	
		}
		//ƽ������
		subR->_bf = parent->_bf = 0;
	}


	void RotateRL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		int bf = subRL->_bf;

		RotateR(parent->_right);//������������������������ոı�ƽ�����ӣ�����ǰ����б��棬�Ա�����
		RotateL(parent);

		if (bf == 1)   // ˵��subRL���������루��ͼ���;���c����
		{
			subRL->_bf = 0;
			subR->_bf = 0;
			parent->_bf = -1;
		}
		else if (bf == -1)  // ˵��subRL���������루��ͼ���;���b����
		{
			subRL->_bf = 0;
			subR->_bf = 1;
			parent->_bf = 0;
		}
		else if(bf == 0)  //bf == 0˵��subLR���������
		{
			subRL->_bf = subR->_bf = parent->_bf = 0;
		}
		else
		{
			assert(false);
		}
	}

	void RotateLR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = parent->_left->_right;
		int bf = subLR->_bf;

		RotateL(parent->_left);//������������������������ոı�ƽ�����ӣ�����ǰ����б��棬�Ա�����
		RotateR(parent);

		//�����b����c����ڵ㣬����b����c�ĸ߶�������1���ͻ�����˫������Ҫ�ֿ����ۣ�b�������c���룬����ƽ�����Ӹ�����Ҫ�ֿ�������
		if (bf == 1) // ˵��subLR���������루��ͼ���;���c����
		{
			subLR->_bf = 0;//(�����ͼ�У�60������ƽ�����ӿ϶���0)
			parent->_bf = 0;
			subL->_bf = -1;
		}
		else if (bf == -1)   // ˵��subLR���������루��ͼ���;���b����
		{
			subLR->_bf = 0;
			parent->_bf = 1;
			subL->_bf = 0;
		}
		else if(bf == 0)//bf == 0˵��subLR���������
		{
			subL->_bf = subLR->_bf = parent->_bf = 0;
		}
		else
		{
			assert(false);
		}
	}

	int Height(Node* root)
	{
		if (root == nullptr)
		{
			return 0;
		}


		return Height(root->_left) > Height(root->_right) ? Height(root->_left) + 1 : Height(root->_right) + 1;
	}

	bool _IsBalance(Node* root)
	{
		if (root == nullptr)
		{
			return true;
		}

		int leftHeight = Height(root->_left); 
		int rightHeight = Height(root->_right);

		if (rightHeight - leftHeight != root->_bf)
		{
			cout << "ƽ�������쳣��" << endl;
		}
		return abs(rightHeight - leftHeight) < 2
			&& _IsBalance(root->_left)
			&& _IsBalance(root->_right);
		//��һ���ǵ�ǰ���ģ����������㻹�����ԣ�����Ҫ���ǵ�����Ҳ������ƽ������������Բſ���

	}

	bool IsBalance()
	{
		return _IsBalance(_root);
	}


	void _Inorder(Node*& root)
	{
		if (root == nullptr)
			return;
		_Inorder(root->_left);
		cout << root->_kv.first << " ";
		_Inorder(root->_right);
	}

	void Inorder()
	{
		_Inorder(_root);
	}

	//Node* Find(const K& key);
	//V& operator[](const K& key);
	//ɾ��̫���ĸ��ӣ������Ժ󶼲����漰
	//bool Erase(const K& key);




	//����ڲ�����Ч�ʵ�����£���ʵ�����Ѿ���������
	//��AVL��֮ǰ������һ�Ŷ���������,����Ҫ���ն������������в���
	//pair<Node*, bool> Insert(const pair<K, V>& kv)
	//{
	//	if (_root == nullptr)
	//	{
	//		_root = new Node(kv);
	//		return make_pair(_root, true);
	//	}
	//	Node* parent = nullptr;
	//	Node* cur = _root;
	//	while (cur)
	//	{
	//		//��ô˵��������ֵ�ȸ��ڵ��ֵҪС�������ȥ����
	//		if (cur->kv.first > kv.first)
	//		{
	//			parent = cur;
	//			cur = cur->_left;
	//		}
	//		else if (cur->kv.first < kv.first) //���ұ߲���
	//		{
	//			parent = cur;
	//			cur = cur->_right;
	//		}
	//		else
	//		{
	//			//��ʱ˵����ʱ˵������ֵ����ˣ��ǾͲ��ܽ��в�����
	//			return make_pair(cur, false);
	//		}
	//	}

	//	cur = new Node(kv);
	//	//��ʱ˵���Ѿ��ҵ���λ�ã�����Ҫ���в���
	//	if (parent->kv.first < kv.first)
	//	{
	//		//˵����ʱӦ�������ڸ��ڵ���ұ�
	//		parent->_right = cur;
	//		cur->_parent = parent;
	//	}
	//	else
	//	{
	//		parent->_left = cur;
	//		cur->_parent = parent;
	//	}
	//	make_pair(cur, true);
	//}
private:
	Node* _root = nullptr;
};