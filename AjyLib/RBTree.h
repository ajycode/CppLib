#pragma once

namespace Ajy
{

	//typedef bool RBTreeColor;
	using RBTreeColor = bool;
	RBTreeColor const RBTree_RED = false;
	RBTreeColor const RBTree_BLACK = true;

	struct RBTreeNodeBase
	{
		RBTreeColor color;
		RBTreeNodeBase* left;
		RBTreeNodeBase* right;
		RBTreeNodeBase* parent;
	};

	inline bool IsNil(RBTreeNodeBase* node) { return node->parent == nullptr; }
	inline bool IsRed(RBTreeNodeBase* node) { return node->color == RBTree_RED; }
	inline bool IsBlack(RBTreeNodeBase* node) { return node->color == RBTree_BLACK; }
	inline RBTreeNodeBase* Minimum(RBTreeNodeBase* node);
	inline RBTreeNodeBase* Maximum(RBTreeNodeBase* node);
	inline void LeftRotate(RBTreeNodeBase* rotateNode, RBTreeNodeBase*& rootNode);
	inline void RightRotate(RBTreeNodeBase* rotateNode, RBTreeNodeBase*& rootNode);
	inline void InsertRebalance(RBTreeNodeBase* fixupNode, RBTreeNodeBase*& rootNode);
	inline void EraseRebalance(RBTreeNodeBase* fixupNode, RBTreeNodeBase*& rootNode);
	inline void Successor(RBTreeNodeBase*& node);
	inline void Predecessor(RBTreeNodeBase*& node);

	template<class Value>
	struct RBTreeNode:public RBTreeNodeBase
	{
		Value val;
	};

	struct RBTreeIteratorBase
	{
		RBTreeNodeBase* node;
	};

	inline bool operator==(RBTreeIteratorBase const& x, RBTreeIteratorBase const& y)
	{
		return x.node == y.node;
	}

	inline bool operator!=(RBTreeIteratorBase const& x, RBTreeIteratorBase const& y)
	{
		return x.node != y.node;
	}

	template<class Value, class Ref, class Ptr>
	struct RBTreeIterator :public RBTreeIteratorBase
	{
		RBTreeIterator() { node = nullptr; }
		explicit RBTreeIterator(RBTreeNode<Value>* p) { node = p; }
		//-------------用于将Iterator赋值给ConstIterator--------------begin
		RBTreeIterator(RBTreeIterator<Value, Value&, Value*> const& it) { node = it.node; }
		//-------------------------------------------------------------end
		RBTreeIterator& operator++() { Successor(node); return *this; }
		RBTreeIterator operator++(int) { RBTreeIterator iter = *this; ++(*this); return iter; }
		RBTreeIterator& operator--() { Predecessor(node); return *this; }
		RBTreeIterator operator--(int) { RBTreeIterator iter = *this; --(*this); return iter; }
		Ref operator*() { return ((RBTreeNode<Value>*)node)->val; }
		Ptr operator->() { return &(operator*()); }
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
	struct RBTreeReverseIterator :public RBTreeIteratorBase
	{
		RBTreeReverseIterator() { node = nullptr; }
		explicit RBTreeReverseIterator(RBTreeNode<Value>* p) { node = p; }
		//-------------用于将Iterator赋值给ConstIterator--------------begin
		RBTreeReverseIterator(RBTreeReverseIterator<Value, Value&, Value*> const& it) { node = it.node; }
		//-------------------------------------------------------------end
		RBTreeReverseIterator& operator++() { Predecessor(node); return *this; }
		RBTreeReverseIterator operator++(int) { RBTreeReverseIterator iter = *this; ++(*this); return iter; }
		RBTreeReverseIterator& operator--() { Successor(node); return *this; }
		RBTreeReverseIterator operator--(int) { RBTreeReverseIterator iter = *this; --(*this); return iter; }
		Ref operator*() { return ((RBTreeNode<Value>*)node)->val; }
		Ptr operator->() { return &(operator*()); }
	};

	//比较器参数类型必须严格弱序，即可以使用std::less,std::greater，不能使用std::less_equal,std::greater_equal
	template<class Key, class Value, class Compare, class KeyofValue>
	class RBTree :MemoryMoveable
	{
	public:
		typedef RBTreeNodeBase	NodeBase;
		typedef	RBTreeNodeBase*	NodeBasePtr;
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
		MemoryPool pool;
	public:
		explicit RBTree(size_t capacity);
		RBTree(RBTree const& copy);
		RBTree(RBTree&& copy);
		RBTree& operator=(RBTree const& copy);
		RBTree& operator=(RBTree&& copy);
		~RBTree();
		//若要支持C++11 range-based for循环，Begin和End首字母小写即可
		Iterator Begin() { return Iterator((NodePtr)nil->left); }
		ConstIterator Begin() const { return ConstIterator((NodePtr)nil->left); }
		Iterator End() { return Iterator(nil); }
		ConstIterator End() const { return ConstIterator(nil); }
		ReverseIterator RBegin() { return ReverseIterator((NodePtr)nil->right); }
		ConstReverseIterator RBegin() const { return ConstReverseIterator((NodePtr)nil->right); }
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
		NodePtr FarLeft() { return NodePtr(nil->left); }
		NodePtr FarRight() { return NodePtr(nil->right); }
		void Dispose(NodePtr dispose);
		void Destory(NodePtr destory);
		NodePtr CloneNode(NodePtr node);
		void CopyTree(NodePtr dstParent, NodePtr src);
		Key const& GetKey(NodePtr node) { return kov(node->val); }
		Iterator Insert(NodePtr insertParent, Value const& val);
		Iterator Delete(NodeBasePtr del);
	};

}
