#pragma once

namespace Ajy
{

#define Use_RBTree 0

	template<class Key, class Compare = std::less<Key>>
	class Set : public MemoryMoveable
	{
	public:
		typedef Key RBTreeData;
#if Use_RBTree
		typedef typename RBTree<Key, RBTreeData, Compare, GetWhole<RBTreeData>>::Iterator Iterator;
		typedef typename RBTree<Key, RBTreeData, Compare, GetWhole<RBTreeData>>::ConstIterator ConstIterator;
		typedef typename RBTree<Key, RBTreeData, Compare, GetWhole<RBTreeData>>::ReverseIterator ReverseIterator;
		typedef typename RBTree<Key, RBTreeData, Compare, GetWhole<RBTreeData>>::ConstReverseIterator ConstReverseIterator;
#else
		typedef typename NSRBTree2::RBTree<Key, RBTreeData, Compare, GetWhole<RBTreeData>>::Iterator Iterator;
		typedef typename NSRBTree2::RBTree<Key, RBTreeData, Compare, GetWhole<RBTreeData>>::ConstIterator ConstIterator;
		typedef typename NSRBTree2::RBTree<Key, RBTreeData, Compare, GetWhole<RBTreeData>>::ReverseIterator ReverseIterator;
		typedef typename NSRBTree2::RBTree<Key, RBTreeData, Compare, GetWhole<RBTreeData>>::ConstReverseIterator ConstReverseIterator;
#endif
	private:
#if Use_RBTree
		RBTree<Key, RBTreeData, Compare, GetWhole<RBTreeData>> rbtree;
#else
		NSRBTree2::RBTree<Key, RBTreeData, Compare, GetWhole<RBTreeData>> rbtree;
#endif
	public:
		Set();
		Set(size_t capacity);
		Set(Set const& copy);
		Set(Set&& copy);
		Set& operator=(Set const& copy);
		Set& operator=(Set&& copy);
		~Set() {}
		//若要支持C++11 range-based for循环，Begin和End首字母小写即可
		Iterator Begin() { return rbtree.Begin(); }
		ConstIterator Begin() const { return rbtree.Begin(); }
		Iterator End() { return rbtree.End(); }
		ConstIterator End() const { return rbtree.End(); }
		ReverseIterator RBegin() { return rbtree.RBegin(); }
		ConstReverseIterator RBegin() const { return rbtree.RBegin(); }
		ReverseIterator REnd() { return rbtree.REnd(); }
		ConstReverseIterator REnd() const { return rbtree.REnd(); }
		bool Empty() const { return rbtree.Empty(); }
		size_t Size() const { return rbtree.Size(); }
		size_t Count(Key const& k) const { return rbtree.Count(k); }
		Iterator Find(Key const& k) { return rbtree.Find(k); }
		ConstIterator Find(Key const& k) const { return rbtree.Find(k); }
		std::pair<Iterator, bool> Insert(RBTreeData const& data) { return rbtree.Insert(false, data); }
		std::pair<Iterator, bool> Insert(RBTreeData&& data) { return rbtree.Insert(false, std::forward<RBTreeData>(data)); }
		template<class Iter>
		void Insert(Iter first, Iter last) { rbtree.Insert(first, last); }
		Iterator Delete(Iterator pos) { return rbtree.Delete(pos); }
		void Delete(Iterator first, Iterator last) { rbtree.Delete(first, last); }
		size_t Delete(Key const& k) { return rbtree.Delete(k); }
		void Clear() { rbtree.Clear(); }
	};

	template<class Key, class Compare = std::less<Key>>
	class MultiSet : public MemoryMoveable
	{
	public:
		typedef Key RBTreeData;
#if Use_RBTree
		typedef typename RBTree<Key, RBTreeData, Compare, GetWhole<RBTreeData>>::Iterator Iterator;
		typedef typename RBTree<Key, RBTreeData, Compare, GetWhole<RBTreeData>>::ConstIterator ConstIterator;
		typedef typename RBTree<Key, RBTreeData, Compare, GetWhole<RBTreeData>>::ReverseIterator ReverseIterator;
		typedef typename RBTree<Key, RBTreeData, Compare, GetWhole<RBTreeData>>::ConstReverseIterator ConstReverseIterator;
#else
		typedef typename NSRBTree2::RBTree<Key, RBTreeData, Compare, GetWhole<RBTreeData>>::Iterator Iterator;
		typedef typename NSRBTree2::RBTree<Key, RBTreeData, Compare, GetWhole<RBTreeData>>::ConstIterator ConstIterator;
		typedef typename NSRBTree2::RBTree<Key, RBTreeData, Compare, GetWhole<RBTreeData>>::ReverseIterator ReverseIterator;
		typedef typename NSRBTree2::RBTree<Key, RBTreeData, Compare, GetWhole<RBTreeData>>::ConstReverseIterator ConstReverseIterator;
#endif
	private:
#if Use_RBTree
		RBTree<Key, RBTreeData, Compare, GetWhole<RBTreeData>> rbtree;
#else
		NSRBTree2::RBTree<Key, RBTreeData, Compare, GetWhole<RBTreeData>> rbtree;
#endif
	public:
		MultiSet();
		MultiSet(size_t capacity);
		MultiSet(MultiSet const& copy);
		MultiSet(MultiSet&& copy);
		MultiSet& operator=(MultiSet const& copy);
		MultiSet& operator=(MultiSet&& copy);
		~MultiSet() {}
		//若要支持C++11 range-based for循环，Begin和End首字母小写即可
		Iterator Begin() { return rbtree.Begin(); }
		ConstIterator Begin() const { return rbtree.Begin(); }
		Iterator End() { return rbtree.End(); }
		ConstIterator End() const { return rbtree.End(); }
		ReverseIterator RBegin() { return rbtree.RBegin(); }
		ConstReverseIterator RBegin() const { return rbtree.RBegin(); }
		ReverseIterator REnd() { return rbtree.REnd(); }
		ConstReverseIterator REnd() const { return rbtree.REnd(); }
		bool Empty() const { return rbtree.Empty(); }
		size_t Size() const { return rbtree.Size(); }
		size_t Count(Key const& k) const { return rbtree.Count(k); }
		Iterator Find(Key const& k) { return rbtree.Find(k); }
		Iterator Insert(RBTreeData const& data) { return rbtree.InsertMulti(data); }
		Iterator Insert(RBTreeData&& data) { return rbtree.InsertMulti(std::forward<RBTreeData>(data)); }
		template<class Iter>
		void Insert(Iter first, Iter last) { rbtree.InsertMulti(first, last); }
		Iterator Delete(Iterator pos) { return rbtree.Delete(pos); }
		void Delete(Iterator first, Iterator last) { rbtree.Delete(first, last); }
		size_t Delete(Key const& k) { return rbtree.Delete(k); }
		void Clear() { rbtree.Clear(); }

		// for Serialization
		void WriteTo(ByteBuffer& bb) const;
		bool ReadFrom(ByteBuffer& bb);
		bool ReadFrom(ByteBuffer& bb, size_t minLen, size_t maxLen);
		void VarWriteTo(ByteBuffer& bb) const;
		bool VarReadFrom(ByteBuffer& bb);
		bool VarReadFrom(ByteBuffer& bb, size_t minLen, size_t maxLen);
	};

}
