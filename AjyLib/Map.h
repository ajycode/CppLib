#pragma once

namespace Ajy
{

	class ByteBuffer;

#define Use_RBTree2 1


	template<class Key, class Value, class Compare = std::less<Key>>
	class Map : public MemoryMoveable
	{
	public:
		typedef std::pair<Key const, Value> RBTreeData;
#if !Use_RBTree2
		typedef typename RBTree<Key, RBTreeData, Compare, Get1st<RBTreeData>>::Iterator Iterator;
		typedef typename RBTree<Key, RBTreeData, Compare, Get1st<RBTreeData>>::ConstIterator ConstIterator;
		typedef typename RBTree<Key, RBTreeData, Compare, Get1st<RBTreeData>>::ReverseIterator ReverseIterator;
		typedef typename RBTree<Key, RBTreeData, Compare, Get1st<RBTreeData>>::ConstReverseIterator ConstReverseIterator;
#else
		typedef typename NSRBTree2::RBTree<Key, RBTreeData, Compare, Get1st<RBTreeData>>::Iterator Iterator;
		typedef typename NSRBTree2::RBTree<Key, RBTreeData, Compare, Get1st<RBTreeData>>::ConstIterator ConstIterator;
		typedef typename NSRBTree2::RBTree<Key, RBTreeData, Compare, Get1st<RBTreeData>>::ReverseIterator ReverseIterator;
		typedef typename NSRBTree2::RBTree<Key, RBTreeData, Compare, Get1st<RBTreeData>>::ConstReverseIterator ConstReverseIterator;
#endif
	private:
#if !Use_RBTree2
		RBTree<Key, RBTreeData, Compare, Get1st<RBTreeData>> rbtree;
#else
		NSRBTree2::RBTree<Key, RBTreeData, Compare, Get1st<RBTreeData>> rbtree;
#endif
	public:
		Map();
		Map(size_t capacity);
		Map(Map const& copy);
		Map(Map&& copy);
		Map& operator=(Map const& copy);
		Map& operator=(Map&& copy);
		~Map() {}
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
		Value& operator[](Key const& k) { return rbtree.Insert(false, RBTreeData(k, Value())).first->second; }
		std::pair<Iterator, bool> Insert(RBTreeData const& data, bool replace = false) { return rbtree.Insert(replace, data); }
		std::pair<Iterator, bool> Insert(RBTreeData&& data, bool replace = false) { return rbtree.Insert(replace, std::forward<RBTreeData>(data)); }
		template<class Iter>
		void Insert(Iter first, Iter last) { rbtree.Insert(first, last); }
		void Insert(ConstIterator first, ConstIterator last) { rbtree.Insert(first, last); }
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

	template<class Key, class Value, class Compare = std::less<Key>>
	class MultiMap : public MemoryMoveable
	{
	public:
#if !Use_RBTree2
		typedef std::pair<const Key, Value> RBTreeData;
		typedef typename RBTree<Key, RBTreeData, Compare, Get1st<RBTreeData>>::Iterator Iterator;
		typedef typename RBTree<Key, RBTreeData, Compare, Get1st<RBTreeData>>::ConstIterator ConstIterator;
		typedef typename RBTree<Key, RBTreeData, Compare, Get1st<RBTreeData>>::ReverseIterator ReverseIterator;
		typedef typename RBTree<Key, RBTreeData, Compare, Get1st<RBTreeData>>::ConstReverseIterator ConstReverseIterator;
#else
		typedef std::pair<const Key, Value> RBTreeData;
		typedef typename NSRBTree2::RBTree<Key, RBTreeData, Compare, Get1st<RBTreeData>>::Iterator Iterator;
		typedef typename NSRBTree2::RBTree<Key, RBTreeData, Compare, Get1st<RBTreeData>>::ConstIterator ConstIterator;
		typedef typename NSRBTree2::RBTree<Key, RBTreeData, Compare, Get1st<RBTreeData>>::ReverseIterator ReverseIterator;
		typedef typename NSRBTree2::RBTree<Key, RBTreeData, Compare, Get1st<RBTreeData>>::ConstReverseIterator ConstReverseIterator;
#endif
	private:
#if !Use_RBTree2
		RBTree<Key, RBTreeData, Compare, Get1st<RBTreeData>> rbtree;
#else
		NSRBTree2::RBTree<Key, RBTreeData, Compare, Get1st<RBTreeData>> rbtree;
#endif
	public:
		MultiMap();
		MultiMap(size_t capacity);
		MultiMap(MultiMap const& copy);
		MultiMap(MultiMap&& copy);
		MultiMap& operator=(MultiMap const& copy);
		MultiMap& operator=(MultiMap&& copy);
		~MultiMap() {}
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
		Iterator Insert(RBTreeData const& data) { return rbtree.InsertMulti(data); }
		Iterator Insert(RBTreeData&& data) { return rbtree.InsertMulti(std::forward<RBTreeData>(data)); }
		template<class Iter>
		void Insert(Iter first, Iter last) { rbtree.InsertMulti(first, last); }
		void Insert(ConstIterator first, ConstIterator last) { rbtree.InsertMulti(first, last); }
		Iterator Delete(Iterator pos) { return rbtree.Delete(pos); }
		void Delete(Iterator first, Iterator last) { rbtree.Delete(first, last); }
		size_t Delete(Key const& k) { return rbtree.Delete(k); }
		void Clear() { rbtree.Clear(); }
	};

}
