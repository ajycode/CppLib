#pragma once

namespace Ajy
{

	RBTreeNodeBase* Minimum(RBTreeNodeBase* node)
	{
		while (!IsNil(node->left))
			node = node->left;
		return node;
	}

	RBTreeNodeBase* Maximum(RBTreeNodeBase* node)
	{
		while (!IsNil(node->right))
			node = node->right;
		return node;
	}

	void LeftRotate(RBTreeNodeBase* rotateNode, RBTreeNodeBase*& rootNode)
	{
		RBTreeNodeBase* tmp = rotateNode->right;
		rotateNode->right = tmp->left;
		if (!IsNil(tmp->left))
			tmp->left->parent = rotateNode;
		tmp->parent = rotateNode->parent;

		if (IsNil(rotateNode->parent))
			rootNode = tmp;
		else if (rotateNode == rotateNode->parent->left)
			rotateNode->parent->left = tmp;
		else
			rotateNode->parent->right = tmp;
		tmp->left = rotateNode;
		rotateNode->parent = tmp;
	}

	void RightRotate(RBTreeNodeBase* rotateNode, RBTreeNodeBase*& rootNode)
	{
		RBTreeNodeBase* tmp = rotateNode->left;
		rotateNode->left = tmp->right;
		if (!IsNil(tmp->right))
			tmp->right->parent = rotateNode;
		tmp->parent = rotateNode->parent;

		if (IsNil(rotateNode->parent))
			rootNode = tmp;
		else if (rotateNode == rotateNode->parent->left)
			rotateNode->parent->left = tmp;
		else
			rotateNode->parent->right = tmp;
		tmp->right = rotateNode;
		rotateNode->parent = tmp;
	}

	void InsertRebalance(RBTreeNodeBase* fixupNode, RBTreeNodeBase*& rootNode)
	{
		while (IsRed(fixupNode->parent))
		{
			if (fixupNode->parent == fixupNode->parent->parent->left)   //  
			{
				RBTreeNodeBase* uncle = fixupNode->parent->parent->right;
				if (IsRed(uncle))   //�������1��fixupNode������uncle�Ǻ�ɫ�ġ�  
				{
					fixupNode->parent->color = RBTree_BLACK;
					uncle->color = RBTree_BLACK;
					uncle->parent->color = RBTree_RED;
					fixupNode = fixupNode->parent->parent;
				}
				else
				{
					if (fixupNode == fixupNode->parent->right) //�������2��fixupNode������uncle�Ǻ�ɫ�ģ���fixupNode���Һ���  
					{
						fixupNode = fixupNode->parent;
						LeftRotate(fixupNode, rootNode);
					}
					//�������3�����2����תΪ���3
					fixupNode->parent->color = RBTree_BLACK;
					fixupNode->parent->parent->color = RBTree_RED;
					RightRotate(fixupNode->parent->parent, rootNode);
				}
			}
			else
			{
				RBTreeNodeBase* uncle = fixupNode->parent->parent->left;
				if (IsRed(uncle))
				{
					fixupNode->parent->color = RBTree_BLACK;
					uncle->color = RBTree_BLACK;
					uncle->parent->color = RBTree_RED;
					fixupNode = fixupNode->parent->parent;
				}
				else
				{
					if (fixupNode == fixupNode->parent->left)
					{
						fixupNode = fixupNode->parent;
						RightRotate(fixupNode, rootNode);     //������������ȣ�������Ϊ����  
					}
					fixupNode->parent->color = RBTree_BLACK;
					fixupNode->parent->parent->color = RBTree_RED;
					LeftRotate(fixupNode->parent->parent, rootNode);   //������Ϊ���������ɡ�  
				}
			}
		}
		rootNode->color = RBTree_BLACK;
	}

	void EraseRebalance(RBTreeNodeBase* fixupNode, RBTreeNodeBase*& rootNode)
	{
		while (fixupNode != rootNode && IsBlack(fixupNode))
		{
			if (fixupNode == fixupNode->parent->left)
			{
				RBTreeNodeBase* brother = fixupNode->parent->right;
				if (IsRed(brother))         //���1��fixupNode���ֵ�brother�Ǻ�ɫ�ġ� 
				{
					brother->color = RBTree_BLACK;
					fixupNode->parent->color = RBTree_RED;
					LeftRotate(fixupNode->parent, rootNode);
					brother = fixupNode->parent->right;
				}
				if (IsBlack(brother->left) && IsBlack(brother->right))        //���2:fixupNode���ֵ�brother���������Ӷ��Ǻ�ɫ��
				{
					brother->color = RBTree_RED;
					fixupNode = fixupNode->parent;
				}
				else
				{
					if (IsBlack(brother->right))         //���3��fixupNode���ֵ�brother�������Ǻ�ɫ���Һ����Ǻ�ɫ
					{
						brother->left->color = RBTree_BLACK;
						brother->color = RBTree_RED;
						RightRotate(brother, rootNode);
						brother = fixupNode->parent->right;
					}
					//���4:brother�ĺ��Ӷ��Ǻ�ɫ
					brother->color = fixupNode->parent->color;
					fixupNode->parent->color = RBTree_BLACK;
					brother->right->color = RBTree_BLACK;
					LeftRotate(fixupNode->parent, rootNode);
					fixupNode = rootNode;
				}
			}
			else
			{
				RBTreeNodeBase* brother = fixupNode->parent->left;
				if (IsRed(brother))         //���1��fixupNode���ֵ�brother�Ǻ�ɫ�ġ� 
				{
					brother->color = RBTree_BLACK;
					fixupNode->parent->color = RBTree_RED;
					RightRotate(fixupNode->parent, rootNode);
					brother = fixupNode->parent->left;
				}
				if (IsBlack(brother->right) && IsBlack(brother->left))        //���2:fixupNode���ֵ�brother���������Ӷ��Ǻ�ɫ��
				{
					brother->color = RBTree_RED;
					fixupNode = fixupNode->parent;
				}
				else
				{
					if (IsBlack(brother->left))         //���3��fixupNode���ֵ�brother�������Ǻ�ɫ���Һ����Ǻ�ɫ
					{
						brother->right->color = RBTree_BLACK;
						brother->color = RBTree_RED;
						LeftRotate(brother, rootNode);
						brother = fixupNode->parent->left;
					}
					//���4:brother�ĺ��Ӷ��Ǻ�ɫ
					brother->color = fixupNode->parent->color;
					fixupNode->parent->color = RBTree_BLACK;
					brother->left->color = RBTree_BLACK;
					RightRotate(fixupNode->parent, rootNode);
					fixupNode = rootNode;
				}
			}
		}
		fixupNode->color = RBTree_BLACK;
	}

	void Successor(RBTreeNodeBase*& node)
	{
		if (IsNil(node))
		{
			node = node->left;					//nil�ڵ�ĺ�����Ϊ��������ڵ�,���㷴������ReverseIterator���������,REnd()->ReverseIterator(nil),--REnd()
		}
		else
		{
			if (!IsNil(node->right))
			{
				node = Minimum(node->right);
			}
			else
			{
				RBTreeNodeBase* tmp = node->parent;
				while (!IsNil(tmp) && node == tmp->right)
				{
					node = tmp;
					tmp = tmp->parent;
				}
				node = tmp;
			}
		}
	}

	void Predecessor(RBTreeNodeBase*& node)
	{
		if (IsNil(node))
		{
			node = node->right;					//nil�ڵ��ǰ�����Ϊ�������ҽڵ�,���㷴������Iterator���������,End()->Iterator(nil),--End()
		}
		else
		{
			if (!IsNil(node->left))
			{
				node = Maximum(node->left);
			}
			else
			{
				RBTreeNodeBase* tmp = node->parent;
				while (!IsNil(tmp) && node == tmp->left)
				{
					node = tmp;
					tmp = tmp->parent;
				}
				node = tmp;
			}
		}
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	RBTree<Key, Value, Compare, KeyOfValue>::RBTree(size_t capacity)
	{
		Init(capacity);
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	RBTree<Key, Value, Compare, KeyOfValue>::RBTree(RBTree const& copy)
		: RBTree(copy.Size())
	{
		if (!IsNil(copy.root))
		{
			CopyTree(nil, copy.root);
			nil->left = Minimum(root);
			nil->right = Maximum(root);
			nodeCount = copy.nodeCount;
		}
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	RBTree<Key, Value, Compare, KeyOfValue>::RBTree(RBTree&& copy)
		: pool(std::move(copy.pool))
		, nil(copy.nil)
		, root(copy.root)
		, nodeCount(copy.nodeCount)
	{
		copy.Init(32);
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	RBTree<Key, Value, Compare, KeyOfValue>&
		RBTree<Key, Value, Compare, KeyOfValue>::operator=(RBTree const& copy)
	{
		if (this != &copy)
		{
			Clear();
			if (!IsNil(copy.root))
			{
				CopyTree(nil, copy.root);
				nil->left = Minimum(root);
				nil->right = Maximum(root);
				nodeCount = copy.nodeCount;
			}
		}
		return *this;
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	RBTree<Key, Value, Compare, KeyOfValue>&
		RBTree<Key, Value, Compare, KeyOfValue>::operator=(RBTree&& copy)
	{
		if (this != &copy)
		{
			Clear();
			Dispose(nil);
			pool = std::move(copy.pool);
			nil = copy.nil;
			root = copy.root;
			nodeCount = copy.nodeCount;
			copy.Init(32);
		}
		return *this;
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	RBTree<Key, Value, Compare, KeyOfValue>::~RBTree()
	{
		if (nil)
		{
			Clear();
			Dispose(nil);
			nil = nullptr;
			root = nullptr;
		}
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	void RBTree<Key, Value, Compare, KeyOfValue>::Init(size_t capacity)
	{
		if (capacity <= 0) capacity = 32;
		else capacity = Round2n(capacity);
		pool.Init<Node>(capacity);
		nil = (NodePtr)pool.FastAlloc();
		new (&nil->val) Value();
		nil->left = nil;
		nil->right = nil;
		nil->parent = nullptr;
		nil->color = RBTree_BLACK;
		root = nil;
		nodeCount = 0;
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	size_t RBTree<Key, Value, Compare, KeyOfValue>::Count(Key const& k) const
	{
		size_t num = Iterator::Distance(LowerBound(k), UpperBound(k));
		return num;
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	typename RBTree<Key, Value, Compare, KeyOfValue>::Iterator
		RBTree<Key, Value, Compare, KeyOfValue>::Find(Key const& k)
	{
		NodePtr psearch = nil;
		NodePtr search = root;
		while (!IsNil(search))
		{
			if (!keyCompare(GetKey(search), k))
			{
				psearch = search;
				search = (NodePtr)search->left;
			}
			else
				search = (NodePtr)search->right;
		}
		return (psearch == nil || keyCompare(k, GetKey(psearch))) ? End() : Iterator(psearch);
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	typename RBTree<Key, Value, Compare, KeyOfValue>::ConstIterator
		RBTree<Key, Value, Compare, KeyOfValue>::Find(Key const& k) const
	{
		return ConstIterator(const_cast<RBTree<Key, Value, Compare, KeyOfValue>*>(this)->Find(k).node);
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	typename RBTree<Key, Value, Compare, KeyOfValue>::Iterator
		RBTree<Key, Value, Compare, KeyOfValue>::Insert(NodePtr insertParent, Value const& val)
	{
		NodePtr insertNode = (NodePtr)pool.Alloc();
		new (&insertNode->val) Value(val);
		if (IsNil(insertParent)) //����������һ�ſ���  
		{
			root = insertNode;
			insertParent->left = insertNode;
			insertParent->right = insertNode;
		}
		else
		{
			if (keyCompare(kov(val), GetKey(insertParent)))
			{
				insertParent->left = insertNode;
				if (insertParent == FarLeft())
					nil->left = insertNode;
			}
 			else
			{
				insertParent->right = insertNode;
				if (insertParent == FarRight())
					nil->right = insertNode;
			}
		}
		insertNode->color = RBTree_RED;
		insertNode->left = nil;
		insertNode->right = nil;
		insertNode->parent = insertParent;
		InsertRebalance(insertNode, (RBTreeNodeBase*&)root);
		++nodeCount;
		return Iterator(insertNode);
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	std::pair<typename RBTree<Key, Value, Compare, KeyOfValue>::Iterator, bool>
		RBTree<Key, Value, Compare, KeyOfValue>::Insert(bool replace, Value const& val)
	{
		NodePtr psearch = nil;
		NodePtr search = root;
		bool compRes = true;
		Key key = kov(val);
		// ����һ��ʹ���ڴ�ؽ�����ִ��Ч�ʣ�ԭ���д�����  --begin
		while (!IsNil(search))
		{
			psearch = search;
			compRes = keyCompare(key, GetKey(search));
			search = NodePtr(compRes ? search->left : search->right);
		}
		// ����һ��ʹ���ڴ�ؽ�����ִ��Ч�ʣ�ԭ���д�����  --end
		// �뿪whileѭ����psearchΪ����ڵ�֮���ڵ�
		Iterator iter = Iterator(psearch);
		//-------------�ж������Ƿ��Ѵ���Ҫ�������ݵ�keyֵ-------begin
		if (compRes)
		{
			if (iter == Begin())
				return std::pair<Iterator, bool>(Insert(psearch, val), true);
			else
				--iter;
		}
		if (keyCompare(GetKey(NodePtr(iter.node)), key))
			//-------------�ж������Ƿ��Ѵ���Ҫ�������ݵ�keyֵ-------end
			return std::pair<Iterator, bool>(Insert(psearch, val), true);
		if (replace)
			new (&NodePtr(iter.node)->val) Value(val);
		return std::pair<Iterator, bool>(iter, false);
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	std::pair<typename RBTree<Key, Value, Compare, KeyOfValue>::Iterator, bool>
		RBTree<Key, Value, Compare, KeyOfValue>::Insert(bool replace, Value&& val)
	{
		NodeBasePtr psearch = nil;
		NodeBasePtr search = root;
		bool compRes = true;
		Value v(std::forward<Value>(val));
		Key key = kov(v);
		// ����һ��ʹ���ڴ�ؽ�����ִ��Ч�ʣ�ԭ���д�����  --begin
		while (!IsNil(search))
		{
			psearch = search;
			compRes = keyCompare(key, GetKey((NodePtr)search));
			search = compRes ? search->left : search->right;
		}
		// ����һ��ʹ���ڴ�ؽ�����ִ��Ч�ʣ�ԭ���д�����  --end
		// �뿪whileѭ����psearchΪ����ڵ�֮���ڵ�
		Iterator iter = Iterator((NodePtr)psearch);
		//-------------�ж������Ƿ��Ѵ���Ҫ�������ݵ�keyֵ-------begin
		if (compRes)
		{
			if (iter == Begin())
				return std::pair<Iterator, bool>(Insert((NodePtr)psearch, v), true);
			else
				--iter;
		}
		if (keyCompare(GetKey(NodePtr(iter.node)), key))
			//-------------�ж������Ƿ��Ѵ���Ҫ�������ݵ�keyֵ-------end
			return std::pair<Iterator, bool>(Insert((NodePtr)psearch, v), true);
		if (replace)
			new (&NodePtr(iter.node)->val) Value(std::forward<Value>(val));
		return std::pair<Iterator, bool>(iter, false);
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	template<class Iter>
	void RBTree<Key, Value, Compare, KeyOfValue>::Insert(Iter first, Iter last)
	{
		for (; first != last; ++first)
			Insert(false, *first);
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	void RBTree<Key, Value, Compare, KeyOfValue>::Insert(ConstIterator first, ConstIterator last)
	{
		for (; first != last; ++first)
			Insert(false, *first);
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	typename RBTree<Key, Value, Compare, KeyOfValue>::Iterator
		RBTree<Key, Value, Compare, KeyOfValue>::InsertMulti(Value const& val)
	{
		NodePtr psearch = nil;
		NodePtr search = root;
		Key key = kov(val);
		while (!IsNil(search))
		{
			psearch = search;
			search = NodePtr(keyCompare(key, GetKey(search)) ? search->left : search->right);
		}
		return Insert(psearch, val);
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	typename RBTree<Key, Value, Compare, KeyOfValue>::Iterator
		RBTree<Key, Value, Compare, KeyOfValue>::InsertMulti(Value&& val)
	{
		NodePtr psearch = nil;
		NodePtr search = root;
		Value v(std::forward<Value>(val));
		Key key = kov(v);
		while (!IsNil(search))
		{
			psearch = search;
			search = NodePtr(keyCompare(key, GetKey(search)) ? search->left : search->right);
		}
		return Insert(psearch, v);
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	template<class Iter>
	void RBTree<Key, Value, Compare, KeyOfValue>::InsertMulti(Iter first, Iter last)
	{
		for (; first != last; ++first)
			InsertMulti(*first);
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	void RBTree<Key, Value, Compare, KeyOfValue>::InsertMulti(ConstIterator first, ConstIterator last)
	{
		for (; first != last; ++first)
			InsertMulti(*first);
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	typename RBTree<Key, Value, Compare, KeyOfValue>::Iterator
		RBTree<Key, Value, Compare, KeyOfValue>::Delete(NodeBasePtr del)
	{
		/*
		1.      �ڵ��Ǻ�ɫ���ɫ
		2.      ���Ǻ�ɫ
		3.      ����Ҷ�Ӷ��Ǻ�ɫ(Ҷ����NIL�ڵ�)
		4.      ÿ����ɫ�ڵ������������ɫ���ӽڵ㡣����ÿ��Ҷ�ӵ���������·���ϲ��������������ĺ�ɫ�ڵ㡣��
		5.      ����һ�ڵ㵽��ÿ��Ҷ�ӵ����м�·����������ͬ��Ŀ�ĺ�ɫ�ڵ㡣
		���������5�����ʣ����ǿ��Եó�����Ľ��ۣ�
		����1���ں��������xֻ��һ�÷ǿ���������x��Ϊ��ɫ��
		֤��������xΪ��ɫ����������4�����Ƴ�x��������ɫ�ӽڵ㡣��xֻ��һ�÷ǿ�����ì�ܡ�
		����2���ں��������xֻ��һ�÷ǿ���������÷ǿ������ĸ���Ϊ��ɫ���Ҹ÷ǿ���������ֻ��һ�����ڵ㡣
		֤��������yΪx�����ӣ��ڵ�y����ɫΪ��ɫ����y������������y�Ǻ�ɫ��x���Һ���Ϊ�գ����Դ�x������������Ҷ�ӽ���·���Ϻ�ɫ���������x����������Ҷ��Ҷ�ڵ��·���Ϻ�ɫ�ڵ�����Υ������5�����Խڵ�yΪ��ɫ����ΪyΪ��ɫ�����y���������ڣ���������4���Եó�y������������Ϊ��ɫ����x������y����Ҷ�ڵ��·���ϵĺ�ɫ�ڵ������ڵ�������Ҷ�ڵ�·���ϵĺ�ɫ�ڵ�����
		ͬ����������yΪx���Һ���ʱҲ����֤������2

		1. ��������������ۿ��Կ��������Ҫɾ���Ľڵ�ֻ��һ�����ӣ���ôֱ���ú��ӽڵ��ֵ���游�ڵ��ֵ��ɾ���ӽڵ�Ϳ��ԣ�����Ҫ����ɾ�������㷨��
		2. ����ǰҪɾ���Ľڵ��������Ӷ���Ϊ�գ���ʱ����ֻ��Ҫ�ҵ���ǰ�ڵ��������еĺ�̽ڵ㡣�ú�̽ڵ��ֵ�滻��ǰ�ڵ��ֵ������̽ڵ���Ϊ�µĵ�ǰ�ڵ㣬��ʱ�ĵ�ǰ�ڵ�һ��ֻ��һ���Һ��ӻ����Һ��Ӷ�Ϊ�ա�
		3. ͨ������2�������ǰ�ڵ��к�̽ڵ�(���Һ���)��ֱ�������̽ڵ�ֵ�滻��ǰ�ڵ�ֵ������Ҫ����ɾ�������㷨�������ǰ�ڵ�û�к�̽ڵ�(û�����Һ���)������ɾ�������㷨��
		�㷨���̲��裺
		1.      �ж�x�����������Ƿ�Ϊ�ա�
		2.      ���x�������Ӷ���Ϊ�գ��ҵ�����������x�ĺ�̽ڵ�y����x��ֵ��y���棻
		3.      ���x�����Ӳ��գ���x��ֵ�����ӵ�ֵ���棬ɾ��x�����ӣ��㷨����������ת4
		4.      ���x�����Ӳ��գ���x��ֵ�����ӵ�ֵ���棬ɾ��x�����ӣ��㷨����������ת5
		5.      ���뵽�����ɾ���ڵ��������,ɾ��x
		*/
		
		if (!del || IsNil(del)) return End();
		NodeBasePtr nextNode = del;
		Successor(nextNode);
		if (!IsNil(del->left) && !IsNil(del->right))
		{
			NodeBasePtr next = Minimum(del->right);      //�Һ�̽ڵ�
			Ajy::Swap(del->color, next->color);
			NodeBasePtr delLeft = del->left;
			NodeBasePtr delRight = del->right;
			NodeBasePtr delParent = del->parent;
			NodeBasePtr nextLeft = next->left;
			NodeBasePtr nextRight = next->right;
			NodeBasePtr nextParent = next->parent;
			bool isLeftChild = false;
			if (del == delParent->left)
				isLeftChild = true;
			next->parent = delParent;
			next->left = delLeft;
			delLeft->parent = next;
			del->left = nextLeft;
			del->right = nextRight;
			if (delRight != next)
			{
				next->right = delRight;
				del->parent = nextParent;
				delRight->parent = next;
				nextParent->left = del;
			}
			else
			{
				next->right = del;
				del->parent = next;
			}
			if (!IsNil(nextRight))
			{
				nextRight->parent = del;
			}
			else
			{
				if (next == FarRight())
					nil->right = del;
			}
			if (!IsNil(nextLeft))
				nextLeft->parent = del;
			if (IsNil(delParent))
				root = NodePtr(next);
			else if (isLeftChild)
				delParent->left = next;
			else
				delParent->right = next;
		}
		if (!IsNil(del->left))
		{
			//�����Լ�������
			del->left->color = RBTree_BLACK;		//���ú��ӽڵ����ɫ��del�ڵ�һ��,���ݽ���1�ɵ�del�ڵ����ɫΪ��ɫ
			if (del == root)
				root = NodePtr(del->left);
			else
			{
				if (del == del->parent->left)
					del->parent->left = del->left;
				else
					del->parent->right = del->left;
			}
			del->left->parent = del->parent;
			if (del == FarRight())
				nil->right = del->left;				//�޸�far right(nil->right)
		}
		else if (!IsNil(del->right))
		{
			//�����Լ����Һ���
			del->right->color = RBTree_BLACK;		//���ú��ӽڵ����ɫ��del�ڵ�һ��,���ݽ���1�ɵ�del�ڵ����ɫΪ��ɫ
			if (del == root)
				root = NodePtr(del->right);
			else
			{
				if (del == del->parent->left)
					del->parent->left = del->right;
				else
					del->parent->right = del->right;
			}
			del->right->parent = del->parent;
			if (del == FarLeft())
				nil->left = del->right;				//�޸�far left(nil->left)
		}
		else
		{
			//���������������ڣ���Ҫ����ɾ�������㷨  
			EraseRebalance(del, (NodeBasePtr&)root);
			if (del == root)
			{
				root = nil;
				nil->left = root;
				nil->right = root;
			}
			else if (del == del->parent->left)
			{
				del->parent->left = nil;				//���ڵ��ָ������Ҫ�޸�
				if (del == FarLeft())
					nil->left = del->parent;			//�޸�far left(nil->left)
			}
			else if (del == del->parent->right)
			{
				del->parent->right = nil;				//���ڵ��ָ������Ҫ�޸�  
				if (del == FarRight())
					nil->right = del->parent;				//�޸�far right(nil->right)
			}
		}
		Dispose((NodePtr)del);
		--nodeCount;
		return Iterator((NodePtr)nextNode);
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	void RBTree<Key, Value, Compare, KeyOfValue>::Delete(Iterator first, Iterator last)
	{
		if (first == Begin() && last == End())
			Clear();
		else
			while (first != last)
				Delete(first++);
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	size_t RBTree<Key, Value, Compare, KeyOfValue>::Delete(Key const& k)
	{
		Iterator first = LowerBound(k);
		Iterator last = UpperBound(k);
		size_t num = Iterator::Distance(first, last);
		Delete(first, last);
		return num;
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	void RBTree<Key, Value, Compare, KeyOfValue>::Clear()
	{
		Destory(root);
		nil->left = nil;
		nil->right = nil;
		root = nil;
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	void RBTree<Key, Value, Compare, KeyOfValue>::Dispose(NodePtr dispose)
	{
		dispose->val.~Value();
		dispose->left = nullptr;
		dispose->right = nullptr;
		dispose->parent = nullptr;
		pool.Free(dispose);
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	void RBTree<Key, Value, Compare, KeyOfValue>::Destory(NodePtr destory)
	{
		/*while (!IsNil(destory))
		{
			Destory(destory->right);
			NodePtr tmp = destory->left;
			Dispose(destory);
			--nodeCount;
			destory = tmp;
		}*/
		if (IsNil(destory))
			return;
		else
		{
			Destory(NodePtr(destory->left));
			Destory(NodePtr(destory->right));
			Dispose(destory);
			--nodeCount;
		}
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	typename RBTree<Key, Value, Compare, KeyOfValue>::NodePtr
		RBTree<Key, Value, Compare, KeyOfValue>::CloneNode(NodePtr node)
	{
		NodePtr tmp = (NodePtr)pool.Alloc();
		new (&tmp->val) Value(node->val);
		tmp->color = node->color;
		tmp->left = nil;
		tmp->right = nil;
		return tmp;
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	void RBTree<Key, Value, Compare, KeyOfValue>::CopyTree(NodePtr dstParent, NodePtr src)
	{
		if (IsNil(src)) return;
		NodePtr dst = CloneNode(src);
		if (IsNil(src->parent))           //���ڵ�
		{
			root = dst;
		}
		else if (src == src->parent->left)       //����
		{
			dstParent->left = dst;
		}
		else									//�Һ���
		{
			dstParent->right = dst;
		}
		dst->parent = dstParent;
		CopyTree(dst, (NodePtr)src->left);
		CopyTree(dst, (NodePtr)src->right);
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	typename RBTree<Key, Value, Compare, KeyOfValue>::Iterator
		RBTree<Key, Value, Compare, KeyOfValue>::LowerBound(Key const& k)
	{
		NodePtr psearch = nil;
		NodePtr search = root;
		while (!IsNil(search))
		{
			if (!keyCompare(GetKey(search), k))
			{
				psearch = search;
				search = NodePtr(search->left);
			}
			else
				search = NodePtr(search->right);
		}
		return Iterator(psearch);
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	typename RBTree<Key, Value, Compare, KeyOfValue>::ConstIterator
		RBTree<Key, Value, Compare, KeyOfValue>::LowerBound(Key const& k) const
	{
		NodePtr psearch = nil;
		NodePtr search = root;
		while (!IsNil(search))
		{
			if (!keyCompare(GetKey(search), k))
			{
				psearch = search;
				search = NodePtr(search->left);
			}
			else
				search = NodePtr(search->right);
		}
		return ConstIterator(psearch);
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	typename RBTree<Key, Value, Compare, KeyOfValue>::Iterator
		RBTree<Key, Value, Compare, KeyOfValue>::UpperBound(Key const& k)
	{
		NodePtr psearch = nil;
		NodePtr search = root;
		while (!IsNil(search))
		{
			if (keyCompare(k, GetKey(search)))
			{
				psearch = search;
				search = NodePtr(search->left);
			}
			else
				search = NodePtr(search->right);
		}
		return Iterator(psearch);
	}

	template<class Key, class Value, class Compare, class KeyOfValue>
	typename RBTree<Key, Value, Compare, KeyOfValue>::ConstIterator
		RBTree<Key, Value, Compare, KeyOfValue>::UpperBound(Key const& k) const
	{
		NodePtr psearch = nil;
		NodePtr search = root;
		while (!IsNil(search))
		{
			if (keyCompare(k, GetKey(search)))
			{
				psearch = search;
				search = NodePtr(search->left);
			}
			else
				search = NodePtr(search->right);
		}
		return ConstIterator(psearch);
	}

}
