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
			if (IsNil(node)) return node->left;      //nil节点的后继设计为树的最左节点,方便反向利用ReverseIterator遍历红黑树,REnd()->ReverseIterator(nil),--REnd()
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
			if (IsNil(node)) return node->right;      //nil节点的前驱设计为树的最右节点,方便反向利用Iterator遍历红黑树,End()->Iterator(nil),--End()
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
			if (Node::IsNil(insertParent)) //如果插入的是一颗空树  
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
			// 离开while循环后，psearch为插入节点之父节点
			Iterator iter = Iterator(psearch);
			//-------------判断树中是否已存在要插入数据的key值-------begin
			if (compRes)
			{
				if (iter == Begin())
					return std::pair<Iterator, bool>(Insert(psearch, val), true);
				else
					--iter;
			}
			if (keyCompare(GetKey(iter.node), key))
				//-------------判断树中是否已存在要插入数据的key值-------end
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
			// 离开while循环后，psearch为插入节点之父节点
			Iterator iter = Iterator(psearch);
			//-------------判断树中是否已存在要插入数据的key值-------begin
			if (compRes)
			{
				if (iter == Begin())
					return std::pair<Iterator, bool>(Insert(psearch, v), true);
				else
					--iter;
			}
			if (keyCompare(GetKey(iter.node), key))
				//-------------判断树中是否已存在要插入数据的key值-------end
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
			1.      节点是红色或黑色
			2.      根是黑色
			3.      所有叶子都是黑色(叶子是NIL节点)
			4.      每个红色节点必须有两个黑色的子节点。（从每个叶子到根的所有路径上不能有两个连续的红色节点。）
			5.      从任一节点到其每个叶子的所有简单路径都包含相同数目的黑色节点。
			根据上面的5个性质，我们可以得出下面的结论：
			结论1：在红黑树中若x只有一棵非空子树，则x必为黑色。
			证明：假设x为红色，根据性质4可以推出x有两个黑色子节点。与x只有一棵非空子树矛盾。
			结论2：在红黑树中若x只有一棵非空子树。则该非空子树的根必为红色，且该非空子树仅且只有一个根节点。
			证明：假设y为x的左孩子，节点y的颜色为黑色，且y有子树。由于y是黑色，x的右孩子为空，所以从x到其左子树各叶子结点的路径上黑色结点数大于x到其右子树叶到叶节点的路径上黑色节点数，违反性质5，所以节点y为红色。因为y为红色，如果y的子树存在，根据性质4可以得出y的两棵子树必为黑色。从x到经过y到各叶节点的路径上的黑色节点数大于到右子树叶节点路径上的黑色节点数。
			同上所述，当y为x的右孩子时也可以证明结论2

			1. 从上面的两个结论可以看出，如果要删除的节点只有一个孩子，那么直接用孩子节点的值代替父节点的值。删除子节点就可以，不需要进入删除调整算法。
			2. 若当前要删除的节点两个孩子都不为空，此时我们只需要找到当前节点中序序列的后继节点。用后继节点的值替换当前节点的值。将后继节点作为新的当前节点，此时的当前节点一定只有一个右孩子或左右孩子都为空。
			3. 通过步骤2后如果当前节点有后继节点(有右孩子)，直接用其后继节点值替换当前节点值，不需要进入删除调整算法。如果当前节点没有后继节点(没有左右孩子)，进入删除调整算法。
			算法流程步骤：
			1.      判断x的左右子树是否为空。
			2.      如果x的左右子都不为空，找到中序序列中x的后继节点y，将x的值用y代替；
			3.      如果x的左子不空，将x的值用左子的值代替，删除x的左子，算法结束。否则转4
			4.      如果x的右子不空，将x的值用右子的值代替，删除x的右子，算法结束。否则转5
			5.      进入到红黑树删除节点调整程序,删除x
			*/

			if (!del || Node::IsNil(del)) return End();
			NodePtr nextNode = Node::Successor(del);
			if (!Node::IsNil(del->left) && !Node::IsNil(del->right))
			{
				NodePtr next = Node::Minimum(del->right);      //找后继节点
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
				//交换自己和左孩子
				del->left->color = RBTree_BLACK;		//设置孩子节点的颜色与del节点一致,根据结论1可得del节点的颜色为黑色
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
					nil->right = del->left;				//修改far right(nil->right)
			}
			else if (!Node::IsNil(del->right))
			{
				//交换自己和右孩子
				del->right->color = RBTree_BLACK;		//设置孩子节点的颜色与del节点一致,根据结论1可得del节点的颜色为黑色
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
					nil->left = del->right;				//修改far left(nil->left)
			}
			else
			{
				//左右子树都不存在，需要进入删除调整算法  
				DeleteFixUp(del);
				if (del == root)
				{
					root = nil;
					nil->left = root;
					nil->right = root;
				}
				else if (del == del->parent->left)
				{
					del->parent->left = nil;				//父节点的指针域需要修改
					if (del == FarLeft())
						nil->left = del->parent;			//修改far left(nil->left)
				}
				else if (del == del->parent->right)
				{
					del->parent->right = nil;				//父节点的指针域需要修改  
					if (del == FarRight())
						nil->right = del->parent;				//修改far right(nil->right)
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
			if (Node::IsNil(src->parent))           //根节点
			{
				root = dst;
			}
			else if (src == src->parent->left)       //左孩子
			{
				dstParent->left = dst;
			}
			else									//右孩子
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
					if (Node::IsRed(uncle))   //插入情况1，fixupNode的叔叔uncle是红色的。  
					{
						fixupNode->parent->color = RBTree_BLACK;
						uncle->color = RBTree_BLACK;
						uncle->parent->color = RBTree_RED;
						fixupNode = fixupNode->parent->parent;
					}
					else
					{
						if (fixupNode == fixupNode->parent->right) //插入情况2：fixupNode的叔叔uncle是黑色的，且fixupNode是右孩子  
						{
							fixupNode = fixupNode->parent;
							LeftRotate(fixupNode);
						}
						//插入情况3，情况2可以转为情况3
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
							RightRotate(fixupNode);     //与上述代码相比，左旋改为右旋  
						}
						fixupNode->parent->color = RBTree_BLACK;
						fixupNode->parent->parent->color = RBTree_RED;
						LeftRotate(fixupNode->parent->parent);   //右旋改为左旋，即可。  
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
					if (Node::IsRed(brother))         //情况1：fixupNode的兄弟brother是红色的。 
					{
						brother->color = RBTree_BLACK;
						fixupNode->parent->color = RBTree_RED;
						LeftRotate(fixupNode->parent);
						brother = fixupNode->parent->right;
					}
					if (Node::IsBlack(brother->left) && Node::IsBlack(brother->right))        //情况2:fixupNode的兄弟brother的两个孩子都是黑色的
					{
						brother->color = RBTree_RED;
						fixupNode = fixupNode->parent;
					}
					else
					{
						if (Node::IsBlack(brother->right))         //情况3：fixupNode的兄弟brother的左孩子是红色，右孩子是黑色
						{
							brother->left->color = RBTree_BLACK;
							brother->color = RBTree_RED;
							RightRotate(brother);
							brother = fixupNode->parent->right;
						}
						//情况4:brother的孩子都是红色
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
					if (Node::IsRed(brother))         //情况1：fixupNode的兄弟brother是红色的。 
					{
						brother->color = RBTree_BLACK;
						fixupNode->parent->color = RBTree_RED;
						RightRotate(fixupNode->parent);
						brother = fixupNode->parent->left;
					}
					if (Node::IsBlack(brother->right) && Node::IsBlack(brother->left))        //情况2:fixupNode的兄弟brother的两个孩子都是黑色的
					{
						brother->color = RBTree_RED;
						fixupNode = fixupNode->parent;
					}
					else
					{
						if (Node::IsBlack(brother->left))         //情况3：fixupNode的兄弟brother的左孩子是黑色，右孩子是红色
						{
							brother->right->color = RBTree_BLACK;
							brother->color = RBTree_RED;
							LeftRotate(brother);
							brother = fixupNode->parent->left;
						}
						//情况4:brother的孩子都是红色
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
