#pragma once

namespace Ajy
{

	namespace NSRBTree2
	{

		typedef bool RBTreeColor;
		RBTreeColor const RBTree_RED = false;
		RBTreeColor const RBTree_BLACK = true;

		template<class Value>
		struct RBTreeNode
		{
			static bool IsNil(RBTreeNode* node) { return node->parent == nullptr; }
			static bool IsRed(RBTreeNode* node) { return node->color == RBTree_RED; }
			static bool IsBlack(RBTreeNode* node) { return node->color == RBTree_BLACK; }
			static RBTreeNode* Minimum(RBTreeNode* node);
			static RBTreeNode* Maximum(RBTreeNode* node);
			static RBTreeNode* Successor(RBTreeNode* node);
			static RBTreeNode* Predecessor(RBTreeNode* node);
			RBTreeNode* left;
			RBTreeNode* right;
			RBTreeNode* parent;
			RBTreeColor color;
			Value val;
		};

		template<class Value, class Ref, class Ptr>
		struct RBTreeIterator
		{
			RBTreeNode<Value>* node;
			RBTreeIterator() : node(nullptr) {}
			explicit RBTreeIterator(RBTreeNode<Value>* p) : node(p) {}
			//-------------用于将Iterator赋值给ConstIterator--------------begin
			RBTreeIterator(RBTreeIterator<Value, Value&, Value*> const& it) :node(it.node) {}
			//-------------------------------------------------------------end
			friend bool operator==(RBTreeIterator const& l, RBTreeIterator const& r)
			{
				return l.node == r.node;
			}
			friend bool operator!=(RBTreeIterator const& l, RBTreeIterator const& r)
			{
				return !(l.node == r.node);
			}
			RBTreeIterator& operator++()
			{
				node = RBTreeNode<Value>::Successor(node);
				return *this;
			}
			RBTreeIterator operator++(int) { RBTreeIterator iter = *this; ++(*this); return iter; }
			RBTreeIterator& operator--()
			{
				node = RBTreeNode<Value>::Predecessor(node);
				return *this;
			}
			RBTreeIterator operator--(int) { RBTreeIterator iter = *this; --(*this); return iter; }
			Ref operator*() { return node->val; }
			Ptr operator->() { return &node->val; }
			static size_t Distance(RBTreeIterator first, RBTreeIterator const last)
			{
				size_t num = 0;
				while (first != last)
				{
					++first;
					++num;
				}
				return num;
			}
		};

		template<class Value, class Ref, class Ptr>
		struct RBTreeReverseIterator
		{
			RBTreeNode<Value>* node;
			RBTreeReverseIterator() : node(nullptr) {}
			explicit RBTreeReverseIterator(RBTreeNode<Value>* p) : node(p) {}
			//-------------用于将Iterator赋值给ConstIterator--------------begin
			RBTreeReverseIterator(RBTreeReverseIterator<Value, Value&, Value*> const& it) :node(it.node) {}
			//-------------------------------------------------------------end
			friend bool operator==(RBTreeReverseIterator const& l, RBTreeReverseIterator const& r)
			{
				return l.node == r.node;
			}
			friend bool operator!=(RBTreeReverseIterator const& l, RBTreeReverseIterator const& r)
			{
				return !(l.node == r.node);
			}
			RBTreeReverseIterator& operator++()
			{
				node = RBTreeNode<Value>::Predecessor(node);
				return *this;
			}
			RBTreeReverseIterator operator++(int) { RBTreeReverseIterator iter = *this; ++(*this); return iter; }
			RBTreeReverseIterator& operator--()
			{
				node = RBTreeNode<Value>::Successor(node);
				return *this;
			}
			RBTreeReverseIterator operator--(int) { RBTreeReverseIterator iter = *this; --(*this); return iter; }
			Ref operator*() { return node->val; }
			Ptr operator->() { return &node->val; }
		};

		//比较器参数类型必须严格弱序，即可以使用std::less,std::greater，不能使用std::less_equal,std::greater_equal
		template<class Key, class Value, class Compare, class KeyofValue>
		class RBTree :MemoryMoveable
		{
		public:
			typedef RBTreeNode<Value> Node;
			typedef RBTreeNode<Value>* NodePtr;
			typedef RBTreeIterator<Value, Value&, Value*> Iterator;
			typedef RBTreeIterator<Value, Value const&, Value const*> ConstIterator;
			typedef RBTreeReverseIterator<Value, Value&, Value*> ReverseIterator;
			typedef RBTreeReverseIterator<Value, Value const&, Value const*> ConstReverseIterator;
		private:
			NodePtr root = nullptr;		//父节点为nil
			NodePtr nil = nullptr;        //哨兵结点(Color为Black，父节点为nullptr(用于区分nil节点和普通节点),左孩子为树的最左节点，右孩子为树的最右节点)
			size_t nodeCount;
			Compare keyCompare;
			KeyofValue kov;
			//MemoryPool pool;
		public:
			explicit RBTree(size_t capacity);
			RBTree(RBTree const& copy);
			RBTree(RBTree&& copy);
			RBTree& operator=(RBTree const& copy);
			RBTree& operator=(RBTree&& copy);
			~RBTree();
			Iterator Begin() { return Iterator(nil->left); }
			ConstIterator Begin() const { return ConstIterator(nil->left); }
			Iterator End() { return Iterator(nil); }
			ConstIterator End() const { return ConstIterator(nil); }
			ReverseIterator RBegin() { return ReverseIterator(nil->right); }
			ConstReverseIterator RBegin() const { return ConstReverseIterator(nil->right); }
			ReverseIterator REnd() { return ReverseIterator(nil); }
			ConstReverseIterator REnd() const { return ConstReverseIterator(nil); }
			bool Empty() const { return nodeCount == 0; }
			size_t Size() const { return nodeCount; }
			size_t Count(Key const& k) const;
			Iterator Find(Key const& k);
			ConstIterator Find(Key const& k) const;
			std::pair<Iterator, bool> Insert(bool replace, Value const& val);
			std::pair<Iterator, bool> Insert(bool replace, Value&& val);
			template<class Iter>
			void Insert(Iter first, Iter last);
			void Insert(ConstIterator first, ConstIterator last);
			Iterator InsertMulti(Value const& val);
			Iterator InsertMulti(Value&& val);
			template<class Iter>
			void InsertMulti(Iter first, Iter last);
			void InsertMulti(ConstIterator first, ConstIterator last);
			Iterator Delete(Iterator pos) { return Delete(pos.node); }
			void Delete(Iterator first, Iterator last);
			size_t Delete(Key const& k);
			void Clear();
			Iterator LowerBound(Key const& k);
			ConstIterator LowerBound(Key const& k) const;
			Iterator UpperBound(Key const& k);
			ConstIterator UpperBound(Key const& k) const;
		private:
			void Init(size_t capacity);
			NodePtr FarLeft() const { return nil->left; }
			NodePtr FarRight() const { return nil->right; }
			void Dispose(NodePtr dispose);
			void Destory(NodePtr destory);
			NodePtr CloneNode(NodePtr node);
			void CopyTree(NodePtr dstParent, NodePtr src);
			Key const& GetKey(NodePtr node) { return kov(node->val); }
			Iterator Insert(NodePtr insertParent, Value const& val);
			Iterator Delete(NodePtr del);
			void LeftRotate(NodePtr rotateNode);
			void RightRotate(NodePtr rotateNode);
			void InsertFixUp(NodePtr fixupNode);
			void DeleteFixUp(NodePtr fixupNode);
		};

	}

}
