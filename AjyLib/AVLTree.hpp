#pragma once

namespace Ajy
{
	// 标准库风格，用法和标准库map/multimap/set/multiset一样
	// MSVC下natvis友好调试视图支持

	template<class value_t>
	struct avltree_node
	{
		typedef value_t			value_type;
		typedef avltree_node*	link_type;
		signed char balance_factor : 2;
		link_type parent;
		link_type left;
		link_type right;
		value_type value;
		static link_type successor(link_type node)
		{
			if (is_nil(node)) return node->left;      //nil节点的后继设计为树的最左节点,方便反向利用reverse_iterator遍历avl树
			if (!is_nil(node->right)) return minimum(node->right);
			link_type tmp = node->parent;
			while (!is_nil(tmp) && node == tmp->right)
			{
				node = tmp;
				tmp = tmp->parent;
			}
			return tmp;
		}
		static link_type predecessor(link_type node)
		{
			if (is_nil(node)) return node->right;      //nil节点的前驱设计为树的最右节点,方便反向利用iterator遍历avl树
			if (!is_nil(node->left)) return maximum(node->left);
			link_type tmp = node->parent;
			while (!is_nil(tmp) && node == tmp->left)
			{
				node = tmp;
				tmp = tmp->parent;
			}
			return tmp;
		}
		static link_type minimum(link_type node)
		{
			while (!is_nil(node->left))
				node = node->left;
			return node;
		}
		static link_type maximum(link_type node)
		{
			while (!is_nil(node->right))
				node = node->right;
			return node;
		}
		static bool is_nil(link_type node)
		{
			return node->balance_factor == -2;
		}
	};

	template<class value_t>
	struct avltree_iterator
	{
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef std::ptrdiff_t					difference_type;
		typedef value_t							value_type;
		typedef value_t&						reference;
		typedef value_t*						pointer;
		typedef avltree_node<value_type>		node_type;
		typedef avltree_node<value_type>*		link_type;

		avltree_iterator()
			: node(nullptr)
		{
		}
		explicit avltree_iterator(link_type in_node)
			: node(in_node)
		{
		}
		avltree_iterator(avltree_iterator const&) = default;
		avltree_iterator& operator++()
		{
			node = node_type::successor(node);
			return *this;
		}
		avltree_iterator& operator--()
		{
			node = node_type::predecessor(node);
			return *this;
		}
		avltree_iterator operator++(int)
		{
			avltree_iterator save(*this);
			++*this;
			return save;
		}
		avltree_iterator operator--(int)
		{
			avltree_iterator save(*this);
			--*this;
			return save;
		}
		reference operator *() const
		{
			return node->value;
		}
		pointer operator->() const
		{
			return &(node->value);
		}
		bool operator == (avltree_iterator const& other) const
		{
			return node == other.node;
		}
		bool operator != (avltree_iterator const& other) const
		{
			return node != other.node;
		}

		link_type node;
	};

	template<class value_t>
	struct avltree_const_iterator
	{
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef std::ptrdiff_t					difference_type;
		typedef value_t							value_type;
		typedef value_t const&					reference;
		typedef value_t const*					pointer;
		typedef avltree_node<value_type>		node_type;
		typedef avltree_node<value_type>*		link_type;

		avltree_const_iterator()
			: node(nullptr)
		{
		}
		explicit avltree_const_iterator(link_type in_node)
			: node(in_node)
		{
		}
		avltree_const_iterator(avltree_iterator<value_t> const& it)
			: node(it.node)
		{
		}
		avltree_const_iterator(avltree_const_iterator const&) = default;
		avltree_const_iterator& operator++()
		{
			node = node_type::successor(node);
			return *this;
		}
		avltree_const_iterator& operator--()
		{
			node = node_type::predecessor(node);
			return *this;
		}
		avltree_const_iterator operator++(int)
		{
			avltree_const_iterator save(*this);
			++*this;
			return save;
		}
		avltree_const_iterator operator--(int)
		{
			avltree_const_iterator save(*this);
			--*this;
			return save;
		}
		reference operator *() const
		{
			return node->value;
		}
		pointer operator->() const
		{
			return &(node->value);
		}
		bool operator == (avltree_const_iterator const& other) const
		{
			return node == other.node;
		}
		bool operator != (avltree_const_iterator const& other) const
		{
			return node != other.node;
		}

		link_type node;
	};

	template<class value_t>
	inline bool operator==(avltree_iterator<value_t> const& left, avltree_const_iterator<value_t> const& right)
	{
		return left.node == right.node;
	}

	template<class value_t>
	inline bool operator!=(avltree_iterator<value_t> const& left, avltree_const_iterator<value_t> const& right)
	{
		return left.node != right.node;
	}

	template<class params_t> class avl_tree;


	template<class key_t, class comparator_t, class allocator_t, class unique_t>
	struct avltree_set_params_t
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
	using avltree_set = avl_tree<avltree_set_params_t<value_t, comparator_t, allocator_t, std::true_type>>;
	template<class value_t, class comparator_t = std::less<value_t>, class allocator_t = std::allocator<value_t>>
	using avltree_multiset = avl_tree<avltree_set_params_t<value_t, comparator_t, allocator_t, std::false_type>>;



	template<class key_t, class value_t, class comparator_t, class allocator_t, class unique_t>
	struct avltree_map_params_t
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
	using avltree_map = avl_tree<avltree_map_params_t<key_t, value_t, comparator_t, allocator_t, std::true_type>>;
	template<class key_t, class value_t, class comparator_t = std::less<key_t>, class allocator_t = std::allocator<std::pair<key_t const, value_t>>>
	using avltree_multimap = avl_tree<avltree_map_params_t<key_t, value_t, comparator_t, allocator_t, std::false_type>>;



	template<class params_t>
	class avl_tree
	{
	public:
		typedef typename params_t::key_type				key_type;
		typedef typename params_t::mapped_type			mapped_type;
		typedef typename params_t::value_type			value_type;
		typedef std::size_t								size_type;
		typedef std::ptrdiff_t							difference_type;
		typedef typename params_t::key_compare			key_compare;
		typedef typename params_t::allocator_type		allocator_type;
		typedef value_type&								reference;
		typedef value_type const&						const_reference;
		typedef value_type*								pointer;
		typedef value_type const*						const_pointer;
		typedef typename std::conditional<std::is_same<key_type, value_type>::value, avltree_const_iterator<value_type>, avltree_iterator<value_type>>::type iterator;
		typedef avltree_const_iterator<value_type>		const_iterator;
		typedef std::reverse_iterator<iterator>			reverse_iterator;
		typedef std::reverse_iterator<const_iterator>	const_reverse_iterator;
	protected:
		typedef typename allocator_type::template rebind<avltree_node<value_type>>::other node_allocator_t;
		typedef avltree_node<value_type>				node_type;
		typedef avltree_node<value_type>*				link_type;
		struct impl_t : public key_compare, public node_allocator_t
		{
			template<class any_key_compare, class any_allocator_t>
			impl_t(any_key_compare&& comp, any_allocator_t&& alloc)
				: key_compare(std::forward<any_key_compare>(comp))
				, node_allocator_t(std::forward<any_allocator_t>(alloc))
			{
			}
			size_type size;
			link_type header;
		};

		template<class k_t, class v_t>
		struct get_key_select_t
		{
			key_type const& operator()(value_type const& value)
			{
				return params_t::get_key(value);
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
				return params_t::get_key(value);
			}

			template<class in_t, class ...args_t>
			key_type operator()(in_t const& in, args_t const&... args)
			{
				return key_type(in, args...);
			}
		};
		typedef get_key_select_t<key_type, value_type> get_key_t;

	public:
		typedef std::pair<iterator, bool> pair_ib_t;
		typedef std::pair<iterator, iterator> pair_ii_t;
		typedef std::pair<const_iterator, const_iterator> pair_cici_t;
		typedef typename std::conditional<params_t::unique_type::value, pair_ib_t, iterator>::type insert_result_t;
	protected:
		typedef std::pair<link_type, bool> pair_posi_t;

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

		impl_t _impl;
	public:
		// empty
		avl_tree();
		// empty
		avl_tree(key_compare const& compare);
		// empty
		avl_tree(allocator_type const& alloc);
		// empty
		avl_tree(key_compare const& compare, allocator_type const& alloc);
		// range
		template<class iterator_t>
		avl_tree(iterator_t begin, iterator_t end, key_compare const& compare, allocator_type const& alloc);
		// range
		template<class iterator_t>
		avl_tree(iterator_t begin, iterator_t end, key_compare const& compare);
		// range
		template<class iterator_t>
		avl_tree(iterator_t begin, iterator_t end);
		//initializer list
		avl_tree(std::initializer_list<value_type> il, key_compare const& compare, allocator_type const& alloc);
		//initializer list
		avl_tree(std::initializer_list<value_type> il, key_compare const& compare);
		//initializer list
		avl_tree(std::initializer_list<value_type> il);
		// copy
		avl_tree(avl_tree const& other);
		// copy
		avl_tree(avl_tree const& other, allocator_type const& alloc);
		// move
		avl_tree(avl_tree&& other);
		// move
		avl_tree(avl_tree&& other, allocator_type const& alloc);
		// destructor
		~avl_tree();
		//copy
		avl_tree& operator = (avl_tree const& other);
		//move
		avl_tree& operator = (avl_tree&& other);
		//initializer list
		avl_tree& operator = (std::initializer_list<value_type> il);
		void						swap(avl_tree& other);
		size_type					size() const;
		size_type					max_size() const;
		bool						empty() const;
		iterator					begin();
		const_iterator				begin() const;
		iterator					end();
		const_iterator				end() const;
		reverse_iterator			rbegin();
		const_reverse_iterator		rbegin() const;
		reverse_iterator			rend();
		const_reverse_iterator		rend() const;
		const_iterator				cbegin() const;
		const_iterator				cend() const;
		const_reverse_iterator		crbegin() const;
		const_reverse_iterator		crend() const;
		insert_result_t				insert(value_type const& value);
		insert_result_t				insert(value_type&& value);
		template<class in_value_t>
		typename std::enable_if<std::is_convertible<in_value_t, value_type>::value, insert_result_t>::type insert(in_value_t&& value)
		{
			return result_<typename params_t::unique_type>(insert_nohint(typename params_t::unique_type(), false, std::forward<in_value_t>(value)));
		}
		insert_result_t				insert(const_iterator hint, value_type const& value);
		insert_result_t				insert(const_iterator hint, value_type&& value);
		template<class in_value_t>
		typename std::enable_if<std::is_convertible<in_value_t, value_type>::value, insert_result_t>::type insert(const_iterator hint, in_value_t&& value)
		{
			return result_<typename params_t::unique_type>(insert_hint(typename params_t::unique_type(), hint, std::forward<in_value_t>(value)));
		}
		template<class iterator_t>
		void						insert(iterator_t begin, iterator_t end);
		void						insert(std::initializer_list<value_type> il);
		template<class ...args_t>
		insert_result_t				emplace(args_t&&... args);
		template<class ...args_t>
		insert_result_t				emplace_hint(const_iterator hint, args_t&&... args);
		iterator					find(key_type const& key);
		const_iterator				find(key_type const& key) const;
		iterator					lower_bound(key_type const& key);
		const_iterator				lower_bound(key_type const& key) const;
		iterator					upper_bound(key_type const& key);
		const_iterator				upper_bound(key_type const& key) const;
		pair_ii_t					equal_range(key_type const& key);
		pair_cici_t					equal_range(key_type const& key) const;
		size_type					count(key_type const& key) const;
		template<class in_key_t, class = typename std::enable_if<std::is_convertible<in_key_t, key_type>::value && params_t::unique_type::value && !std::is_same<key_type, value_type>::value, void>::type>
		mapped_type&				at(in_key_t const& key);
		template<class in_key_t, class = typename std::enable_if<std::is_convertible<in_key_t, key_type>::value && params_t::unique_type::value && !std::is_same<key_type, value_type>::value, void>::type>
		mapped_type const&			at(in_key_t const& key) const;
		template<class in_key_t, class = typename std::enable_if<std::is_convertible<in_key_t, key_type>::value && params_t::unique_type::value && !std::is_same<key_type, value_type>::value, void>::type>
		mapped_type&				operator[](in_key_t&& key);
		size_type					erase(key_type const& key);
		iterator					erase(const_iterator it);
		iterator					erase(const_iterator erase_begin, const_iterator erase_end);
		void						clear();
		bool						is_tree_balance() const;
		bool						verify() const;
	protected:
		node_allocator_t&			get_node_allocator()
		{
			return _impl;
		}
		node_allocator_t const&		get_node_allocator() const
		{
			return _impl;
		}
		key_compare&				get_key_comp()
		{
			return _impl;
		}
		key_compare const&			get_key_comp() const
		{
			return _impl;
		}
		link_type					get_node()
		{
			return get_node_allocator().allocate(1);
			//return _impl.node_allocator_t::allocate(1);
		}
		link_type&					head() const
		{
			return const_cast<link_type&>(_impl.header);
		}
		link_type&					root() const
		{
			return _impl.header->parent;
		}
		link_type&					left_most() const
		{
			return _impl.header->left;
		}
		link_type&					right_most() const
		{
			return _impl.header->right;
		}
		link_type					lower_bound_(key_type const& key) const;
		link_type					upper_bound_(key_type const& key) const;
		template<class in_value_t>
		link_type					create_node(signed char bf, link_type parent, in_value_t&& value);
		void						destroy_node(link_type& dispose);
		void						init();
		template<bool is_move>
		void						copy_all(link_type dstParent, link_type src);
		void						dealloc_all();
		bool						is_balance(link_type const node) const;
		difference_type				height(link_type const node) const;
		void						left_rotate(link_type rotate_node);
		void						right_rotate(link_type rotate_node);
		void						leftright_rotate(link_type rotate_node);
		void						rightleft_rotate(link_type rotate_node);
		void						insert_rebalance(link_type fixup_node);
		link_type					erase_rebalance(link_type const fixup_node);
		// set/map
		template<class in_value_t>
		pair_posi_t					insert_nohint(std::true_type, bool is_leftish, in_value_t&& value);
		// multiset/multimap
		template<class in_value_t>
		pair_posi_t					insert_nohint(std::false_type, bool is_leftish, in_value_t&& value);
		// set/map
		template<class in_value_t>
		pair_posi_t					insert_hint(std::true_type, const_iterator where, in_value_t&& value);
		// multiset/multimap
		template<class in_value_t>
		pair_posi_t					insert_hint(std::false_type, const_iterator where, in_value_t&& value);
		template<class in_value_t>
		link_type					insert_at(bool addLeft, link_type insert_parent, in_value_t&& value);
		void						erase(link_type erase_node);
	};


	template<class params_t>
	avl_tree<params_t>::avl_tree()
		: avl_tree(key_compare(), allocator_type())
	{
	}

	template<class params_t>
	avl_tree<params_t>::avl_tree(key_compare const& compare)
		: avl_tree(compare, allocator_type())
	{
	}

	template<class params_t>
	avl_tree<params_t>::avl_tree(allocator_type const& alloc)
		: avl_tree(key_compare(), alloc)
	{
	}

	template<class params_t>
	avl_tree<params_t>::avl_tree(key_compare const& compare, allocator_type const& alloc)
		: _impl(compare, alloc)
	{
		init();
	}

	template<class params_t>
	template<class iterator_t>
	avl_tree<params_t>::avl_tree(iterator_t begin, iterator_t end, key_compare const& compare, allocator_type const& alloc)
		: _impl(compare, alloc)
	{
		init();
		insert(begin, end);
	}

	template<class params_t>
	template<class iterator_t>
	avl_tree<params_t>::avl_tree(iterator_t begin, iterator_t end, key_compare const& compare)
		: avl_tree(begin, end, compare, allocator_type())
	{
	}

	template<class params_t>
	template<class iterator_t>
	avl_tree<params_t>::avl_tree(iterator_t begin, iterator_t end)
		: avl_tree(begin, end, key_compare(), allocator_type())
	{
	}

	template<class params_t>
	avl_tree<params_t>::avl_tree(std::initializer_list<value_type> il, key_compare const& compare, allocator_type const& alloc)
		: avl_tree(il.begin(), il.end(), compare, alloc)
	{
	}

	template<class params_t>
	avl_tree<params_t>::avl_tree(std::initializer_list<value_type> il, key_compare const& compare)
		: avl_tree(il.begin(), il.end(), compare)
	{
	}

	template<class params_t>
	avl_tree<params_t>::avl_tree(std::initializer_list<value_type> il)
		: avl_tree(il.begin(), il.end())
	{
	}

	template<class params_t>
	avl_tree<params_t>::avl_tree(avl_tree const& other)
		: _impl(other.get_key_comp(), other.get_node_allocator())
	{
		init();
		copy_all<false>(head(), other.root());
		left_most() = node_type::minimum(root());
		right_most() = node_type::maximum(root());
		_impl.size = other.size();
	}

	template<class params_t>
	avl_tree<params_t>::avl_tree(avl_tree const& other, allocator_type const& alloc)
		: _impl(other.get_key_comp(), alloc)
	{
		init();
		copy_all<false>(head(), other.root());
		left_most() = node_type::minimum(root());
		right_most() = node_type::maximum(root());
		_impl.size = other.size();
	}

	template<class params_t>
	avl_tree<params_t>::avl_tree(avl_tree&& other)
		: _impl(key_compare(), node_allocator_t())
	{
		init();
		swap(other);
	}

	template<class params_t>
	avl_tree<params_t>::avl_tree(avl_tree&& other, allocator_type const& alloc)
		: _impl(std::move(other.get_key_comp()), alloc)
	{
		init();
		swap(other);
		/*copy_all<true>(head(), other.root());
		left_most() = node_type::minimum(root());
		right_most() = node_type::maximum(root());
		_impl.size = other._impl.size;*/
	}

	template<class params_t>
	avl_tree<params_t>::~avl_tree()
	{
		dealloc_all();
	}

	template<class params_t>
	avl_tree<params_t>& avl_tree<params_t>::operator = (avl_tree const& other)
	{
		if (this == &other)
		{
			return *this;
		}
		clear();
		get_key_comp() = other.get_key_comp();
		get_node_allocator() = other.get_node_allocator();
		copy_all<false>(head(), other.root());
		left_most() = node_type::minimum(root());
		right_most() = node_type::maximum(root());
		_impl.size = other._impl.size;
		return *this;
	}

	template<class params_t>
	avl_tree<params_t>& avl_tree<params_t>::operator = (avl_tree&& other)
	{
		if (this == &other)
		{
			return *this;
		}
		swap(other);
		other.clear();
		return *this;
	}

	template<class params_t>
	avl_tree<params_t>& avl_tree<params_t>::operator = (std::initializer_list<value_type> il)
	{
		clear();
		insert(il.begin(), il.end());
		return *this;
	}

	template<class params_t>
	void avl_tree<params_t>::swap(avl_tree& other)
	{
		std::swap(_impl, other._impl);
	}

	template<class params_t>
	typename avl_tree<params_t>::size_type avl_tree<params_t>::size() const
	{
		return _impl.size;
	}

	template<class params_t>
	typename avl_tree<params_t>::size_type avl_tree<params_t>::max_size() const
	{
		return get_node_allocator().max_size();
	}

	template<class params_t>
	bool avl_tree<params_t>::empty() const
	{
		return _impl.size == 0;
	}

	template<class params_t>
	typename avl_tree<params_t>::iterator avl_tree<params_t>::begin()
	{
		return iterator(left_most());
	}

	template<class params_t>
	typename avl_tree<params_t>::const_iterator avl_tree<params_t>::begin() const
	{
		return const_iterator(left_most());
	}

	template<class params_t>
	typename avl_tree<params_t>::iterator avl_tree<params_t>::end()
	{
		return iterator(_impl.header);
	}

	template<class params_t>
	typename avl_tree<params_t>::const_iterator avl_tree<params_t>::end() const
	{
		return const_iterator(_impl.header);
	}

	template<class params_t>
	typename avl_tree<params_t>::reverse_iterator avl_tree<params_t>::rbegin()
	{
		return reverse_iterator(end());
	}

	template<class params_t>
	typename avl_tree<params_t>::const_reverse_iterator avl_tree<params_t>::rbegin() const
	{
		return const_reverse_iterator(end());
	}

	template<class params_t>
	typename avl_tree<params_t>::reverse_iterator avl_tree<params_t>::rend()
	{
		return reverse_iterator(begin());
	}

	template<class params_t>
	typename avl_tree<params_t>::const_reverse_iterator avl_tree<params_t>::rend() const
	{
		return const_reverse_iterator(begin());
	}

	template<class params_t>
	typename avl_tree<params_t>::const_iterator avl_tree<params_t>::cbegin() const
	{
		return const_iterator(begin());
	}

	template<class params_t>
	typename avl_tree<params_t>::const_iterator avl_tree<params_t>::cend() const
	{
		return const_iterator(end());
	}

	template<class params_t>
	typename avl_tree<params_t>::const_reverse_iterator avl_tree<params_t>::crbegin() const
	{
		return const_reverse_iterator(rbegin());
	}

	template<class params_t>
	typename avl_tree<params_t>::const_reverse_iterator avl_tree<params_t>::crend() const
	{
		return const_reverse_iterator(rend());
	}

	template<class params_t>
	typename avl_tree<params_t>::insert_result_t avl_tree<params_t>::insert(value_type const& value)
	{
		return result_<typename params_t::unique_type>(insert_nohint(typename params_t::unique_type(), false, value));
	}

	template<class params_t>
	typename avl_tree<params_t>::insert_result_t avl_tree<params_t>::insert(value_type&& value)
	{
		return result_<typename params_t::unique_type>(insert_nohint(typename params_t::unique_type(), false, std::forward<value_type>(value)));
	}

	/*template<class params_t>
	template<class in_value_t>
	typename std::enable_if<std::is_convertible<in_value_t, typename avl_tree<params_t>::value_type>::value, typename avl_tree<params_t>::insert_result_t>::type
	avl_tree<params_t>::insert(in_value_t&& value)
	{
	return result_<typename params_t::unique_type>(insert_nohint(typename params_t::unique_type(), false, std::forward<in_value_t>(value)));
	}*/

	template<class params_t>
	typename avl_tree<params_t>::insert_result_t avl_tree<params_t>::insert(const_iterator hint, value_type const& value)
	{
		return result_<typename params_t::unique_type>(insert_hint(typename params_t::unique_type(), hint, value));
	}

	template<class params_t>
	typename avl_tree<params_t>::insert_result_t avl_tree<params_t>::insert(const_iterator hint, value_type&& value)
	{
		return result_<typename params_t::unique_type>(insert_hint(typename params_t::unique_type(), hint, std::forward<value_type>(value)));
	}

	/*template<class params_t>
	template<class in_value_t>
	typename std::enable_if<std::is_convertible<in_value_t, typename avl_tree<params_t>::value_type>::value, typename avl_tree<params_t>::insert_result_t>::type
	avl_tree<params_t>::insert(const_iterator hint, in_value_t&& value)
	{
	return result_<typename params_t::unique_type>(insert_hint(typename params_t::unique_type(), hint, std::forward<in_value_t>(value)));
	}*/

	template<class params_t>
	template<class iterator_t>
	void avl_tree<params_t>::insert(iterator_t begin, iterator_t end)
	{
		for (; begin != end; ++begin)
		{
			emplace_hint(cend(), *begin);
		}
	}

	template<class params_t>
	void avl_tree<params_t>::insert(std::initializer_list<value_type> il)
	{
		insert(il.begin(), il.end());
	}

	template<class params_t>
	template<class ...args_t>
	typename avl_tree<params_t>::insert_result_t avl_tree<params_t>::emplace(args_t&&... args)
	{
		return result_<typename params_t::unique_type>(insert_nohint(typename params_t::unique_type(), false, std::move(value_type(std::forward<args_t>(args)...))));
	}

	template<class params_t>
	template<class ...args_t>
	typename avl_tree<params_t>::insert_result_t avl_tree<params_t>::emplace_hint(const_iterator hint, args_t&&... args)
	{
		return result_<typename params_t::unique_type>(insert_hint(typename params_t::unique_type(), hint, std::move(value_type(std::forward<args_t>(args)...))));
	}

	template<class params_t>
	typename avl_tree<params_t>::iterator avl_tree<params_t>::find(key_type const& key)
	{
		link_type node = lower_bound_(key);
		return (node_type::is_nil(node) || get_key_comp()(key, get_key_t()(node->value))) ? end() : iterator(node);
	}

	template<class params_t>
	typename avl_tree<params_t>::const_iterator avl_tree<params_t>::find(key_type const& key) const
	{
		link_type node = lower_bound_(key);
		return (node_type::is_nil(node) || get_key_comp()(key, get_key_t()(node->value))) ? end() : const_iterator(node);
	}

	template<class params_t>
	typename avl_tree<params_t>::iterator avl_tree<params_t>::lower_bound(key_type const& key)
	{
		return iterator(lower_bound_(key));
	}

	template<class params_t>
	typename avl_tree<params_t>::const_iterator avl_tree<params_t>::lower_bound(key_type const& key) const
	{
		return const_iterator(lower_bound_(key));
	}

	template<class params_t>
	typename avl_tree<params_t>::iterator avl_tree<params_t>::upper_bound(key_type const& key)
	{
		return iterator(upper_bound_(key));
	}

	template<class params_t>
	typename avl_tree<params_t>::const_iterator avl_tree<params_t>::upper_bound(key_type const& key) const
	{
		return const_iterator(upper_bound_(key));
	}

	template<class params_t>
	typename avl_tree<params_t>::pair_ii_t avl_tree<params_t>::equal_range(key_type const& key)
	{
		return pair_ii_t(iterator(lower_bound_(key)), iterator(upper_bound_(key)));
	}

	template<class params_t>
	typename avl_tree<params_t>::pair_cici_t avl_tree<params_t>::equal_range(key_type const& key) const
	{
		return pair_cici_t(const_iterator(lower_bound_(key)), const_iterator(upper_bound_(key)));
	}

	template<class params_t>
	typename avl_tree<params_t>::size_type avl_tree<params_t>::count(key_type const& key) const
	{
		if (params_t::unique_type::value)
		{
			return find(key) == end() ? 0 : 1;
		}
		else
		{
			pair_cici_t range = equal_range(key);
			return std::distance(range.first, range.second);
		}
	}

	template<class params_t>
	template<class in_key_t, class = typename std::enable_if<std::is_convertible<in_key_t, key_type>::value && params_t::unique_type::value && !std::is_same<key_type, value_type>::value, void>::type>
	typename avl_tree<params_t>::mapped_type& avl_tree<params_t>::at(in_key_t const& key)
	{
		link_type node = lower_bound_(key);
		if (node_type::is_nil(node) || get_key_comp()(key, get_key_t()(node->value)))
		{
			throw std::out_of_range("avl_tree out of range");
		}
		return node->value.second;
	}

	template<class params_t>
	template<class in_key_t, class = typename std::enable_if<std::is_convertible<in_key_t, key_type>::value && params_t::unique_type::value && !std::is_same<key_type, value_type>::value, void>::type>
	typename avl_tree<params_t>::mapped_type const& avl_tree<params_t>::at(in_key_t const& key) const
	{
		link_type node = lower_bound_(key);
		if (node_type::is_nil(node) || get_key_comp()(key, get_key_t()(node->value)))
		{
			throw std::out_of_range("avl_tree out of range");
		}
		return node->value.second;
	}

	template<class params_t>
	template<class in_key_t, class = typename std::enable_if<std::is_convertible<in_key_t, key_type>::value && params_t::unique_type::value && !std::is_same<key_type, value_type>::value, void>::type>
	typename avl_tree<params_t>::mapped_type& avl_tree<params_t>::operator[](in_key_t&& key)
	{
		iterator it = lower_bound(key);
		if (it == end() || get_key_comp()(key, get_key_t()(*it)))
		{
			it = emplace_hint(it, key, mapped_type()).first;
		}
		return it->second;
	}

	template<class params_t>
	typename avl_tree<params_t>::iterator avl_tree<params_t>::erase(const_iterator it)
	{
		if (node_type::is_nil(it.node))
		{
			throw std::out_of_range("avltree map/set erase iterator outside range");
		}
		link_type nextNode = node_type::successor(it.node);
		link_type del = erase_rebalance(it.node);
		destroy_node(del);
		--_impl.size;
		return iterator(nextNode);
	}

	template<class params_t>
	typename avl_tree<params_t>::size_type avl_tree<params_t>::erase(key_type const& key)
	{
		pair_ii_t where = equal_range(key);
		difference_type num = std::distance(where.first, where.second);
		erase(where.first, where.second);
		return (size_type)num;
	}

	template<class params_t>
	typename avl_tree<params_t>::iterator avl_tree<params_t>::erase(const_iterator erase_begin, const_iterator erase_end)
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

	template<class params_t>
	void avl_tree<params_t>::clear()
	{
		erase(root());
		root() = _impl.header;
		left_most() = _impl.header;
		right_most() = _impl.header;
	}

	template<class params_t>
	bool avl_tree<params_t>::is_tree_balance() const
	{
		return is_balance(root());
	}

	template<class params_t>
	bool avl_tree<params_t>::verify() const
	{
		if (size() == 0 || begin() == end())
			return size() == 0 && begin() == end() && left_most() == _impl.header && right_most() == _impl.header;
		if (!is_tree_balance())
			return false;
		if (left_most() != node_type::minimum(root()))
			return false;
		if (right_most() != node_type::maximum(root()))
			return false;
		return true;
	}

	template<class params_t>
	typename avl_tree<params_t>::link_type avl_tree<params_t>::lower_bound_(key_type const& key) const
	{
		auto getkey = get_key_t();
		auto getkeycomp = get_key_comp();
		link_type psearch = _impl.header;
		link_type search = _impl.header->parent;
		while (!node_type::is_nil(search))
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

	template<class params_t>
	typename avl_tree<params_t>::link_type avl_tree<params_t>::upper_bound_(key_type const& key) const
	{
		auto getkey = get_key_t();
		auto getkeycomp = get_key_comp();
		link_type psearch = _impl.header;
		link_type search = _impl.header->parent;
		while (!node_type::is_nil(search))
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

	template<class params_t>
	template<class in_value_t>
	typename avl_tree<params_t>::link_type avl_tree<params_t>::create_node(signed char bf, link_type parent, in_value_t&& value)
	{
		link_type node = get_node();
		new (&node->value) value_type(std::forward<in_value_t>(value));
		node->balance_factor = bf;
		node->parent = parent;
		node->left = _impl.header;
		node->right = _impl.header;
		return node;
	}

	template<class params_t>
	void avl_tree<params_t>::destroy_node(link_type& dispose)
	{
		get_node_allocator().deallocate(dispose, 1);
		dispose = nullptr;
	}

	template<class params_t>
	void avl_tree<params_t>::init()
	{
		_impl.header = get_node();
		_impl.header->balance_factor = -2;
		_impl.header->parent = _impl.header;
		_impl.header->left = _impl.header;
		_impl.header->right = _impl.header;
		_impl.size = 0;
	}

	template<class params_t>
	template<bool is_move>
	void avl_tree<params_t>::copy_all(link_type dstParent, link_type src)
	{
		if (node_type::is_nil(src)) return;
		link_type dst;
		if (is_move)
		{
			dst = create_node(src->balance_factor, dstParent, std::move(src->value));
		}
		else
		{
			dst = create_node(src->balance_factor, dstParent, src->value);
		}
		dst->left = _impl.header;
		dst->right = _impl.header;
		if (node_type::is_nil(src->parent))           //根节点
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

	template<class params_t>
	void avl_tree<params_t>::dealloc_all()
	{
		if (_impl.header != nullptr)
		{
			clear();
			destroy_node(_impl.header);
			//_impl.header = nullptr;
		}
	}

	template<class params_t>
	bool avl_tree<params_t>::is_balance(link_type const node) const
	{
		if (node_type::is_nil(node))
		{
			return true;
		}
		difference_type lefth = height(node->left);
		difference_type righth = height(node->right);

		difference_type bf = abs(righth - lefth);
		if (bf > 1)
		{
			return false;
		}
		if (bf != abs(node->balance_factor))
		{
			//std::cout << "errorKey:   " << get_key_t()(node->value) << std::endl;
			return false;
		}
		return is_balance(node->left) && is_balance(node->right);
	}

	template<class params_t>
	typename avl_tree<params_t>::difference_type avl_tree<params_t>::height(link_type const node) const
	{
		if (node_type::is_nil(node))
		{
			return 0;
		}
		difference_type lefth = height(node->left);
		difference_type righth = height(node->right);
		return lefth > righth ? lefth + 1 : righth + 1;
	}

	template<class params_t>
	void avl_tree<params_t>::left_rotate(link_type rotate_node)
	{
		/*x->rotate_node,y->sub_right
		|                          |
		x(2)                       y(0)
		/ \          ==>           / \
		n[a]  y(1)n+2          n+1(0)x  [c]n+1
		/ \                    / \
		n[b] [c]n+1            n[a] [b]n
		*/

		link_type const sub_right = rotate_node->right;
		// switch
		rotate_node->right = sub_right->left;
		sub_right->left = rotate_node;
		// rearrange parents
		sub_right->parent = rotate_node->parent;
		rotate_node->parent = sub_right;
		// do we have [b]?
		if (!node_type::is_nil(rotate_node->right))
			rotate_node->right->parent = rotate_node;

		if (root() == rotate_node)
			root() = sub_right;
		else
			// need to reparent sub_right
			if (sub_right->parent->left == rotate_node)
				sub_right->parent->left = sub_right;
			else
				sub_right->parent->right = sub_right;

		// reset the balancing factor
		if (sub_right->balance_factor == 1)
		{
			rotate_node->balance_factor = 0;
			sub_right->balance_factor = 0;
		}
		else	// this doesn't happen during insertions
		{
			rotate_node->balance_factor = 1;
			sub_right->balance_factor = -1;
		}
	}

	template<class params_t>
	void avl_tree<params_t>::right_rotate(link_type rotate_node)
	{
		link_type const sub_left = rotate_node->left;
		// switch
		rotate_node->left = sub_left->right;
		sub_left->right = rotate_node;
		// rearrange parents
		sub_left->parent = rotate_node->parent;
		rotate_node->parent = sub_left;
		// do we have [b]?
		if (!node_type::is_nil(rotate_node->left))
			rotate_node->left->parent = rotate_node;

		if (root() == rotate_node)
			root() = sub_left;
		else
			// need to reparent sub_right
			if (sub_left->parent->left == rotate_node)
				sub_left->parent->left = sub_left;
			else
				sub_left->parent->right = sub_left;

		// reset the balancing factor
		if (sub_left->balance_factor == -1)
		{
			rotate_node->balance_factor = 0;
			sub_left->balance_factor = 0;
		}
		else	// this doesn't happen during insertions
		{
			rotate_node->balance_factor = -1;
			sub_left->balance_factor = 1;
		}
	}

	template<class params_t>
	void avl_tree<params_t>::leftright_rotate(link_type rotate_node)
	{
		/*a->rotate_node,b->lson,c->lrgrandson
		|                               |
		a(-2)                           c
		/ \                             / \
		/   \        ==>                /   \
		(1)b    [g]                        b     a
		/ \                             / \   / \
		[d]  c                          [d] e  f  [g]
		/ \
		e   f
		*/

		link_type lson = rotate_node->left;
		link_type lrgrandson = lson->right;
		// switch
		rotate_node->left = lrgrandson->right;
		lson->right = lrgrandson->left;
		lrgrandson->right = rotate_node;
		lrgrandson->left = lson;
		// set the parents
		lrgrandson->parent = rotate_node->parent;
		rotate_node->parent = lson->parent = lrgrandson;

		if (!node_type::is_nil(rotate_node->left))   // do we have f?
			rotate_node->left->parent = rotate_node;
		if (!node_type::is_nil(lson->right))    // do we have e?
			lson->right->parent = lson;

		if (root() == rotate_node)
			root() = lrgrandson;
		else    // rotate_node had a parent, his child is now lrgrandson
			if (rotate_node == lrgrandson->parent->left)
				lrgrandson->parent->left = lrgrandson;
			else
				lrgrandson->parent->right = lrgrandson;

		// balancing...
		switch (lrgrandson->balance_factor)
		{
		case -1:
			rotate_node->balance_factor = 1;
			lson->balance_factor = 0;
			break;
		case 0:
			rotate_node->balance_factor = 0;
			lson->balance_factor = 0;
			break;
		case 1:
			rotate_node->balance_factor = 0;
			lson->balance_factor = -1;
			break;
		default:        assert(false);  // never reached
		}
		lrgrandson->balance_factor = 0;
	}

	template<class params_t>
	void avl_tree<params_t>::rightleft_rotate(link_type rotate_node)
	{
		/*a->rotate_node,b->rson,c->rlgrandson
		|                               |
		a(1)                            c
		/ \                             / \
		/   \                           /   \
		[d]   b(-1)          ==>         a     b
		/ \                        / \   / \
		c  [g]                    [d] e  f  [g]
		/ \
		e  f
		*/

		link_type rson = rotate_node->right;
		link_type rlgrandson = rson->left;
		// switch
		rotate_node->right = rlgrandson->left;
		rson->left = rlgrandson->right;
		rlgrandson->left = rotate_node;
		rlgrandson->right = rson;
		// fix the parents
		rlgrandson->parent = rotate_node->parent;
		rotate_node->parent = rson->parent = rlgrandson;

		if (!node_type::is_nil(rotate_node->right))   // have e?
			rotate_node->right->parent = rotate_node;
		if (!node_type::is_nil(rson->left))    // have f?
			rson->left->parent = rson;

		if (root() == rotate_node)
			root() = rlgrandson;
		else    // rotate_node had a parent, his child is now rlgrandson
			if (rotate_node == rlgrandson->parent->left)
				rlgrandson->parent->left = rlgrandson;
			else
				rlgrandson->parent->right = rlgrandson;

		// balancing
		switch (rlgrandson->balance_factor)
		{
		case -1:
			rotate_node->balance_factor = 0;
			rson->balance_factor = 1;
			break;
		case 0:
			rotate_node->balance_factor = 0;
			rson->balance_factor = 0;
			break;
		case 1:
			rotate_node->balance_factor = -1;
			rson->balance_factor = 0;
			break;

		default:        assert(false);  // never reached
		}
		rlgrandson->balance_factor = 0;
	}

	template<class params_t>
	void avl_tree<params_t>::insert_rebalance(link_type fixup_node)
	{
		while (!node_type::is_nil(fixup_node->parent))
		{
			switch (fixup_node->parent->balance_factor)
			{
			case 0:
				// if fixup_node is left, parent will have parent->balance_factor = -1
				// else, parent->balance_factor = 1
				fixup_node->parent->balance_factor = (fixup_node == fixup_node->parent->left) ? -1 : 1;
				fixup_node = fixup_node->parent;
				break;
			case 1:
				// if fixup_node is a left child, parent->balance_factor = 0
				if (fixup_node == fixup_node->parent->left)
					fixup_node->parent->balance_factor = 0;
				else
				{        // fixup_node is a right child, needs rebalancing
					if (fixup_node->balance_factor == -1)
						rightleft_rotate(fixup_node->parent);
					else
						left_rotate(fixup_node->parent);
				}
				return;
			case -1:
				// if fixup_node is a left child, needs rebalancing
				if (fixup_node == fixup_node->parent->left)
				{
					if (fixup_node->balance_factor == 1)
						leftright_rotate(fixup_node->parent);
					else
						right_rotate(fixup_node->parent);
				}
				else
					fixup_node->parent->balance_factor = 0;
				return;

			default:        assert(false);  // never reached
			}
		}
	}

	template<class params_t>
	typename avl_tree<params_t>::link_type avl_tree<params_t>::erase_rebalance(link_type const fixup_node)
	{
		link_type y = fixup_node;
		link_type x = _impl.header;
		link_type x_parent = _impl.header;

		if (node_type::is_nil(y->left))     // fixup_node has at most one non-null child. y == fixup_node.
			x = y->right;     // x might be null.
		else
			if (node_type::is_nil(y->right))  // fixup_node has exactly one non-null child. y == fixup_node.
				x = y->left;    // x is not null.
			else
			{
				// fixup_node has two non-null children.  Set y to
				y = y->right;   //   fixup_node's successor.  x might be null.
				while (!node_type::is_nil(y->left))
					y = y->left;
				x = y->right;
			}

		if (y != fixup_node)
		{
			// relink y in place of fixup_node.  y is fixup_node's successor
			fixup_node->left->parent = y;
			y->left = fixup_node->left;
			if (y != fixup_node->right)
			{
				x_parent = y->parent;
				if (!node_type::is_nil(x))
					x->parent = y->parent;
				y->parent->left = x;   // y must be a child of left
				y->right = fixup_node->right;
				fixup_node->right->parent = y;
			}
			else
				x_parent = y;
			if (root() == fixup_node)      // if we are deleting the root
				root() = y;   // the new root is y
			else if (fixup_node->parent->left == fixup_node)        // else, fix parent's child
				fixup_node->parent->left = y;
			else
				fixup_node->parent->right = y;
			y->parent = fixup_node->parent;
			y->balance_factor = fixup_node->balance_factor;
			y = fixup_node;
			// y now points to node to be actually deleted
		}
		else
		{                        // y == fixup_node    --> fixup_node has only one child, or none
			x_parent = y->parent;
			if (!node_type::is_nil(x))        // if fixup_node has at least one child
				x->parent = y->parent;      // new parent is now y
			if (root() == fixup_node)      // if we deleted the root
				root() = x;   // new root is x
			else    // else, fix the parent's child
				if (fixup_node->parent->left == fixup_node)
					fixup_node->parent->left = x;
				else
					fixup_node->parent->right = x;
			if (left_most() == fixup_node)
			{ // need to fix the nil?
				if (node_type::is_nil(fixup_node->right))           // fixup_node->left must be null also
																	// because fixup_node is leftmost. if fixup_node had left,
																	// fixup_node wouldn't be leftmost.
					left_most() = fixup_node->parent;
				// makes leftmost == nil if fixup_node == root
				else
					left_most() = node_type::minimum(x);
			}
			if (right_most() == fixup_node)
			{ // if fixup_node is rightmost, fixup_node don't have right
				if (node_type::is_nil(fixup_node->left))         // fixup_node->right must be null also
					right_most() = fixup_node->parent;
				// makes rightmost == nil if fixup_node == root
				else                      // x == fixup_node->left
					right_most() = node_type::maximum(x);
			}
		}

		// Rebalancing
		// x: may be null

		while (x != root())
		{
			switch (x_parent->balance_factor)
			{
			case 0:
				x_parent->balance_factor = (x == x_parent->right) ? -1 : 1;
				return y;       // the height didn't change, let's stop here
			case -1:
				if (x == x_parent->left)
				{
					x_parent->balance_factor = 0; // balanced
					x = x_parent;
					x_parent = x_parent->parent;
				}
				else
				{
					// x is right child
					// a is left child
					link_type a = x_parent->left;
					assert(!node_type::is_nil(a));
					if (a->balance_factor == 1)
					{
						// a MUST have a right child
						assert(!node_type::is_nil(a->right));
						leftright_rotate(x_parent);
						x = x_parent->parent;
						x_parent = x_parent->parent->parent;
					}
					else
					{
						right_rotate(x_parent);
						x = x_parent->parent;
						x_parent = x_parent->parent->parent;
					}
					// if changed from -1 to 1, no need to check above
					if (x->balance_factor == 1)
						return y;
				}
				break;
			case 1:
				if (x == x_parent->right)
				{
					x_parent->balance_factor = 0; // balanced
					x = x_parent;
					x_parent = x_parent->parent;
				}
				else
				{
					// x is left child
					// a is right child
					link_type a = x_parent->right;
					assert(!node_type::is_nil(a));
					if (a->balance_factor == -1)
					{
						// a MUST have then a left child
						assert(!node_type::is_nil(a->left));
						rightleft_rotate(x_parent);
						x = x_parent->parent;
						x_parent = x_parent->parent->parent;
					}
					else
					{
						left_rotate(x_parent);
						x = x_parent->parent;
						x_parent = x_parent->parent->parent;
					}
					// if changed from 1 to -1, no need to check above
					if (x->balance_factor == -1)
						return y;
				}
				break;
			default:        assert(false);  // never reached
			}
		}

		return y;
	}

	template<class params_t>
	template<class in_value_t>
	typename avl_tree<params_t>::pair_posi_t avl_tree<params_t>::insert_nohint(std::true_type, bool is_leftish, in_value_t&& value)
	{
		auto getkey = get_key_t();
		auto getkeycomp = get_key_comp();
		link_type psearch = _impl.header;
		link_type search = root();
		bool addLeft = true;
		key_type key = getkey(value);
		if (is_leftish)
		{
			while (!node_type::is_nil(search))
			{
				psearch = search;
				addLeft = !getkeycomp(getkey(search->value), key);
				search = addLeft ? search->left : search->right;
			}
		}
		else
		{
			while (!node_type::is_nil(search))
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

	template<class params_t>
	template<class in_value_t>
	typename avl_tree<params_t>::pair_posi_t avl_tree<params_t>::insert_nohint(std::false_type, bool is_leftish, in_value_t&& value)
	{
		auto getkey = get_key_t();
		auto getkeycomp = get_key_comp();
		link_type psearch = _impl.header;
		link_type search = _impl.header->parent;
		key_type key = getkey(value);
		bool addLeft = true;
		if (is_leftish)
		{
			while (!node_type::is_nil(search))
			{
				psearch = search;
				addLeft = !getkeycomp(getkey(search->value), key);
				search = addLeft ? search->left : search->right;
			}
		}
		else
		{
			while (!node_type::is_nil(search))
			{
				psearch = search;
				addLeft = getkeycomp(key, getkey(search->value));
				search = addLeft ? search->left : search->right;
			}
		}
		return pair_posi_t(insert_at(addLeft, psearch, std::forward<in_value_t>(value)), true);
	}

	template<class params_t>
	template<class in_value_t>
	typename avl_tree<params_t>::pair_posi_t avl_tree<params_t>::insert_hint(std::true_type, const_iterator where, in_value_t&& value)
	{
		if (size() == 0)
		{
			return pair_posi_t(insert_at(true, _impl.header, std::forward<in_value_t>(value)), true);
		}
		auto getkey = get_key_t();
		auto getkeycomp = get_key_comp();
		key_type k = getkey(value);
		const_iterator next;
		if (where == begin())
		{
			if (getkeycomp(getkey(value), getkey(*where)))
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
			if (node_type::is_nil(next.node->right))
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
			if (node_type::is_nil(where.node->right))
			{
				return pair_posi_t(insert_at(false, where.node, std::forward<in_value_t>(value)), true);
			}
			else
			{
				return pair_posi_t(insert_at(true, next.node, std::forward<in_value_t>(value)), true);
			}
		}
		return insert_nohint(typename params_t::unique_type(), false, std::forward<in_value_t>(value));
	}

	template<class params_t>
	template<class in_value_t>
	typename avl_tree<params_t>::pair_posi_t avl_tree<params_t>::insert_hint(std::false_type, const_iterator where, in_value_t&& value)
	{
		if (size() == 0)
		{
			return pair_posi_t(insert_at(true, _impl.header, std::forward<in_value_t>(value)), true);
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
			if (node_type::is_nil(next.node->right))
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
			if (node_type::is_nil(where.node->right))
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
		return insert_nohint(typename params_t::unique_type(), is_leftish, std::forward<in_value_t>(value));
	}

	template<class params_t>
	template<class in_value_t>
	typename avl_tree<params_t>::link_type avl_tree<params_t>::insert_at(bool addLeft, link_type insert_parent, in_value_t&& value)
	{
		if (_impl.size >= max_size() - 1)
		{
			std::length_error("map/set too long");
		}
		link_type insert_node = create_node(0, insert_parent, std::forward<in_value_t>(value));

		if (node_type::is_nil(insert_parent)) // 如果插入的是一颗空树  
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
		++_impl.size;
		return insert_node;
	}

	template<class params_t>
	void avl_tree<params_t>::erase(link_type erase_node)
	{
		link_type tmp;
		while (erase_node && !node_type::is_nil(erase_node))
		{
			erase(erase_node->right);
			tmp = erase_node->left;
			destroy_node(erase_node);
			--_impl.size;
			erase_node = tmp;
		}
		/*if (!erase_node || node_type::is_nil(erase_node))
		return;
		else
		{
		erase(erase_node->left);
		erase(erase_node->right);
		destroy_node(erase_node);
		--_impl.size;
		}*/
	}
}
