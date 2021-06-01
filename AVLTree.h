#pragma once

#include<iostream>
#include<assert.h>
using namespace std;

template<class K,class V>
struct AVLTreeNode
{
	//使用三叉连,这样也比较便于找到parent节点
	AVLTreeNode<K,V>* _left;
	AVLTreeNode<K, V>* _right;
	AVLTreeNode<K, V>* _parent;
	pair<K, V> _kv;

	int _bf;//平衡因子  左右高度差 用来衡量这棵树是否平衡

	AVLTreeNode(const pair<K, V>& kv)
		:_left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		, _kv(kv)
		, _bf(0) //刚创建出来的节点左右子树都是空，所以平衡因子给0
	{}
};

template<class K, class V>
class AVLTree
{
	typedef AVLTreeNode<K,V> Node;
public:
	//对于AVL树也无非就是增删查，但是不能改
	//1.还需要更新平衡因子（因为你插入一个新的节点，那么他的祖先的平衡因子也会受到影响）
	//2.如果更新过程中|bf| > 1,就说明此时已经不再平衡，需要旋转
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
			//那么说明你插入的值比根节点的值要小，往左边去插入
			if (cur->_kv.first > kv.first)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (cur->_kv.first < kv.first) //往右边插入
			{
				parent = cur;
				cur = cur->_right;
			}
			else
			{
				//此时说明此时说明两个值相等了，那就不能进行插入了
				return make_pair(cur, false);
			}
		}

		cur = new Node(kv);
		//此时说明已经找到了位置，还需要进行插入
		if (parent->_kv.first < kv.first)
		{
			//说明此时应该链接在父节点的右边
			parent->_right = cur;
			cur->_parent = parent;
		}
		else
		{
			parent->_left = cur;
			cur->_parent = parent;
		}


		Node* newnode = cur;
		//在这里插入成功了，但是他会影响祖先的平衡因子，所以这里也是和二叉搜索树的不同和关键所在
		//1.更新平衡因子
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
				break;//说明parent所在子树的高度不变（没有更新前，parent->_bf是1或者-1，又一遍高，现在变成了0，说明把矮的
				//那边给填上了），不会再对上层路径上的结点构成影响，所以更新结束。
			}
			else if (abs(parent->_bf) == 1)
			{
				//说明parent所在的子树的高度改变了，对上一层是有影响的，需要继续向上更新
				cur = parent;
				parent = parent->_parent;//往上跳一层
			}
			else if (abs(parent->_bf) == 2)
			{
				//说明parent所在的子树已经不再平衡了，需要旋转处理
				//此时就是旋转的问题
				//1.右单旋
				//2.左单旋
				//3.先左单旋再右单旋
				//4.先右单旋再左单旋
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
				break;//旋转完成之后，就已经恢复了原来的平衡二叉树特性
			}
			else
			{
				//就不可能走到这里，如果走到了，说明在平衡因子为2的时候，就没有正确的处理
				assert(false);
			}
		}

		//这里的cur可能会改变，因为会向上进行更新,所以可以早早的保存下来
		make_pair(newnode, true);
	}

	void RotateR(Node* parent)
	{
		Node* subL = parent->_left; 
		Node* subLR = subL->_right;

		parent->_left = subLR;
		//但是subLR有可能是空的，那么下面这个代码就会崩
		if (subLR)
			subLR->_parent = parent;

		Node* parentParent = parent->_parent;

		subL->_right = parent;  
		parent->_parent = subL;


		//此时还需要最后一步，把根结点换掉
		if (parent == _root)
		{
			_root = subL;
			_root->_parent = nullptr;
		}
		else
		{
			//此时你需要把30这个结点连接住，但是应该连接在哪一边还需要进行判断
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

		//还有平衡因子的更新
		subL->_bf = parent->_bf = 0;
	}

	void RotateL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		parent->_right = subRL;
		if (subRL)
			subRL->_parent = parent;
		
		Node* parentParent = parent->_parent;//先保存下来，因为后面会改变这个，就找不到最开始的父节点了
		
		subR->_left = parent;
		parent->_parent = subR;

		if (parent == _root)
		{
			_root = subR;
			subR->_parent = nullptr;
		}
		else
		{
			//作为子树的一部分
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
		//平衡因子
		subR->_bf = parent->_bf = 0;
	}


	void RotateRL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		int bf = subRL->_bf;

		RotateR(parent->_right);//这里的左旋和右旋操作会最终改变平衡因子，所以前面进行保存，以便讨论
		RotateL(parent);

		if (bf == 1)   // 说明subRL是右树插入（看图解释就是c处）
		{
			subRL->_bf = 0;
			subR->_bf = 0;
			parent->_bf = -1;
		}
		else if (bf == -1)  // 说明subRL是左树插入（看图解释就是b处）
		{
			subRL->_bf = 0;
			subR->_bf = 1;
			parent->_bf = 0;
		}
		else if(bf == 0)  //bf == 0说明subLR是新增结点
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

		RotateL(parent->_left);//这里的左旋和右旋操作会最终改变平衡因子，所以前面进行保存，以便讨论
		RotateR(parent);

		//如果在b或者c插入节点，导致b或者c的高度增加了1，就会引起双旋并且要分开讨论，b插入或者c插入，树的平衡因子更新是要分开看待的
		if (bf == 1) // 说明subLR是右树插入（看图解释就是c处）
		{
			subLR->_bf = 0;//(类比于图中，60这个结点平衡因子肯定是0)
			parent->_bf = 0;
			subL->_bf = -1;
		}
		else if (bf == -1)   // 说明subLR是左树插入（看图解释就是b处）
		{
			subLR->_bf = 0;
			parent->_bf = 1;
			subL->_bf = 0;
		}
		else if(bf == 0)//bf == 0说明subLR是新增结点
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
			cout << "平衡因子异常：" << endl;
		}
		return abs(rightHeight - leftHeight) < 2
			&& _IsBalance(root->_left)
			&& _IsBalance(root->_right);
		//第一个是当前树的，但是他满足还不足以，还需要他们的子树也都满足平衡二叉树的特性才可以

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
	//删除太过的复杂，所以以后都不会涉及
	//bool Erase(const K& key);




	//如果在不考虑效率的情况下，其实现在已经插入完了
	//是AVL树之前首先是一颗二叉搜索树,所以要按照二叉搜索树进行插入
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
	//		//那么说明你插入的值比根节点的值要小，往左边去插入
	//		if (cur->kv.first > kv.first)
	//		{
	//			parent = cur;
	//			cur = cur->_left;
	//		}
	//		else if (cur->kv.first < kv.first) //往右边插入
	//		{
	//			parent = cur;
	//			cur = cur->_right;
	//		}
	//		else
	//		{
	//			//此时说明此时说明两个值相等了，那就不能进行插入了
	//			return make_pair(cur, false);
	//		}
	//	}

	//	cur = new Node(kv);
	//	//此时说明已经找到了位置，还需要进行插入
	//	if (parent->kv.first < kv.first)
	//	{
	//		//说明此时应该链接在父节点的右边
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