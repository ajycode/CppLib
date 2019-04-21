#pragma once

namespace Ajy
{

	namespace NSRBTree2
	{

		template<class Value>
		RBTreeNode<Value>* RBTreeNode<Value>::Minimum(RBTreeNode* node)
		{
			while (!IsNil(node->left))
				node = node->left;
			return node;
		}

		template<class Value>
		RBTreeNode<Value>* RBTreeNode<Value>::Maximum(RBTreeNode* node)
		{
			while (!IsNil(node->right))
				node = node->right;
			return node;
		}

		template<class Value>
		RBTreeNode<Value>* RBTreeNode<Value>::Successor(RBTreeNode* node)
		{
			if (IsNil(node)) return node->left;      //nil�ڵ�ĺ�����Ϊ��������ڵ�,���㷴������ReverseIterator���������,REnd()->ReverseIterator(nil),--REnd()
			if (!IsNil(node->right)) return Minimum(node->right);
			RBTreeNode* tmp = node->parent;
			while (!IsNil(tmp) && node == tmp->right)
			{
				node = tmp;
				tmp = tmp->parent;
			}
			return tmp;
		}

		template<class Value>
		RBTreeNode<Value>* RBTreeNode<Value>::Predecessor(RBTreeNode* node)
		{
			if (IsNil(node)) return node->right;      //nil�ڵ��ǰ�����Ϊ�������ҽڵ�,���㷴������Iterator���������,End()->Iterator(nil),--End()
			if (!IsNil(node->left)) return Maximum(node->left);
			RBTreeNode* tmp = node->parent;
			while (!IsNil(tmp) && node == tmp->left)
			{
				node = tmp;
				tmp = tmp->parent;
			}
			return tmp;
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
			if (!Node::IsNil(copy.root))
			{
				CopyTree(nil, copy.root);
				nil->left = Node::Minimum(root);
				nil->right = Node::Maximum(root);
				nodeCount = copy.nodeCount;
			}
		}

		template<class Key, class Value, class Compare, class KeyOfValue>
		RBTree<Key, Value, Compare, KeyOfValue>::RBTree(RBTree&& copy)
			//: pool(std::move(copy.pool))
			: nil(copy.nil)
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
				if (!Node::IsNil(copy.root))
				{
					CopyTree(nil, copy.root);
					nil->left = Node::Minimum(root);
					nil->right = Node::Maximum(root);
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
				//pool = std::move(copy.pool);
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
			/*pool.Init<Node>(capacity);
			nil = (NodePtr)pool.FastAlloc();*/
			nil = new Node;
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
			while (!Node::IsNil(search))
			{
				if (!keyCompare(GetKey(search), k))
				{
					psearch = search;
					search = search->left;
				}
				else
					search = search->right;
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
			//NodePtr insertNode = (NodePtr)pool.Alloc();
			NodePtr insertNode = new Node;
			new (&insertNode->val) Value(val);
			if (Node::IsNil(insertParent)) //����������һ�ſ���  
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
			InsertFixUp(insertNode);
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
			while (!Node::IsNil(search))
			{
				psearch = search;
				compRes = keyCompare(key, GetKey(search));
				search = compRes ? search->left : search->right;
			}
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
			if (keyCompare(GetKey(iter.node), key))
				//-------------�ж������Ƿ��Ѵ���Ҫ�������ݵ�keyֵ-------end
				return std::pair<Iterator, bool>(Insert(psearch, val), true);
			if (replace)
				new (&iter.node->val) Value(val);
			return std::pair<Iterator, bool>(iter, false);
		}

		template<class Key, class Value, class Compare, class KeyOfValue>
		std::pair<typename RBTree<Key, Value, Compare, KeyOfValue>::Iterator, bool>
			RBTree<Key, Value, Compare, KeyOfValue>::Insert(bool replace, Value&& val)
		{
			NodePtr psearch = nil;
			NodePtr search = root;
			bool compRes = true;
			Value v(std::forward<Value>(val));
			Key key = kov(v);
			while (!Node::IsNil(search))
			{
				psearch = search;
				compRes = keyCompare(key, GetKey(search));
				search = compRes ? search->left : search->right;
			}
			// �뿪whileѭ����psearchΪ����ڵ�֮���ڵ�
			Iterator iter = Iterator(psearch);
			//-------------�ж������Ƿ��Ѵ���Ҫ�������ݵ�keyֵ-------begin
			if (compRes)
			{
				if (iter == Begin())
					return std::pair<Iterator, bool>(Insert(psearch, v), true);
				else
					--iter;
			}
			if (keyCompare(GetKey(iter.node), key))
				//-------------�ж������Ƿ��Ѵ���Ҫ�������ݵ�keyֵ-------end
				return std::pair<Iterator, bool>(Insert(psearch, v), true);
			if (replace)
				new (&iter.node->val) Value(v);
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
			while (!Node::IsNil(search))
			{
				psearch = search;
				search = keyCompare(key, GetKey(search)) ? search->left : search->right;
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
			while (!Node::IsNil(search))
			{
				psearch = search;
				search = keyCompare(key, GetKey(search)) ? search->left : search->right;
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
			RBTree<Key, Value, Compare, KeyOfValue>::Delete(NodePtr del)
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

			if (!del || Node::IsNil(del)) return End();
			NodePtr nextNode = Node::Successor(del);
			if (!Node::IsNil(del->left) && !Node::IsNil(del->right))
			{
				NodePtr next = Node::Minimum(del->right);      //�Һ�̽ڵ�
				Ajy::Swap(del->color, next->color);
				NodePtr delLeft = del->left;
				NodePtr delRight = del->right;
				NodePtr delParent = del->parent;
				NodePtr nextLeft = next->left;
				NodePtr nextRight = next->right;
				NodePtr nextParent = next->parent;
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
				if (!Node::IsNil(nextRight))
				{
					nextRight->parent = del;
				}
				else
				{
					if (next == FarRight())
						nil->right = del;
				}
				if (!Node::IsNil(nextLeft))
					nextLeft->parent = del;
				if (Node::IsNil(delParent))
					root = next;
				else if (isLeftChild)
					delParent->left = next;
				else
					delParent->right = next;
			}
			if (!Node::IsNil(del->left))
			{
				//�����Լ�������
				del->left->color = RBTree_BLACK;		//���ú��ӽڵ����ɫ��del�ڵ�һ��,���ݽ���1�ɵ�del�ڵ����ɫΪ��ɫ
				if (del == root)
					root = del->left;
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
			else if (!Node::IsNil(del->right))
			{
				//�����Լ����Һ���
				del->right->color = RBTree_BLACK;		//���ú��ӽڵ����ɫ��del�ڵ�һ��,���ݽ���1�ɵ�del�ڵ����ɫΪ��ɫ
				if (del == root)
					root = del->right;
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
				DeleteFixUp(del);
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
			Dispose(del);
			--nodeCount;
			return Iterator(nextNode);
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
			/*dispose->val.~Value();
			dispose->left = nullptr;
			dispose->right = nullptr;
			dispose->parent = nullptr;
			pool.Free(dispose);*/
			delete dispose;
			dispose = nullptr;
		}

		template<class Key, class Value, class Compare, class KeyOfValue>
		void RBTree<Key, Value, Compare, KeyOfValue>::Destory(NodePtr destory)
		{
			/*while (!Node::IsNil(destory))
			{
			Destory(destory->right);
			NodePtr tmp = destory->left;
			Dispose(destory);
			--nodeCount;
			destory = tmp;
			}*/
			if (Node::IsNil(destory))
				return;
			else
			{
				Destory(destory->left);
				Destory(destory->right);
				Dispose(destory);
				--nodeCount;
			}
		}

		template<class Key, class Value, class Compare, class KeyOfValue>
		typename RBTree<Key, Value, Compare, KeyOfValue>::NodePtr
			RBTree<Key, Value, Compare, KeyOfValue>::CloneNode(NodePtr node)
		{
			//NodePtr tmp = (NodePtr)pool.Alloc();
			NodePtr tmp = new Node;
			new (&tmp->val) Value(node->val);
			tmp->color = node->color;
			tmp->left = nil;
			tmp->right = nil;
			return tmp;
		}

		template<class Key, class Value, class Compare, class KeyOfValue>
		void RBTree<Key, Value, Compare, KeyOfValue>::CopyTree(NodePtr dstParent, NodePtr src)
		{
			if (Node::IsNil(src)) return;
			NodePtr dst = CloneNode(src);
			if (Node::IsNil(src->parent))           //���ڵ�
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
			CopyTree(dst, src->left);
			CopyTree(dst, src->right);
		}

		template<class Key, class Value, class Compare, class KeyOfValue>
		void RBTree<Key, Value, Compare, KeyOfValue>::LeftRotate(NodePtr rotateNode)
		{
			NodePtr tmp = rotateNode->right;
			rotateNode->right = tmp->left;
			if (!Node::IsNil(tmp->left))
				tmp->left->parent = rotateNode;
			tmp->parent = rotateNode->parent;

			if (Node::IsNil(rotateNode->parent))
				root = tmp;
			else if (rotateNode == rotateNode->parent->left)
				rotateNode->parent->left = tmp;
			else
				rotateNode->parent->right = tmp;
			tmp->left = rotateNode;
			rotateNode->parent = tmp;
		}

		template<class Key, class Value, class Compare, class KeyOfValue>
		void RBTree<Key, Value, Compare, KeyOfValue>::RightRotate(NodePtr rotateNode)
		{
			NodePtr tmp = rotateNode->left;
			rotateNode->left = tmp->right;
			if (!Node::IsNil(tmp->right))
				tmp->right->parent = rotateNode;
			tmp->parent = rotateNode->parent;

			if (Node::IsNil(rotateNode->parent))
				root = tmp;
			else if (rotateNode == rotateNode->parent->left)
				rotateNode->parent->left = tmp;
			else
				rotateNode->parent->right = tmp;
			tmp->right = rotateNode;
			rotateNode->parent = tmp;
		}

		template<class Key, class Value, class Compare, class KeyOfValue>
		void RBTree<Key, Value, Compare, KeyOfValue>::InsertFixUp(NodePtr fixupNode)
		{
			while (Node::IsRed(fixupNode->parent))
			{
				if (fixupNode->parent == fixupNode->parent->parent->left)   //  
				{
					NodePtr uncle = fixupNode->parent->parent->right;
					if (Node::IsRed(uncle))   //�������1��fixupNode������uncle�Ǻ�ɫ�ġ�  
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
							LeftRotate(fixupNode);
						}
						//�������3�����2����תΪ���3
						fixupNode->parent->color = RBTree_BLACK;
						fixupNode->parent->parent->color = RBTree_RED;
						RightRotate(fixupNode->parent->parent);
					}
				}
				else
				{
					NodePtr uncle = fixupNode->parent->parent->left;
					if (Node::IsRed(uncle))
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
							RightRotate(fixupNode);     //������������ȣ�������Ϊ����  
						}
						fixupNode->parent->color = RBTree_BLACK;
						fixupNode->parent->parent->color = RBTree_RED;
						LeftRotate(fixupNode->parent->parent);   //������Ϊ���������ɡ�  
					}
				}
			}
			root->color = RBTree_BLACK;
		}

		template<class Key, class Value, class Compare, class KeyOfValue>
		void RBTree<Key, Value, Compare, KeyOfValue>::DeleteFixUp(NodePtr fixupNode)
		{
			while (fixupNode != root && Node::IsBlack(fixupNode))
			{
				if (fixupNode == fixupNode->parent->left)
				{
					NodePtr brother = fixupNode->parent->right;
					if (Node::IsRed(brother))         //���1��fixupNode���ֵ�brother�Ǻ�ɫ�ġ� 
					{
						brother->color = RBTree_BLACK;
						fixupNode->parent->color = RBTree_RED;
						LeftRotate(fixupNode->parent);
						brother = fixupNode->parent->right;
					}
					if (Node::IsBlack(brother->left) && Node::IsBlack(brother->right))        //���2:fixupNode���ֵ�brother���������Ӷ��Ǻ�ɫ��
					{
						brother->color = RBTree_RED;
						fixupNode = fixupNode->parent;
					}
					else
					{
						if (Node::IsBlack(brother->right))         //���3��fixupNode���ֵ�brother�������Ǻ�ɫ���Һ����Ǻ�ɫ
						{
							brother->left->color = RBTree_BLACK;
							brother->color = RBTree_RED;
							RightRotate(brother);
							brother = fixupNode->parent->right;
						}
						//���4:brother�ĺ��Ӷ��Ǻ�ɫ
						brother->color = fixupNode->parent->color;
						fixupNode->parent->color = RBTree_BLACK;
						brother->right->color = RBTree_BLACK;
						LeftRotate(fixupNode->parent);
						fixupNode = root;
					}
				}
				else
				{
					NodePtr brother = fixupNode->parent->left;
					if (Node::IsRed(brother))         //���1��fixupNode���ֵ�brother�Ǻ�ɫ�ġ� 
					{
						brother->color = RBTree_BLACK;
						fixupNode->parent->color = RBTree_RED;
						RightRotate(fixupNode->parent);
						brother = fixupNode->parent->left;
					}
					if (Node::IsBlack(brother->right) && Node::IsBlack(brother->left))        //���2:fixupNode���ֵ�brother���������Ӷ��Ǻ�ɫ��
					{
						brother->color = RBTree_RED;
						fixupNode = fixupNode->parent;
					}
					else
					{
						if (Node::IsBlack(brother->left))         //���3��fixupNode���ֵ�brother�������Ǻ�ɫ���Һ����Ǻ�ɫ
						{
							brother->right->color = RBTree_BLACK;
							brother->color = RBTree_RED;
							LeftRotate(brother);
							brother = fixupNode->parent->left;
						}
						//���4:brother�ĺ��Ӷ��Ǻ�ɫ
						brother->color = fixupNode->parent->color;
						fixupNode->parent->color = RBTree_BLACK;
						brother->left->color = RBTree_BLACK;
						RightRotate(fixupNode->parent);
						fixupNode = root;
					}
				}
			}
			fixupNode->color = RBTree_BLACK;
		}

		template<class Key, class Value, class Compare, class KeyOfValue>
		typename RBTree<Key, Value, Compare, KeyOfValue>::Iterator
			RBTree<Key, Value, Compare, KeyOfValue>::LowerBound(Key const& k)
		{
			NodePtr psearch = nil;
			NodePtr search = root;
			while (!Node::IsNil(search))
			{
				if (!keyCompare(GetKey(search), k))
				{
					psearch = search;
					search = search->left;
				}
				else
					search = search->right;
			}
			return Iterator(psearch);
		}

		template<class Key, class Value, class Compare, class KeyOfValue>
		typename RBTree<Key, Value, Compare, KeyOfValue>::ConstIterator
			RBTree<Key, Value, Compare, KeyOfValue>::LowerBound(Key const& k) const
		{
			return ConstIterator(const_cast<RBTree<Key, Value, Compare, KeyOfValue>*>(this)->LowerBound(k).node);
		}

		template<class Key, class Value, class Compare, class KeyOfValue>
		typename RBTree<Key, Value, Compare, KeyOfValue>::Iterator
			RBTree<Key, Value, Compare, KeyOfValue>::UpperBound(Key const& k)
		{
			NodePtr psearch = nil;
			NodePtr search = root;
			while (!Node::IsNil(search))
			{
				if (keyCompare(k, GetKey(search)))
				{
					psearch = search;
					search = search->left;
				}
				else
					search = search->right;
			}
			return Iterator(psearch);
		}

		template<class Key, class Value, class Compare, class KeyOfValue>
		typename RBTree<Key, Value, Compare, KeyOfValue>::ConstIterator
			RBTree<Key, Value, Compare, KeyOfValue>::UpperBound(Key const& k) const
		{
			return ConstIterator(const_cast<RBTree<Key, Value, Compare, KeyOfValue>*>(this)->UpperBound(k).node);
		}

	}

}
