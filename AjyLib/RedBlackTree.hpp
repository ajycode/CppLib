#pragma once

namespace Ajy
{
	// 原型为RBTree2::RBTree
	// MSVC下natvis友好调试视图支持
	// 有set/multiset,map/multimap实现,用法和标准库一样
	// stl风格


	template<class config_t> class red_black_tree;


	template<class key_t, class comparator_t, class allocator_t, class unique_t>
	struct rbtree_set_config_t
	{
		typedef key_t key_type;
		typedef key_t mapped_type;
		typedef key_t value_type;
		typedef comparator_t key_compare;
		typedef allocator_t allocator_type;
		typedef unique_t unique_type;
		template<class in_type>
		static key_type const& get_key(in_type&& value)
		{
			return value;
		}
	};
	template<class value_t, class comparator_t = std::less<value_t>, class allocator_t = std::allocator<value_t>>
	using rbtree_set = red_black_tree<rbtree_set_config_t<value_t, comparator_t, allocator_t, std::true_type>>;
	template<class value_t, class comparator_t = std::less<value_t>, class allocator_t = std::allocator<value_t>>
	using rbtree_multiset = red_black_tree<rbtree_set_config_t<value_t, comparator_t, allocator_t, std::false_type>>;


	template<class key_t, class value_t, class comparator_t, class allocator_t, class unique_t>
	struct rbtree_map_config_t
	{
		typedef key_t key_type;
		typedef value_t mapped_type;
		typedef std::pair<key_t const, value_t> value_type;
		typedef comparator_t key_compare;
		typedef allocator_t allocator_type;
		typedef unique_t unique_type;
		template<class in_type>
		static key_type const& get_key(in_type&& value)
		{
			return value.first;
		}
	};
	template<class key_t, class value_t, class comparator_t = std::less<key_t>, class allocator_t = std::allocator<std::pair<key_t const, value_t>>>
	using rbtree_map = red_black_tree<rbtree_map_config_t<key_t, value_t, comparator_t, allocator_t, std::true_type>>;
	template<class key_t, class value_t, class comparator_t = std::less<key_t>, class allocator_t = std::allocator<std::pair<key_t const, value_t>>>
	using rbtree_multimap = red_black_tree<rbtree_map_config_t<key_t, value_t, comparator_t, allocator_t, std::false_type>>;



	namespace red_black_tree_detail
	{
		typedef bool color_t;
		color_t const color_red = false;
		color_t const color_black = true;
	}

	template<class config_t>
	class red_black_tree
	{
	public:
		typedef typename config_t::key_type key_type;
		typedef typename config_t::mapped_type mapped_type;
		typedef typename config_t::value_type value_type;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;
		typedef typename config_t::key_compare key_compare;
		typedef typename config_t::allocator_type allocator_type;
		typedef value_type& reference;
		typedef value_type const& const_reference;
		typedef value_type* pointer;
		typedef value_type const* const_pointer;
		typedef red_black_tree_detail::color_t color_t;
	protected:
		struct node_t
		{
			color_t color;
			node_t* parent;
			node_t* left;
			node_t* right;
			value_type value;
			static bool is_nil(node_t* node) { return node->parent == nullptr; }
			static bool is_red(node_t* node) { return node->color == red_black_tree_detail::color_red; }
			static bool is_black(node_t* node) { return node->color == red_black_tree_detail::color_black; }
			static node_t* minimum(node_t* node)
			{
				while (!is_nil(node->left))
					node = node->left;
				return node;
			}
			static node_t* maximum(node_t* node)
			{
				while (!is_nil(node->right))
					node = node->right;
				return node;
			}
			static node_t* successor(node_t* node)
			{
				if (is_nil(node)) return node->left;      //nil节点的后继设计为树的最左节点,方便反向利用ReverseIterator遍历红黑树
				if (!is_nil(node->right)) return minimum(node->right);
				node_t* tmp = node->parent;
				while (!is_nil(tmp) && node == tmp->right)
				{
					node = tmp;
					tmp = tmp->parent;
				}
				return tmp;
			}
			static node_t* predecessor(node_t* node)
			{
				if (is_nil(node)) return node->right;      //nil节点的前驱设计为树的最右节点,方便反向利用Iterator遍历红黑树
				if (!is_nil(node->left)) return maximum(node->left);
				node_t* tmp = node->parent;
				while (!is_nil(tmp) && node == tmp->left)
				{
					node = tmp;
					tmp = tmp->parent;
				}
				return tmp;
			}
		};

		typedef node_t* node_ptr;
		typedef typename allocator_type::template rebind<node_t>::other node_allocator_t;
		struct root_t : public key_compare, public node_allocator_t
		{
			template<class any_key_compare, class any_allocator_t>
			root_t(any_key_compare&& comp, any_allocator_t&& alloc)
				: key_compare(std::forward<any_key_compare>(comp))
				, node_allocator_t(std::forward<any_allocator_t>(alloc))
			{
			}
			size_type size;
			node_ptr root;	// 父节点为nil
			node_ptr nil;	// 哨兵结点(Color为Black，父节点为nullptr(用于区分nil节点和普通节点),左孩子为树的最左节点，右孩子为树的最右节点)
		};

		template<class k_t, class v_t>
		struct get_key_select_t
		{
			key_type const& operator()(value_type const& value)
			{
				return config_t::get_key(value);
			}

			template<class ...args_t>
			key_type const& operator()(key_type const& in, args_t&&... args)
			{
				return (*this)(in);
			}
		};
		template<class k_t>
		struct get_key_select_t<k_t, k_t>
		{
			key_type const& operator()(key_type const& value)
			{
				return config_t::get_key(value);
			}

			template<class in_t, class ...args_t>
			key_type operator()(in_t const& in, args_t const&... args)
			{
				return key_type(in, args...);
			}
		};
		typedef get_key_select_t<key_type, value_type> get_key_t;
		
	public:

		class iterator
		{
		public:
			typedef std::bidirectional_iterator_tag iterator_category;
			typedef typename red_black_tree::value_type value_type;
			typedef typename red_black_tree::difference_type difference_type;
			typedef typename red_black_tree::reference reference;
			typedef typename red_black_tree::pointer pointer;
		public:
			iterator()
				: node(nullptr)
			{
			}
			iterator(node_ptr in_node)
				: node(in_node)
			{
			}
			iterator(iterator const&) = default;
			iterator& operator++()
			{
				node = node_t::successor(node);
				return *this;
			}
			iterator& operator--()
			{
				node = node_t::predecessor(node);
				return *this;
			}
			iterator operator++(int)
			{
				iterator save(*this);
				++*this;
				return save;
			}
			iterator operator--(int)
			{
				iterator save(*this);
				--*this;
				return save;
			}
			reference operator *() const
			{
				return node->value;
			}
			pointer operator->() const
			{
				return &node->value;
			}
			bool operator == (iterator const& other) const
			{
				return node == other.node;
			}
			bool operator != (iterator const& other) const
			{
				return node != other.node;
			}
		private:
			friend class red_black_tree;
			node_ptr node;
		};

		class const_iterator
		{
		public:
			typedef std::bidirectional_iterator_tag iterator_category;
			typedef typename red_black_tree::value_type value_type;
			typedef typename red_black_tree::difference_type difference_type;
			typedef typename red_black_tree::reference reference;
			typedef typename red_black_tree::const_reference const_reference;
			typedef typename red_black_tree::pointer pointer;
			typedef typename red_black_tree::const_pointer const_pointer;
		public:
			const_iterator()
				: node(nullptr)
			{
			}
			const_iterator(node_ptr in_node)
				: node(in_node)
			{
			}
			const_iterator(iterator const& it)
				: node(it.node)
			{
			}
			const_iterator(const_iterator const&) = default;
			const_iterator& operator++()
			{
				node = node_t::successor(node);
				return *this;
			}
			const_iterator& operator--()
			{
				node = node_t::predecessor(node);
				return *this;
			}
			const_iterator operator++(int)
			{
				const_iterator save(*this);
				++*this;
				return save;
			}
			const_iterator operator--(int)
			{
				const_iterator save(*this);
				--*this;
				return save;
			}
			const_reference operator *() const
			{
				return node->value;
			}
			const_pointer operator->() const
			{
				return &node->value;
			}
			bool operator == (const_iterator const& other) const
			{
				return node == other.node;
			}
			bool operator != (const_iterator const& other) const
			{
				return node != other.node;
			}
		private:
			friend class red_black_tree;
			node_ptr node;
		};

		/*class reverse_iterator
		{
		public:
			typedef std::bidirectional_iterator_tag iterator_category;
			typedef typename red_black_tree::value_type value_type;
			typedef typename red_black_tree::difference_type difference_type;
			typedef typename red_black_tree::reference reference;
			typedef typename red_black_tree::pointer pointer;
		public:
			reverse_iterator()
				: node(nullptr)
			{
			}
			reverse_iterator(node_ptr in_node)
				: node(in_node)
			{
			}
			explicit reverse_iterator(iterator const& other)
				: node(other.node)
			{
				++*this;
			}
			reverse_iterator(reverse_iterator const&) = default;
			reverse_iterator& operator++()
			{
				node = node_t::predecessor(node);
				return *this;
			}
			reverse_iterator& operator--()
			{
				node = node_t::successor(node);
				return *this;
			}
			reverse_iterator operator++(int)
			{
				reverse_iterator save(*this);
				++*this;
				return save;
			}
			reverse_iterator operator--(int)
			{
				reverse_iterator save(*this);
				--*this;
				return save;
			}
			reference operator *() const
			{
				return node->value;
			}
			pointer operator->() const
			{
				return &node->value;
			}
			bool operator == (reverse_iterator const& other) const
			{
				return node == other.node;
			}
			bool operator != (reverse_iterator const& other) const
			{
				return node != other.node;
			}
			iterator base() const
			{
				return ++iterator(node);
			}
		private:
			friend class red_black_tree;
			node_ptr node;
		};

		class const_reverse_iterator
		{
		public:
			typedef std::bidirectional_iterator_tag iterator_category;
			typedef typename red_black_tree::value_type value_type;
			typedef typename red_black_tree::difference_type difference_type;
			typedef typename red_black_tree::reference reference;
			typedef typename red_black_tree::const_reference const_reference;
			typedef typename red_black_tree::pointer pointer;
			typedef typename red_black_tree::const_pointer const_pointer;
		public:
			const_reverse_iterator()
				: node(nullptr)
			{
			}
			const_reverse_iterator(node_ptr in_node)
				: node(in_node)
			{
			}
			explicit const_reverse_iterator(const_iterator const& other)
				: node(other.node)
			{
				++*this;
			}
			const_reverse_iterator(reverse_iterator const& other)
				: node(other.node)
			{
			}
			const_reverse_iterator(reverse_iterator it)
				: node(it.node)
			{
			}
			const_reverse_iterator(const_reverse_iterator const&) = default;
			const_reverse_iterator& operator++()
			{
				node = node_t::predecessor(node);
				return *this;
			}
			const_reverse_iterator& operator--()
			{
				node = node_t::successor(node);
				return *this;
			}
			const_reverse_iterator operator++(int)
			{
				const_reverse_iterator save(*this);
				++*this;
				return save;
			}
			const_reverse_iterator operator--(int)
			{
				const_reverse_iterator save(*this);
				--*this;
				return save;
			}
			const_reference operator *() const
			{
				return node->value;
			}
			const_pointer operator->() const
			{
				return &node->value;
			}
			bool operator == (const_reverse_iterator const& other) const
			{
				return node == other.node;
			}
			bool operator != (const_reverse_iterator const& other) const
			{
				return node != other.node;
			}
			const_iterator base() const
			{
				return ++iterator(node);
			}
		private:
			friend class red_black_tree;
			node_ptr node;
		};*/
		typedef std::reverse_iterator<iterator>				reverse_iterator;
		typedef std::reverse_iterator<const_iterator>		const_reverse_iterator;
	protected:
		typedef std::pair<iterator, bool>					pair_ib_t;
		typedef std::pair<iterator, iterator>				pair_ii_t;
		typedef std::pair<const_iterator, const_iterator>	pair_cici_t;
		typedef std::pair<node_ptr, bool>					pair_posi_t;
		typedef typename std::conditional<config_t::unique_type::value, pair_ib_t, iterator>::type	insert_result_t;

		template<class unique_type>
		typename std::enable_if<unique_type::value, insert_result_t>::type result_(pair_posi_t posi)
		{
			return std::make_pair(iterator(posi.first), posi.second);
		}

		template<class unique_type>
		typename std::enable_if<!unique_type::value, insert_result_t>::type result_(pair_posi_t posi)
		{
			return iterator(posi.first);
		}

		root_t root_;
	public:
		// empty
		red_black_tree()
			: root_(key_compare(), allocator_type())
		{
			init();
		}

		// empty
		red_black_tree(key_compare const& compare)
			: root_(compare, allocator_type())
		{
			init();
		}

		// empty
		red_black_tree(allocator_type const& alloc)
			: root_(key_compare(), alloc)
		{
			init();
		}

		// empty
		red_black_tree(key_compare const& compare, allocator_type const& alloc)
			: root_(compare, alloc)
		{
			init();
		}

		// range
		template<class iterator_t>
		red_black_tree(iterator_t begin, iterator_t end, key_compare const& compare, allocator_type const& alloc)
			: root_(compare, alloc)
		{
			init();
			insert(begin, end);
		}

		// range
		template<class iterator_t>
		red_black_tree(iterator_t begin, iterator_t end, key_compare const& compare)
			: root_(compare, allocator_type())
		{
			init();
			insert(begin, end);
		}

		// range
		template<class iterator_t>
		red_black_tree(iterator_t begin, iterator_t end)
			: root_(key_compare(), allocator_type())
		{
			init();
			insert(begin, end);
		}

		//initializer list
		red_black_tree(std::initializer_list<value_type> il, key_compare const& compare, allocator_type const& alloc)
			: red_black_tree(il.begin(), il.end(), compare, alloc)
		{
		}

		//initializer list
		red_black_tree(std::initializer_list<value_type> il, key_compare const& compare)
			: red_black_tree(il.begin(), il.end(), compare)
		{
		}

		//initializer list
		red_black_tree(std::initializer_list<value_type> il)
			: red_black_tree(il.begin(), il.end())
		{
		}

		// copy
		red_black_tree(red_black_tree const& other)
			: root_(other.get_key_comp(), other.get_node_allocator())
		{
			init();
			copy_all<false>(head(),other.root());
			left_most() = node_t::minimum(root());
			right_most() = node_t::maximum(root());
			root_.size = other.size();
		}

		// copy
		red_black_tree(red_black_tree const& other,allocator_type const& alloc)
			: root_(other.get_key_comp(), alloc)
		{
			init();
			copy_all<false>(head(), other.root());
			left_most() = node_t::minimum(root());
			right_most() = node_t::maximum(root());
			root_.size = other.size();
		}

		// move
		red_black_tree(red_black_tree&& other)
			: root_(key_compare(), node_allocator_t())
		{
			init();
			swap(other);
		}

		// move
		red_black_tree(red_black_tree&& other, allocator_type const& alloc)
			: root_(std::move(other.get_key_comp()), alloc)
		{
			init();
			copy_all<true>(head(), other.root());
			left_most() = node_t::minimum(root());
			right_most() = node_t::maximum(root());
			root_.size = other.size();
		}

		// destructor
		~red_black_tree()
		{
			dealloc_all();
		}

		//copy
		red_black_tree& operator = (red_black_tree const& other)
		{
			if (this == &other)
			{
				return *this;
			}
			clear();
			get_key_comp() = other.get_key_comp();
			get_node_allocator() = other.get_node_allocator();
			copy_all<false>(head(), other.root());
			left_most() = node_t::minimum(root());
			right_most() = node_t::maximum(root());
			root_.size = other.size();
			return *this;
		}

		//move
		red_black_tree& operator = (red_black_tree&& other)
		{
			if (this == &other)
			{
				return *this;
			}
			swap(other);
			other.clear();
			return *this;
		}

		//initializer list
		red_black_tree& operator = (std::initializer_list<value_type> il)
		{
			clear();
			insert(il.begin(), il.end());
			return *this;
		}

		void swap(red_black_tree& other)
		{
			std::swap(root_, other.root_);
		}

		size_type size() const
		{
			return root_.size;
		}

		size_type max_size() const
		{
			return get_node_allocator().max_size();
		}

		bool empty() const
		{
			return root_.size == 0;
		}

		iterator begin()
		{
			return iterator(left_most());
		}

		const_iterator begin() const
		{
			return const_iterator(left_most());
		}

		iterator end()
		{
			return iterator(root_.nil);
		}

		const_iterator end() const
		{
			return const_iterator(root_.nil);
		}

		reverse_iterator rbegin()
		{
			//return reverse_iterator(right_most());
			return reverse_iterator(end());
		}

		const_reverse_iterator rbegin() const
		{
			//return const_reverse_iterator(right_most());
			return const_reverse_iterator(end());
		}
		
		reverse_iterator rend()
		{
			//return reverse_iterator(root_.nil);
			return reverse_iterator(begin());
		}

		const_reverse_iterator rend() const
		{
			//return const_reverse_iterator(root_.nil);
			return const_reverse_iterator(begin());
		}

		const_iterator cbegin() const
		{
			//return const_iterator(left_most());
			return begin();
		}

		const_iterator cend() const
		{
			//return const_iterator(root_.nil);
			return end();
		}

		const_reverse_iterator crbegin() const
		{
			//return const_reverse_iterator(right_most());
			return rbegin();
		}

		const_reverse_iterator crend() const
		{
			//return const_reverse_iterator(root_.nil);
			return rend();
		}

		// single element
		insert_result_t insert(value_type const& value)
		{
			return result_<typename config_t::unique_type>(insert_nohint(typename config_t::unique_type(), false, value));
		}

		// single element
		insert_result_t insert(value_type&& value)
		{
			return result_<typename config_t::unique_type>(insert_nohint(typename config_t::unique_type(), false, std::forward<value_type>(value)));
		}

		// single element
		template<class in_value_t>
		typename std::enable_if<std::is_convertible<in_value_t, value_type>::value, insert_result_t>::type insert(in_value_t&& value)
		{
			return result_<typename config_t::unique_type>(insert_nohint(typename config_t::unique_type(), false, std::forward<in_value_t>(value)));
		}

		// with hint
		insert_result_t insert(const_iterator hint, value_type const& value)
		{
			return result_<typename config_t::unique_type>(insert_hint(typename config_t::unique_type(), hint, value));
		}

		// with hint
		insert_result_t insert(const_iterator hint, value_type&& value)
		{
			return result_<typename config_t::unique_type>(insert_hint(typename config_t::unique_type(), hint, std::forward<value_type>(value)));
		}

		// with hint
		template<class in_value_t>
		typename std::enable_if<std::is_convertible<in_value_t, value_type>::value, insert_result_t>::type insert(const_iterator hint, in_value_t&& value)
		{
			return result_<typename config_t::unique_type>(insert_hint(typename config_t::unique_type(), hint, std::forward<in_value_t>(value)));
		}

		// range
		template<class iterator_t>
		void insert(iterator_t begin, iterator_t end)
		{
			for (; begin != end; ++begin)
			{
				emplace_hint(cend(), *begin);
			}
		}

		// initializer list
		void insert(std::initializer_list<value_type> il)
		{
			insert(il.begin(), il.end());
		}

		// single element
		template<class ...args_t>
		insert_result_t emplace(args_t&&... args)
		{
			return result_<typename config_t::unique_type>(insert_nohint(typename config_t::unique_type(), false, std::move(value_type(std::forward<args_t>(args)...))));
		}

		// with hint
		template<class ...args_t>
		insert_result_t emplace_hint(const_iterator hint, args_t&&... args)
		{
			return result_<typename config_t::unique_type>(insert_hint(typename config_t::unique_type(), hint, std::move(value_type(std::forward<args_t>(args)...))));
		}

		iterator find(key_type const& key)
		{
			node_ptr node = lower_bound_(key);
			return (node_t::is_nil(node) || get_key_comp()(key, get_key_t()(node->value))) ? end() : iterator(node);
		}

		const_iterator find(key_type const& key) const
		{
			node_ptr node = lower_bound_(key);
			return (node_t::is_nil(node) || get_key_comp()(key, get_key_t()(node->value))) ? end() : const_iterator(node);
		}

		iterator lower_bound(key_type const& key)
		{
			return iterator(lower_bound_(key));
		}

		const_iterator lower_bound(key_type const& key) const
		{
			return const_iterator(lower_bound_(key));
		}

		iterator upper_bound(key_type const& key)
		{
			return iterator(upper_bound_(key));
		}

		const_iterator upper_bound(key_type const& key) const
		{
			return const_iterator(upper_bound_(key));
		}

		pair_ii_t equal_range(key_type const& key)
		{
			return pair_ii_t(iterator(lower_bound_(key)), iterator(upper_bound_(key)));
		}

		pair_cici_t equal_range(key_type const& key) const
		{
			return pair_cici_t(const_iterator(lower_bound_(key)), const_iterator(upper_bound_(key)));
		}

		size_type count(key_type const& key) const
		{
			if (config_t::unique_type::value)
			{
				return find(key) == end() ? 0 : 1;
			}
			else
			{
				pair_cici_t range = equal_range(key);
				return std::distance(range.first, range.second);
			}
		}

		template<class in_key_t, class = typename std::enable_if<std::is_convertible<in_key_t, key_type>::value && config_t::unique_type::value && !std::is_same<key_type, value_type>::value, void>::type>
		mapped_type& at(in_key_t const& key)
		{
			node_ptr node = lower_bound_(key);
			if (node_t::is_nil(node) || get_key_comp()(key, get_key_t()(node->value)))
			{
				throw std::out_of_range("red_black_tree out of range");
			}
			return node->value.second;
		}

		template<class in_key_t, class = typename std::enable_if<std::is_convertible<in_key_t, key_type>::value && config_t::unique_type::value && !std::is_same<key_type, value_type>::value, void>::type>
		mapped_type const& at(in_key_t const& key) const
		{
			node_ptr node = lower_bound_(key);
			if (node_t::is_nil(node) || get_key_comp()(key, get_key_t()(node->value)))
			{
				throw std::out_of_range("red_black_tree out of range");
			}
			return node->value.second;
		}

		template<class in_key_t, class = typename std::enable_if<std::is_convertible<in_key_t, key_type>::value && config_t::unique_type::value && !std::is_same<key_type, value_type>::value, void>::type>
		mapped_type& operator[](in_key_t&& key)
		{
			iterator it = lower_bound(key);
			if (it == end() || get_key_comp()(key, get_key_t()(*it)))
			{
				it = emplace_hint(it, key, mapped_type()).first;
			}
			return it->second;
		}

		iterator erase(const_iterator it)
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

			node_ptr del = it.node;
			if (node_t::is_nil(del))
			{
				throw std::out_of_range("rbtree map/set erase iterator outside range");
			}
			node_ptr nextNode = node_t::successor(del);
			if (!node_t::is_nil(del->left) && !node_t::is_nil(del->right))
			{
				node_ptr next = node_t::minimum(del->right);      //找后继节点
				Ajy::Swap(del->color, next->color);
				node_ptr delLeft = del->left;
				node_ptr delRight = del->right;
				node_ptr delParent = del->parent;
				node_ptr nextLeft = next->left;
				node_ptr nextRight = next->right;
				node_ptr nextParent = next->parent;
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
				if (!node_t::is_nil(nextRight))
				{
					nextRight->parent = del;
				}
				else
				{
					if (next == right_most())
						root_.nil->right = del;
				}
				if (!node_t::is_nil(nextLeft))
					nextLeft->parent = del;
				if (node_t::is_nil(delParent))
					root_.root = next;
				else if (isLeftChild)
					delParent->left = next;
				else
					delParent->right = next;
			}
			if (!node_t::is_nil(del->left))
			{
				//交换自己和左孩子
				del->left->color = RBTree_BLACK;			//设置孩子节点的颜色与del节点一致,根据结论1可得del节点的颜色为黑色
				if (del == root_.root)
					root_.root = del->left;
				else
				{
					if (del == del->parent->left)
						del->parent->left = del->left;
					else
						del->parent->right = del->left;
				}
				del->left->parent = del->parent;
				if (del == right_most())
					root_.nil->right = del->left;					//修改right nost(nil->right)
			}
			else if (!node_t::is_nil(del->right))
			{
				//交换自己和右孩子
				del->right->color = RBTree_BLACK;			//设置孩子节点的颜色与del节点一致,根据结论1可得del节点的颜色为黑色
				if (del == root_.root)
					root_.root = del->right;
				else
				{
					if (del == del->parent->left)
						del->parent->left = del->right;
					else
						del->parent->right = del->right;
				}
				del->right->parent = del->parent;
				if (del == left_most())
					root_.nil->left = del->right;					//修改left most(nil->left)
			}
			else
			{
				//左右子树都不存在，需要进入删除调整算法  
				erase_rebalance(del);
				if (del == root_.root)
				{
					root_.root = root_.nil;
					root_.nil->left = root_.root;
					root_.nil->right = root_.root;
				}
				else if (del == del->parent->left)
				{
					del->parent->left = root_.nil;				//父节点的指针域需要修改
					if (del == left_most())
						root_.nil->left = del->parent;			//修改left most(nil->left)
				}
				else if (del == del->parent->right)
				{
					del->parent->right = root_.nil;				//父节点的指针域需要修改  
					if (del == right_most())
						root_.nil->right = del->parent;			//修改right most(nil->right)
				}
			}
			destroy_node(del);
			--root_.size;
			return iterator(nextNode);
		}

		size_type erase(key_type const& key)
		{
			pair_ii_t where = equal_range(key);
			size_type num = std::distance(where.first, where.second);
			erase(where.first, where.second);
			return num;
		}

		iterator erase(const_iterator erase_begin, const_iterator erase_end)
		{
			if (erase_begin == cbegin() && erase_end == cend())
			{
				clear();
				return begin();
			}
			else
			{
				while (erase_begin != erase_end)
					erase(erase_begin++);
				return iterator(erase_begin.node);
			}
		}

		void clear()
		{
			erase(root());
			root() = head();
			left_most() = head();
			right_most() = head();
		}

	protected:

		node_allocator_t& get_node_allocator()
		{
			return root_;
		}

		node_allocator_t const& get_node_allocator() const
		{
			return root_;
		}

		key_compare& get_key_comp()
		{
			return root_;
		}

		key_compare const& get_key_comp() const
		{
			return root_;
		}

		node_ptr lower_bound_(key_type const& key) const
		{
			auto getkey = get_key_t();
			auto getkeycomp = get_key_comp();
			node_ptr psearch = root_.nil;
			node_ptr search = root_.root;
			while (!node_t::is_nil(search))
			{
				if (!getkeycomp(getkey(search->value), key))
				{
					psearch = search;
					search = search->left;
				}
				else
					search = search->right;
			}
			return psearch;
		}

		node_ptr upper_bound_(key_type const& key) const
		{
			auto getkey = get_key_t();
			auto getkeycomp = get_key_comp();
			node_ptr psearch = root_.nil;
			node_ptr search = root_.root;
			while (!node_t::is_nil(search))
			{
				if (getkeycomp(key, getkey(search->value)))
				{
					psearch = search;
					search = search->left;
				}
				else
					search = search->right;
			}
			return psearch;
		}

		node_ptr head() const
		{
			return root_.nil;
		}

		node_ptr& root() const
		{
			return static_cast<node_ptr>(root_.root);
		}

		node_ptr& left_most() const
		{
			return root_.nil->left;
		}

		node_ptr& right_most() const
		{
			return root_.nil->right;
		}

		template<class in_value_t>
		node_ptr create_node(color_t color, node_ptr parent, in_value_t&& value)
		{
			node_ptr node = get_node_allocator().allocate(1);
			new (&node->value) value_type(std::forward<in_value_t>(value));
			node->color = color;
			node->parent = parent;
			node->left = root_.nil;
			node->right = root_.nil;
			return node;
		}

		void destroy_node(node_ptr& dispose)
		{
			get_node_allocator().deallocate(dispose,1);
			dispose = nullptr;
		}

		void init()
		{
			root_.nil = get_node_allocator().allocate(1);
			//new (&root_.nil->value) value_type();
			root_.nil->left = root_.nil;
			root_.nil->right = root_.nil;
			root_.nil->parent = nullptr;
			root_.nil->color = red_black_tree_detail::color_black;
			root_.root = root_.nil;
			root_.size = 0;
		}

		template<bool is_move>
		void copy_all(node_ptr dstParent, node_ptr src)
		{
			if (node_t::is_nil(src)) return;
			node_ptr dst;
			if (is_move)
			{
				dst = create_node(src->color, dstParent, std::move(src->value));
			}
			else
			{
				dst = create_node(src->color, dstParent, src->value);
			}
			dst->left = head();
			dst->right = head();
			if (node_t::is_nil(src->parent))           //根节点
			{
				root() = dst;
			}
			else if (src == src->parent->left)       //左孩子
			{
				dstParent->left = dst;
			}
			else									//右孩子
			{
				dstParent->right = dst;
			}
			copy_all<is_move>(dst, src->left);
			copy_all<is_move>(dst, src->right);
		}

		void dealloc_all()
		{
			clear();
			destroy_node(root_.nil);
			//root_.root = nullptr;
		}

		void left_rotate(node_ptr rotate_node)
		{
			node_ptr tmp = rotate_node->right;
			rotate_node->right = tmp->left;
			if (!node_t::is_nil(tmp->left))
				tmp->left->parent = rotate_node;
			tmp->parent = rotate_node->parent;

			if (root_.root == rotate_node)
				root() = tmp;
			else if (rotate_node == rotate_node->parent->left)
				rotate_node->parent->left = tmp;
			else
				rotate_node->parent->right = tmp;
			tmp->left = rotate_node;
			rotate_node->parent = tmp;
		}

		void right_rotate(node_ptr rotate_node)
		{
			node_ptr tmp = rotate_node->left;
			rotate_node->left = tmp->right;
			if (!node_t::is_nil(tmp->right))
				tmp->right->parent = rotate_node;
			tmp->parent = rotate_node->parent;

			if (root_.root == rotate_node)
				root() = tmp;
			else if (rotate_node == rotate_node->parent->left)
				rotate_node->parent->left = tmp;
			else
				rotate_node->parent->right = tmp;
			tmp->right = rotate_node;
			rotate_node->parent = tmp;
		}

		void insert_rebalance(node_ptr fixup_node)
		{
			while (node_t::is_red(fixup_node->parent))
			{
				if (fixup_node->parent == fixup_node->parent->parent->left)   //  
				{
					node_ptr uncle = fixup_node->parent->parent->right;
					if (node_t::is_red(uncle))   //插入情况1，fixupNode的叔叔uncle是红色的。  
					{
						fixup_node->parent->color = red_black_tree_detail::color_black;
						uncle->color = red_black_tree_detail::color_black;
						uncle->parent->color = red_black_tree_detail::color_red;
						fixup_node = fixup_node->parent->parent;
					}
					else
					{
						if (fixup_node == fixup_node->parent->right) //插入情况2：fixupNode的叔叔uncle是黑色的，且fixupNode是右孩子  
						{
							fixup_node = fixup_node->parent;
							left_rotate(fixup_node);
						}
						//插入情况3，情况2可以转为情况3
						fixup_node->parent->color = red_black_tree_detail::color_black;
						fixup_node->parent->parent->color = red_black_tree_detail::color_red;
						right_rotate(fixup_node->parent->parent);
					}
				}
				else
				{
					node_ptr uncle = fixup_node->parent->parent->left;
					if (node_t::is_red(uncle))
					{
						fixup_node->parent->color = red_black_tree_detail::color_black;
						uncle->color = red_black_tree_detail::color_black;
						uncle->parent->color = red_black_tree_detail::color_red;
						fixup_node = fixup_node->parent->parent;
					}
					else
					{
						if (fixup_node == fixup_node->parent->left)
						{
							fixup_node = fixup_node->parent;
							right_rotate(fixup_node);     //与上述代码相比，左旋改为右旋  
						}
						fixup_node->parent->color = red_black_tree_detail::color_black;
						fixup_node->parent->parent->color = red_black_tree_detail::color_red;
						left_rotate(fixup_node->parent->parent);   //右旋改为左旋，即可。  
					}
				}
			}
			root()->color = red_black_tree_detail::color_black;
		}

		void erase_rebalance(node_ptr fixup_node)
		{
			while (fixup_node != root() && node_t::is_black(fixup_node))
			{
				if (fixup_node == fixup_node->parent->left)
				{
					node_ptr brother = fixup_node->parent->right;
					if (node_t::is_red(brother))         //情况1：fixupNode的兄弟brother是红色的。 
					{
						brother->color = red_black_tree_detail::color_black;
						fixup_node->parent->color = red_black_tree_detail::color_red;
						left_rotate(fixup_node->parent);
						brother = fixup_node->parent->right;
					}
					if (node_t::is_black(brother->left) && node_t::is_black(brother->right))        //情况2:fixupNode的兄弟brother的两个孩子都是黑色的
					{
						brother->color = red_black_tree_detail::color_red;
						fixup_node = fixup_node->parent;
					}
					else
					{
						if (node_t::is_black(brother->right))         //情况3：fixupNode的兄弟brother的左孩子是红色，右孩子是黑色
						{
							brother->left->color = red_black_tree_detail::color_black;
							brother->color = red_black_tree_detail::color_red;
							right_rotate(brother);
							brother = fixup_node->parent->right;
						}
						//情况4:brother的孩子都是红色
						brother->color = fixup_node->parent->color;
						fixup_node->parent->color = red_black_tree_detail::color_black;
						brother->right->color = red_black_tree_detail::color_black;
						left_rotate(fixup_node->parent);
						fixup_node = root();
					}
				}
				else
				{
					node_ptr brother = fixup_node->parent->left;
					if (node_t::is_red(brother))         //情况1：fixupNode的兄弟brother是红色的。 
					{
						brother->color = red_black_tree_detail::color_black;
						fixup_node->parent->color = red_black_tree_detail::color_red;
						right_rotate(fixup_node->parent);
						brother = fixup_node->parent->left;
					}
					if (node_t::is_black(brother->right) && node_t::is_black(brother->left))        //情况2:fixupNode的兄弟brother的两个孩子都是黑色的
					{
						brother->color = red_black_tree_detail::color_red;
						fixup_node = fixup_node->parent;
					}
					else
					{
						if (node_t::is_black(brother->left))         //情况3：fixupNode的兄弟brother的左孩子是黑色，右孩子是红色
						{
							brother->right->color = red_black_tree_detail::color_black;
							brother->color = red_black_tree_detail::color_red;
							left_rotate(brother);
							brother = fixup_node->parent->left;
						}
						//情况4:brother的孩子都是红色
						brother->color = fixup_node->parent->color;
						fixup_node->parent->color = red_black_tree_detail::color_black;
						brother->left->color = red_black_tree_detail::color_black;
						right_rotate(fixup_node->parent);
						fixup_node = root();
					}
				}
			}
			fixup_node->color = red_black_tree_detail::color_black;
		}

		// set/map
		template<class in_value_t>
		pair_posi_t insert_nohint(std::true_type, bool is_leftish, in_value_t&& value)
		{
			auto getkey = get_key_t();
			auto getkeycomp = get_key_comp();
			node_ptr psearch = root_.nil;
			node_ptr search = root_.root;
			bool addLeft = true;
			key_type key = getkey(value);
			if (is_leftish)
			{
				while (!node_t::is_nil(search))
				{
					psearch = search;
					addLeft = !getkeycomp(getkey(search->value), key);
					search = addLeft ? search->left : search->right;
				}
			}
			else
			{
				while (!node_t::is_nil(search))
				{
					psearch = search;
					addLeft = getkeycomp(key, getkey(search->value));
					search = addLeft ? search->left : search->right;
				}
			}
			// 离开while循环后，psearch为插入节点之父节点
			iterator iter = iterator(psearch);
			//-------------判断树中是否已存在要插入数据的key值-------begin
			if (addLeft)
			{
				if (iter == begin())
					return pair_posi_t(insert_at(true, psearch, std::forward<in_value_t>(value)), true);
				else
					--iter;
			}
			if (getkeycomp(getkey(*iter), key))
				//-------------判断树中是否已存在要插入数据的key值-------end
				return pair_posi_t(insert_at(addLeft, psearch, std::forward<in_value_t>(value)), true);
			return pair_posi_t(iter.node, false);
		}

		// multiset/multimap
		template<class in_value_t>
		pair_posi_t insert_nohint(std::false_type, bool is_leftish, in_value_t&& value)
		{
			auto getkey = get_key_t();
			auto getkeycomp = get_key_comp();
			node_ptr psearch = root_.nil;
			node_ptr search = root_.root;
			key_type key = getkey(value);
			bool addLeft = true;
			if (is_leftish)
			{
				while (!node_t::is_nil(search))
				{
					psearch = search;
					addLeft = !getkeycomp(getkey(search->value), key);
					search = addLeft ? search->left : search->right;
				}
			}
			else
			{
				while (!node_t::is_nil(search))
				{
					psearch = search;
					addLeft = getkeycomp(key, getkey(search->value));
					search = addLeft ? search->left : search->right;
				}
			}
			return pair_posi_t(insert_at(addLeft, psearch, std::forward<in_value_t>(value)), true);
		}
		
		// set/map
		template<class in_value_t>
		pair_posi_t insert_hint(std::true_type, const_iterator where, in_value_t&& value)
		{
			if (size() == 0)
			{
				return pair_posi_t(insert_at(true, head(), std::forward<in_value_t>(value)), true);
			}
			auto getkey = get_key_t();
			auto getkeycomp = get_key_comp();
			key_type k = getkey(value);
			const_iterator next;
			if (where == begin())
			{
				if (getkeycomp(getkey(value),getkey(*where)))
				{
					return pair_posi_t(insert_at(true, where.node, std::forward<in_value_t>(value)), true);
				}
			}
			else if (where == end())
			{
				if (getkeycomp(getkey(right_most()->value), getkey(value)))
				{
					return pair_posi_t(insert_at(false, right_most(), std::forward<in_value_t>(value)), true);
				}
			}
			else if (getkeycomp(getkey(value), getkey(*where)) && getkeycomp(getkey(*(--(next = where))), getkey(value)))
			{
				if (node_t::is_nil(next.node->right))
				{
					return pair_posi_t(insert_at(false, next.node, std::forward<in_value_t>(value)), true);
				}
				else
				{
					return pair_posi_t(insert_at(true, where.node, std::forward<in_value_t>(value)), true);
				}
			}
			else if (getkeycomp(getkey(*where), getkey(value)) && (++(next = where) == end() || getkeycomp(getkey(value), getkey(*next))))
			{
				if (node_t::is_nil(where.node->right))
				{
					return pair_posi_t(insert_at(false, where.node, std::forward<in_value_t>(value)), true);
				}
				else
				{
					return pair_posi_t(insert_at(true, next.node, std::forward<in_value_t>(value)), true);
				}
			}
			return insert_nohint(typename config_t::unique_type(), false, std::forward<in_value_t>(value));
		}

		// multiset/multimap
		template<class in_value_t>
		pair_posi_t insert_hint(std::false_type, const_iterator where, in_value_t&& value)
		{
			if (size() == 0)
			{
				return pair_posi_t(insert_at(true, head(), std::forward<in_value_t>(value)), true);
			}
			auto getkey = get_key_t();
			auto getkeycomp = get_key_comp();
			key_type k = getkey(value);
			const_iterator next;
			bool is_leftish = false;
			if (where == begin())
			{
				if (!getkeycomp(getkey(*where), getkey(value)))
				{
					return pair_posi_t(insert_at(true, where.node, std::forward<in_value_t>(value)), true);
				}
				is_leftish = true;
			}
			else if (where == end())
			{
				if (!getkeycomp(getkey(value), getkey(right_most()->value)))
				{
					return pair_posi_t(insert_at(false, right_most(), std::forward<in_value_t>(value)), true);
				}
			}
			else if (!getkeycomp(getkey(*where), getkey(value)) && !getkeycomp(getkey(value), getkey(*(--(next = where)))))
			{
				if (node_t::is_nil(next.node->right))
				{
					return pair_posi_t(insert_at(false, next.node, std::forward<in_value_t>(value)), true);
				}
				else
				{
					return pair_posi_t(insert_at(true, where.node, std::forward<in_value_t>(value)), true);
				}
			}
			else if (!getkeycomp(getkey(value), getkey(*where)) && (++(next = where) == end() || !getkeycomp(getkey(*next), getkey(value))))
			{
				if (node_t::is_nil(where.node->right))
				{
					return pair_posi_t(insert_at(false, where.node, std::forward<in_value_t>(value)), true);
				}
				else
				{
					return pair_posi_t(insert_at(true, next.node, std::forward<in_value_t>(value)), true);
				}
			}
			else
			{
				is_leftish = true;
			}
			return insert_nohint(typename config_t::unique_type(), is_leftish, std::forward<in_value_t>(value));
		}

		template<class in_value_t>
		node_ptr insert_at(bool addLeft, node_ptr insert_parent, in_value_t&& value)
		{
			if (root_.size >= max_size() - 1)
			{
				std::length_error("map/set too long");
			}
			node_ptr insert_node = create_node(red_black_tree_detail::color_red, insert_parent, std::forward<in_value_t>(value));
			if (node_t::is_nil(insert_parent)) // 如果插入的是一颗空树  
			{
				root() = insert_node;
				left_most() = insert_node;
				right_most() = insert_node;
			}
			else
			{
				if (addLeft)
				{
					insert_parent->left = insert_node;
					if (insert_parent == left_most())
						left_most() = insert_node;
				}
				else
				{
					insert_parent->right = insert_node;
					if (insert_parent == right_most())
						right_most() = insert_node;
				}
			}
			insert_rebalance(insert_node);
			++root_.size;
			return insert_node;
		}

		void erase(node_ptr erase_node)
		{
			node_ptr tmp;
			while (erase_node && !node_t::is_nil(erase_node))
			{
				erase(erase_node->right);
				tmp = erase_node->left;
				destroy_node(erase_node);
				--root_.size;
				erase_node = tmp;
			}
			/*if (!erase_node || node_t::is_nil(erase_node))
				return;
			else
			{
				erase(erase_node->left);
				erase(erase_node->right);
				destroy_node(erase_node);
				--root_.size;
			}*/
		}
	};

}
