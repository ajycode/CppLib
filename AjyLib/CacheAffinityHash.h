#pragma once

namespace Ajy
{
	// CacheAffinityHash
	// 原型为Dictionary
	// 基于哈希表实现
	// 内存集中分配, 尽可能利用CPU缓存加速
	// 插入元素可能导致扩容, 产生搬运数据操作
	// 插入和删除元素可能导致迭代器失效
	// 遍历速度飞快,任何情况下都比标准库快的多!
	// 查找性能比标准库好，插入和删除性能比标准库好
	// sizeof(key)巨大情况下,内存占用会偏大并且插入和删除性能下降
	// 在允许重复key时候, equal_range返回local_iterator, 仅支持erase操作
	// 有map / set / multimap / multiset实现
	// stl style
	// MSVC下natvis友好调试视图支持


	template<class params_t> class cache_affinity_hash;


	template<class key_t, class value_t, class hasher_t, class key_equal_t, class allocator_t, class unique_t>
	struct cahash_map_params_t
	{
		typedef key_t							key_type;
		typedef value_t							mapped_type;
		typedef std::pair<key_t const, value_t>	value_type;
		typedef hasher_t						hasher;
		typedef key_equal_t						key_equal;
		typedef allocator_t						allocator_type;
		typedef std::uintptr_t					offset_type;
		typedef unique_t						unique_type;
		typedef typename std::result_of<hasher(key_type)>::type hash_value_type;
		static float grow_proportion(std::size_t)
		{
			return 2;
		}
		template<class in_type>
		static key_type const& get_key(in_type&& value)
		{
			return value.first;
		}
	};
	template<class key_t, class value_t, class hasher_t = Ajy::Hasher<key_t>, class key_equal_t = std::equal_to<key_t>, class allocator_t = std::allocator<std::pair<key_t const, value_t>>>
	using cahash_map = cache_affinity_hash<cahash_map_params_t<key_t, value_t, hasher_t, key_equal_t, allocator_t, std::true_type>>;
	template<class key_t, class value_t, class hasher_t = Ajy::Hasher<key_t>, class key_equal_t = std::equal_to<key_t>, class allocator_t = std::allocator<std::pair<key_t const, value_t>>>
	using cahash_multimap = cache_affinity_hash<cahash_map_params_t<key_t, value_t, hasher_t, key_equal_t, allocator_t, std::false_type>>;


	template<class key_t, class hasher_t, class key_equal_t, class allocator_t, class unique_t>
	struct cahash_set_params_t
	{
		typedef key_t			key_type;
		typedef key_t			mapped_type;
		typedef key_t			value_type;
		typedef hasher_t		hasher;
		typedef key_equal_t		key_equal;
		typedef allocator_t		allocator_type;
		typedef std::uintptr_t	offset_type;
		typedef unique_t		unique_type;
		typedef typename std::result_of<hasher(key_type)>::type hash_value_type;
		static float grow_proportion(std::size_t)
		{
			return 2;
		}
		template<class in_type>
		static key_type const& get_key(in_type&& value)
		{
			return value;
		}
	};
	template<class key_t, class hasher_t = Ajy::Hasher<key_t>, class key_equal_t = std::equal_to<key_t>, class allocator_t = std::allocator<key_t>>
	using cahash_set = cache_affinity_hash<cahash_set_params_t<key_t, hasher_t, key_equal_t, allocator_t, std::true_type>>;
	template<class key_t, class hasher_t = Ajy::Hasher<key_t>, class key_equal_t = std::equal_to<key_t>, class allocator_t = std::allocator<key_t>>
	using cahash_multiset = cache_affinity_hash<cahash_set_params_t<key_t, hasher_t, key_equal_t, allocator_t, std::false_type>>;


	namespace cache_affinity_hash_detail
	{
		class move_trivial_tag
		{
		};

		class move_assign_tag
		{
		};

		template<class T>
		struct is_trivial_expand : public std::is_trivial<T>
		{
		};

		template<class K, class V>
		struct is_trivial_expand<std::pair<K, V>> : public std::conditional<std::is_trivial<K>::value && std::is_trivial<V>::value, std::true_type, std::false_type>::type
		{
		};

		template<class iterator_t>
		struct get_tag
		{
			typedef typename std::conditional<is_trivial_expand<typename std::iterator_traits<iterator_t>::value_type>::value, move_trivial_tag, move_assign_tag>::type type;
		};

		template<class iterator_t, class tag_t, class ...args_t>
		void construct_one(iterator_t where, tag_t, args_t&&... args)
		{
			typedef typename std::iterator_traits<iterator_t>::value_type iterator_value_t;
			::new(std::addressof(*where)) iterator_value_t(std::forward<args_t>(args)...);
		}

		template<class iterator_t>
		void destroy_one(iterator_t where, move_trivial_tag)
		{
		}

		template<class iterator_t>
		void destroy_one(iterator_t where, move_assign_tag)
		{
			typedef typename std::iterator_traits<iterator_t>::value_type iterator_value_t;
			where->~iterator_value_t();
		}

		// trivial类型可以安全memcpy和memmove
		template<class iterator_from_t, class iterator_to_t>
		void move_construct_and_destroy(iterator_from_t move_begin, iterator_from_t move_end, iterator_to_t to_begin, move_trivial_tag)
		{
			std::ptrdiff_t count = move_end - move_begin;
			std::memmove(&*to_begin, &*move_begin, count * sizeof(*move_begin));
		}
		// 非trivial类型利用placement new构造,手动析构
		template<class iterator_from_t, class iterator_to_t>
		void move_construct_and_destroy(iterator_from_t move_begin, iterator_from_t move_end, iterator_to_t to_begin, move_assign_tag)
		{
			for (; move_begin != move_end; ++move_begin)
			{
				construct_one(to_begin++, move_assign_tag(), std::move(*move_begin));
				destroy_one(move_begin, move_assign_tag());
			}
		}
	}

	template<class params_t>
	class cache_affinity_hash
	{
	public:
		typedef typename params_t::key_type			key_type;
		typedef typename params_t::mapped_type		mapped_type;
		typedef typename params_t::value_type		value_type;
		typedef std::size_t							size_type;
		typedef std::ptrdiff_t						difference_type;
		typedef typename params_t::hasher			hasher;
		typedef typename params_t::key_equal		key_equal;
		typedef typename params_t::allocator_type	allocator_type;
		typedef typename params_t::offset_type		offset_type;
		typedef typename params_t::hash_value_type	hash_value_type;
		typedef value_type&							reference;
		typedef value_type const&					const_reference;
		typedef value_type*							pointer;
		typedef value_type const*					const_pointer;

		static constexpr offset_type offset_empty = offset_type(-1);

	protected:
		struct hash_t
		{
			hash_value_type hash;

			hash_t()
			{
			}

			hash_t(hash_value_type value)
			{
				hash = value & ~(hash_value_type(1) << (sizeof(hash_value_type) * 8 - 1));
			}

			hash_t& operator = (hash_t const& other)
			{
				hash = other.hash;
				return *this;
			}

			template<class any_type>
			any_type operator % (any_type const& value) const
			{
				return hash % value;
			}

			bool operator == (hash_t const& other) const
			{
				return hash == other.hash;
			}

			bool operator !() const
			{
				return hash == ~hash_value_type(0);
			}

			operator bool() const
			{
				return hash != ~hash_value_type(0);
			}

			void clear()
			{
				hash = ~hash_value_type(0);
			}
		};
		struct index_t
		{
			hash_t hash;
			offset_type next;
			offset_type prev;
		};
		struct value_t
		{
			typename std::aligned_storage<sizeof(value_type), alignof(value_type)>::type value_pod;

			value_type* value()
			{
				return reinterpret_cast<value_type*>(&value_pod);
			}

			value_type const* value() const
			{
				return reinterpret_cast<value_type const*>(&value_pod);
			}
		};

		typedef typename allocator_type::template rebind<offset_type>::other bucket_allocator_t;
		typedef typename allocator_type::template rebind<index_t>::other index_allocator_t;
		typedef typename allocator_type::template rebind<value_t>::other value_allocator_t;
		struct impl_t : public hasher, public key_equal, public bucket_allocator_t, public index_allocator_t, public value_allocator_t
		{
			template<class any_hasher, class any_key_equal, class any_allocator_type>
			impl_t(any_hasher&& hash, any_key_equal&& equal, any_allocator_type&& alloc)
				: hasher(std::forward<any_hasher>(hash))
				, key_equal(std::forward<any_key_equal>(equal))
				, bucket_allocator_t(alloc)
				, index_allocator_t(alloc)
				, value_allocator_t(std::forward<any_allocator_type>(alloc))
			{
				static_assert(std::is_unsigned<offset_type>::value && std::is_integral<offset_type>::value, "offset_type must be unsighed integer");
				static_assert(sizeof(offset_type) <= sizeof(cache_affinity_hash::size_type), "offset_type too big");
				static_assert(std::is_integral<hash_value_type>::value, "hash_value_type must be integer");
				bucket_count = 0;
				capacity = 0;
				size = 0;
				free_count = 0;
				free_list = offset_empty;
				setting_load_factor = 1;
				bucket = nullptr;
				index = nullptr;
				value = nullptr;
			}
			typename cache_affinity_hash::size_type bucket_count;
			typename cache_affinity_hash::size_type capacity;
			typename cache_affinity_hash::size_type size;
			typename cache_affinity_hash::size_type free_count;
			float setting_load_factor;
			offset_type free_list;
			offset_type* bucket;
			index_t* index;
			value_t* value;
		};
		template<class k_t, class v_t>
		struct get_key_select_t
		{
			key_type const& operator()(key_type const& value)
			{
				return value;
			}

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
		class iterator_t
		{
		public:
			typedef std::forward_iterator_tag iterator_category;
			typedef typename cache_affinity_hash::value_type value_type;
			typedef typename cache_affinity_hash::difference_type difference_type;
			typedef typename cache_affinity_hash::reference reference;
			typedef typename cache_affinity_hash::pointer pointer;
		public:
			iterator_t(size_type _offset, cache_affinity_hash const* _self)
				: offset(_offset)
				, self(_self)
			{
			}

			iterator_t(iterator_t const&) = default;

			iterator_t& operator++()
			{
				offset = self->advance_next_(offset);
				return *this;
			}

			iterator_t operator++(int)
			{
				iterator_t save(*this);
				++*this;
				return save;
			}

			reference operator *() const
			{
				return *self->_impl.value[offset].value();
			}

			pointer operator->() const
			{
				return self->_impl.value[offset].value();
			}

			bool operator == (iterator_t const& other) const
			{
				return offset == other.offset && self == other.self;
			}

			bool operator != (iterator_t const& other) const
			{
				return offset != other.offset || self != other.self;
			}
		private:
			friend class cache_affinity_hash;
			size_type offset;
			cache_affinity_hash const* self;
		};
		class const_iterator_t
		{
		public:
			typedef std::forward_iterator_tag iterator_category;
			typedef typename cache_affinity_hash::value_type value_type;
			typedef typename cache_affinity_hash::difference_type difference_type;
			typedef typename cache_affinity_hash::reference reference;
			typedef typename cache_affinity_hash::const_reference const_reference;
			typedef typename cache_affinity_hash::pointer pointer;
			typedef typename cache_affinity_hash::const_pointer const_pointer;
		public:
			const_iterator_t(size_type _offset, cache_affinity_hash const* _self)
				: offset(_offset)
				, self(_self)
			{
			}

			const_iterator_t(const_iterator_t const&) = default;

			const_iterator_t(iterator_t const& it)
				: offset(it.offset)
				, self(it.self)
			{
			}

			const_iterator_t &operator++()
			{
				offset = self->advance_next_(offset);
				return *this;
			}

			const_iterator_t operator++(int)
			{
				const_iterator save(*this);
				++*this;
				return save;
			}

			const_reference operator *() const
			{
				return *self->_impl.value[offset].value();
			}

			const_pointer operator->() const
			{
				return self->_impl.value[offset].value();
			}

			bool operator == (const_iterator_t const& other) const
			{
				return offset == other.offset && self == other.self;
			}

			bool operator != (const_iterator_t const& other) const
			{
				return offset != other.offset || self != other.self;
			}
		private:
			friend class cache_affinity_hash;
			size_type offset;
			cache_affinity_hash const* self;
		};
		class local_iterator_t
		{
		public:
			typedef std::forward_iterator_tag iterator_category;
			typedef typename cache_affinity_hash::value_type value_type;
			typedef typename cache_affinity_hash::difference_type difference_type;
			typedef typename cache_affinity_hash::reference reference;
			typedef typename cache_affinity_hash::pointer pointer;
		public:
			local_iterator_t(size_type _offset, cache_affinity_hash const* _self)
				: offset(_offset)
				, self(_self)
			{
			}

			local_iterator_t(local_iterator_t const&) = default;

			local_iterator_t &operator++()
			{
				offset = self->local_advance_next_(offset);
				return *this;
			}

			local_iterator_t operator++(int)
			{
				local_iterator_t save(*this);
				++*this;
				return save;
			}

			reference operator *() const
			{
				return *self->_impl.value[offset].value();
			}

			pointer operator->() const
			{
				return self->_impl.value[offset].value();
			}

			bool operator == (local_iterator_t const& other) const
			{
				return offset == other.offset && self == other.self;
			}

			bool operator != (local_iterator_t const& other) const
			{
				return offset != other.offset || self != other.self;
			}
		private:
			friend class cache_affinity_hash;
			size_type offset;
			cache_affinity_hash const* self;
		};
		class const_local_iterator_t
		{
		public:
			typedef std::forward_iterator_tag iterator_category;
			typedef typename cache_affinity_hash::value_type value_type;
			typedef typename cache_affinity_hash::difference_type difference_type;
			typedef typename cache_affinity_hash::reference reference;
			typedef typename cache_affinity_hash::const_reference const_reference;
			typedef typename cache_affinity_hash::pointer pointer;
			typedef typename cache_affinity_hash::const_pointer const_pointer;
		public:
			const_local_iterator_t(size_type _offset, cache_affinity_hash const* _self)
				: offset(_offset)
				, self(_self)
			{
			}

			const_local_iterator_t(const_local_iterator_t const&) = default;

			const_local_iterator_t(local_iterator_t const& it)
				: offset(it.offset)
				, self(it.self)
			{
			}

			const_local_iterator_t& operator++()
			{
				offset = self->local_advance_next_(offset);
				return *this;
			}

			const_local_iterator_t operator++(int)
			{
				const_local_iterator save(*this);
				++*this;
				return save;
			}

			const_reference operator *() const
			{
				return *self->_impl.value[offset].value();
			}

			const_pointer operator->() const
			{
				return self->_impl.value[offset].value();
			}

			bool operator == (const_local_iterator_t const& other) const
			{
				return offset == other.offset && self == other.self;
			}

			bool operator != (const_local_iterator_t const& other) const
			{
				return offset != other.offset || self != other.self;
			}
		private:
			friend class cache_affinity_hash;
			size_type offset;
			cache_affinity_hash const* self;
		};
		typedef typename std::conditional<std::is_same<key_type, value_type>::value, const_iterator_t, iterator_t>::type iterator;
		typedef const_iterator_t const_iterator;
		typedef typename std::conditional<std::is_same<key_type, value_type>::value, const_local_iterator_t, local_iterator_t>::type local_iterator;
		typedef const_local_iterator_t const_local_iterator;
		typedef typename std::conditional<params_t::unique_type::value, std::pair<iterator, bool>, iterator>::type insert_result_t;
		typedef std::pair<iterator, bool> pair_ib_t;
	protected:
		typedef std::pair<size_type, bool> pair_posi_t;
		typedef std::pair<iterator, iterator> pair_ii_t;
		typedef std::pair<const_iterator, const_iterator> pair_cici_t;
		typedef std::pair<local_iterator, local_iterator> pair_lili_t;
		typedef std::pair<const_local_iterator, const_local_iterator> pair_clicli_t;

		template<class unique_type>
		typename std::enable_if<unique_type::value, insert_result_t>::type result_(pair_posi_t posi)
		{
			return std::make_pair(iterator(posi.first, this), posi.second);
		}

		template<class unique_type>
		typename std::enable_if<!unique_type::value, insert_result_t>::type result_(pair_posi_t posi)
		{
			return iterator(posi.first, this);
		}

		impl_t _impl;
	public:
		//empty
		cache_affinity_hash()
			: _impl(hasher(), key_equal(), allocator_type())
		{
		}

		//empty
		explicit cache_affinity_hash(size_type bucket_count, hasher const& hash = hasher(), key_equal const& equal = key_equal(), allocator_type const& alloc = allocator_type())
			: _impl(hash, equal, alloc)
		{
			rehash(bucket_count);
		}

		//empty
		explicit cache_affinity_hash(allocator_type const& alloc)
			: _impl(hasher(), key_equal(), alloc)
		{
		}

		//empty
		cache_affinity_hash(size_type bucket_count, allocator_type const& alloc)
			: _impl(hasher(), key_equal(), alloc)
		{
			rehash(bucket_count);
		}

		//empty
		cache_affinity_hash(size_type bucket_count, hasher const& hash, allocator_type const& alloc)
			: _impl(hash, key_equal(), alloc)
		{
			rehash(bucket_count);
		}

		//range
		template <class iterator_t>
		cache_affinity_hash(iterator_t begin, iterator_t end, size_type bucket_count = 8, hasher const& hash = hasher(), key_equal const& equal = key_equal(), allocator_type const& alloc = allocator_type())
			: _impl(hash, equal, alloc)
		{
			rehash(bucket_count);
			insert(begin, end);
		}

		//range
		template <class iterator_t>
		cache_affinity_hash(iterator_t begin, iterator_t end, size_type bucket_count, allocator_type const& alloc)
			: _impl(hasher(), key_equal(), alloc)
		{
			rehash(bucket_count);
			insert(begin, end);
		}

		//range
		template <class iterator_t>
		cache_affinity_hash(iterator_t begin, iterator_t end, size_type bucket_count, hasher const& hash, allocator_type const& alloc)
			: _impl(hash, key_equal(), alloc)
		{
			rehash(bucket_count);
			insert(begin, end);
		}

		//copy
		cache_affinity_hash(cache_affinity_hash const& other)
			: _impl(other.get_hasher(), other.get_key_equal(), other.get_value_allocator_())
		{
			copy_all_<false>(&other._impl);
		}

		//copy
		cache_affinity_hash(cache_affinity_hash const& other, allocator_type const& alloc)
			: _impl(other.get_hasher(), other.get_key_equal(), alloc)
		{
			copy_all_<false>(&other._impl);
		}

		//move
		cache_affinity_hash(cache_affinity_hash&& other)
			: _impl(hasher(), key_equal(), value_allocator_t())
		{
			swap(other);
		}

		//move
		cache_affinity_hash(cache_affinity_hash&& other, allocator_type const& alloc)
			: _impl(std::move(other.get_hasher()), std::move(other.get_key_equal()), alloc)
		{
			copy_all_<true>(&other._impl);
		}

		//initializer list
		cache_affinity_hash(std::initializer_list<value_type> il, size_type bucket_count = 8, hasher const& hash = hasher(), key_equal const& equal = key_equal(), allocator_type const& alloc = allocator_type())
			: cache_affinity_hash(il.begin(), il.end(), std::distance(il.begin(), il.end()), hash, equal, alloc)
		{
		}

		//initializer list
		cache_affinity_hash(std::initializer_list<value_type> il, size_type bucket_count, allocator_type const& alloc)
			: cache_affinity_hash(il.begin(), il.end(), std::distance(il.begin(), il.end()), alloc)
		{
		}

		//initializer list
		cache_affinity_hash(std::initializer_list<value_type> il, size_type bucket_count, hasher const& hash, allocator_type const& alloc)
			: cache_affinity_hash(il.begin(), il.end(), std::distance(il.begin(), il.end()), hash, alloc)
		{
		}

		//destructor
		~cache_affinity_hash()
		{
			dealloc_all_();
		}

		//copy
		cache_affinity_hash& operator = (cache_affinity_hash const& other)
		{
			if (this == &other)
			{
				return *this;
			}
			clear();
			get_hasher() = other.get_hasher();
			get_key_equal() = other.get_key_equal();
			get_bucket_allocator_() = other.get_bucket_allocator_();
			get_index_allocator_() = other.get_index_allocator_();
			get_value_allocator_() = other.get_value_allocator_();
			copy_all_<false>(&other._impl);
			return *this;
		}

		//move
		cache_affinity_hash& operator = (cache_affinity_hash&& other)
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
		cache_affinity_hash& operator = (std::initializer_list<value_type> il)
		{
			clear();
			insert(il.begin(), il.end());
			return *this;
		}

		allocator_type get_allocator() const
		{
			return *static_cast<value_allocator_t const*>(&_impl);
		}

		hasher hash_function() const
		{
			return *static_cast<hasher const*>(&_impl);
		}

		key_equal key_eq() const
		{
			return *static_cast<key_equal const*>(&_impl);
		}

		void swap(cache_affinity_hash& other)
		{
			std::swap(_impl, other._impl);
		}

		//single element
		insert_result_t insert(value_type const& value)
		{
			return result_<typename params_t::unique_type>(insert_value_(value));
		}

		//single element
		template<class in_value_t>
		typename std::enable_if<std::is_convertible<in_value_t, value_type>::value, insert_result_t>::type insert(in_value_t&& value)
		{
			return result_<typename params_t::unique_type>(insert_value_(std::forward<in_value_t>(value)));
		}

		//with hint
		insert_result_t insert(const_iterator hint, value_type const& value)
		{
			return result_<typename params_t::unique_type>(insert_value_(value));
		}

		//with hint
		template<class in_value_t>
		typename std::enable_if<std::is_convertible<in_value_t, value_type>::value, insert_result_t>::type insert(const_iterator hint, in_value_t&& value)
		{
			return result_<typename params_t::unique_type>(insert_value_(std::forward<in_value_t>(value)));
		}

		//range
		template<class iterator_t>
		void insert(iterator_t begin, iterator_t end)
		{
			for (; begin != end; ++begin)
			{
				emplace_hint(cend(), *begin);
			}
		}

		//initializer list
		void insert(std::initializer_list<value_type> il)
		{
			insert(il.begin(), il.end());
		}

		//single element
		template<class ...args_t>
		insert_result_t emplace(args_t&&... args)
		{
			return result_<typename params_t::unique_type>(insert_value_(std::forward<args_t>(args)...));
		}

		//with hint
		template<class ...args_t>
		insert_result_t emplace_hint(const_iterator hint, args_t&&... args)
		{
			return result_<typename params_t::unique_type>(insert_value_(std::forward<args_t>(args)...));
		}

		iterator find(key_type const& key)
		{
			if (_impl.size == 0)
			{
				return end();
			}
			return iterator(find_value_(key), this);
		}

		const_iterator find(key_type const& key) const
		{
			if (_impl.size == 0)
			{
				return cend();
			}
			return const_iterator(find_value_(key), this);
		}

		template<class in_key_t, class = typename std::enable_if<std::is_convertible<in_key_t, key_type>::value && params_t::unique_type::value && !std::is_same<key_type, value_type>::value, void>::type>
		mapped_type& at(in_key_t const& key)
		{
			offset_type offset = _impl.size;
			if (_impl.size != 0)
			{
				offset = find_value_(key);
			}
			if (offset == _impl.size)
			{
				throw std::out_of_range("contiguous_hash out of range");
			}
			return _impl.value[offset].value()->second;
		}

		template<class in_key_t, class = typename std::enable_if<std::is_convertible<in_key_t, key_type>::value && params_t::unique_type::value && !std::is_same<key_type, value_type>::value, void>::type>
		mapped_type const& at(in_key_t const& key) const
		{
			offset_type offset = _impl.size;
			if (_impl.size != 0)
			{
				offset = find_value_(key);
			}
			if (offset == _impl.size)
			{
				throw std::out_of_range("contiguous_hash out of range");
			}
			return _impl.value[offset].value()->second;
		}

		template<class in_key_t, class = typename std::enable_if<std::is_convertible<in_key_t, key_type>::value && params_t::unique_type::value && !std::is_same<key_type, value_type>::value, void>::type>
		mapped_type& operator[](in_key_t&& key)
		{
			offset_type offset = _impl.size;
			if (_impl.size != 0)
			{
				offset = find_value_(key);
			}
			if (offset == _impl.size)
			{
				offset = insert_value_(key, mapped_type()).first;
			}
			return _impl.value[offset].value()->second;
		}

		iterator erase(const_iterator it)
		{
			if (_impl.size == 0 || it.offset == _impl.size)
			{
				return end();
			}
			remove_offset_(it.offset);
			return iterator(advance_next_(it.offset), this);
		}

		local_iterator erase(const_local_iterator it)
		{
			if (_impl.size == 0 || it.offset == _impl.size)
			{
				return local_iterator(offset_empty, this);
			}
			size_type next = local_advance_next_(offset_type(it.offset));
			remove_offset_(it.offset);
			return local_iterator(next, this);
		}

		size_type erase(key_type const& key)
		{
			if (_impl.size == 0)
			{
				return 0;
			}
			return remove_value_(typename params_t::unique_type(), key);
		}

		iterator erase(const_iterator erase_begin, const_iterator erase_end)
		{
			if (erase_begin == cbegin() && erase_end == cend())
			{
				clear();
				return end();
			}
			else
			{
				while (erase_begin != erase_end)
				{
					erase(erase_begin++);
				}
				return iterator(erase_begin.offset, this);
			}
		}

		local_iterator erase(const_local_iterator erase_begin, const_local_iterator erase_end)
		{
			while (erase_begin != erase_end)
			{
				erase(erase_begin++);
			}
			return local_iterator(erase_begin.offset, this);
		}

		size_type count(key_type const& key) const
		{
			if (params_t::unique_type::value)
			{
				return find(key) == end() ? 0 : 1;
			}
			else
			{
				std::conditional<params_t::unique_type::value, pair_cici_t, pair_clicli_t>::type range = equal_range(key);
				return std::distance(range.first, range.second);
			}
		}

		template<class in_key_t, class = typename std::enable_if<std::is_convertible<in_key_t, key_type>::value && params_t::unique_type::value, void>::type>
		pair_ii_t equal_range(in_key_t const& key)
		{
			iterator where = find(key);
			if (where == end())
			{
				return std::make_pair(end(), end());
			}
			else
			{
				return std::make_pair(where, iterator(advance_next_(where.offset), this));
			}
		}

		template<class in_key_t, class = typename std::enable_if<std::is_convertible<in_key_t, key_type>::value && params_t::unique_type::value, void>::type>
		pair_cici_t equal_range(in_key_t const& key) const
		{
			const_iterator where = find(key);
			if (where == cend())
			{
				return std::make_pair(cend(), cend());
			}
			else
			{
				return std::make_pair(where, const_iterator(advance_next_(where.offset), this));
			}
		}

		template<class in_key_t, class = typename std::enable_if<std::is_convertible<in_key_t, key_type>::value && !params_t::unique_type::value, void>::type>
		pair_lili_t equal_range(in_key_t const& key)
		{
			iterator where = find(key);
			if (where == end())
			{
				return std::make_pair(local_iterator(offset_empty, this), local_iterator(offset_empty, this));
			}
			else
			{
				return std::make_pair(local_iterator(where.offset, this), local_iterator(local_find_equal_(where.offset), this));
			}
		}

		template<class in_key_t, class = typename std::enable_if<std::is_convertible<in_key_t, key_type>::value && !params_t::unique_type::value, void>::type>
		pair_clicli_t equal_range(in_key_t const& key) const
		{
			const_iterator where = find(key);
			if (where == end())
			{
				return std::make_pair(const_local_iterator(offset_empty, this), const_local_iterator(offset_empty, this));
			}
			else
			{
				return std::make_pair(const_local_iterator(where.offset, this), const_local_iterator(local_find_equal_(where.offset), this));
			}
		}

		iterator begin()
		{
			return iterator(find_begin_(), this);
		}

		iterator end()
		{
			return iterator(_impl.size, this);
		}

		const_iterator begin() const
		{
			return const_iterator(find_begin_(), this);
		}

		const_iterator end() const
		{
			return const_iterator(_impl.size, this);
		}

		const_iterator cbegin() const
		{
			return const_iterator(find_begin_(), this);
		}

		const_iterator cend() const
		{
			return const_iterator(_impl.size, this);
		}

		bool empty() const
		{
			return _impl.size == _impl.free_count;
		}

		void clear()
		{
			clear_all_();
		}

		size_type size() const
		{
			return _impl.size - _impl.free_count;
		}

		size_type max_size() const
		{
			return offset_empty - 1;
		}

		local_iterator begin(size_type n)
		{
			return local_iterator(_impl.bucket[n], this);
		}

		local_iterator end(size_type n)
		{
			return local_iterator(offset_empty, this);
		}

		const_local_iterator begin(size_type n) const
		{
			return const_local_iterator(_impl.bucket[n], this);
		}

		const_local_iterator end(size_type n) const
		{
			return const_local_iterator(offset_empty, this);
		}

		const_local_iterator cbegin(size_type n) const
		{
			return const_local_iterator(_impl.bucket[n], this);
		}

		const_local_iterator cend(size_type n) const
		{
			return const_local_iterator(offset_empty, this);
		}

		size_type bucket_count() const
		{
			return _impl.bucket_count;
		}

		size_type max_bucket_count() const
		{
			return max_size();
		}

		size_type bucket_size(size_type n) const
		{
			size_type step = 0;
			for (size_type i = _impl.bucket[n]; i != offset_empty; i = _impl.index[i].next)
			{
				++step;
			}
			return step;
		}

		size_type bucket(key_type const& key) const
		{
			if (_impl.size == 0)
			{
				return 0;
			}
			return hash_t(get_hasher()(key)) % _impl.bucket_count;
		}

		void reserve(size_type count)
		{
			rehash(size_type(std::ceil(count / _impl.setting_load_factor)));
		}

		void rehash(size_type count)
		{
			rehash_(typename params_t::unique_type(), std::max<size_type>({ 8, count, size_type(std::ceil(size() / _impl.setting_load_factor)) }));
		}

		void max_load_factor(float ml)
		{
			if (ml <= 0)
			{
				return;
			}
			_impl.setting_load_factor = ml;
			if (_impl.size != 0)
			{
				rehash_(typename params_t::unique_type(), size_type(std::ceil(size() / _impl.setting_load_factor)));
			}
		}

		float max_load_factor() const
		{
			return _impl.setting_load_factor;
		}

		float load_factor() const
		{
			if (_impl.size == 0)
			{
				return 0;
			}
			return size() / float(_impl.bucket_count);
		}

	protected:

		hasher& get_hasher()
		{
			return _impl;
		}

		hasher const& get_hasher() const
		{
			return _impl;
		}

		key_equal& get_key_equal()
		{
			return _impl;
		}

		key_equal const& get_key_equal() const
		{
			return _impl;
		}

		bucket_allocator_t& get_bucket_allocator_()
		{
			return _impl;
		}

		bucket_allocator_t const& get_bucket_allocator_() const
		{
			return _impl;
		}

		index_allocator_t& get_index_allocator_()
		{
			return _impl;
		}

		index_allocator_t const& get_index_allocator_() const
		{
			return _impl;
		}

		value_allocator_t& get_value_allocator_()
		{
			return _impl;
		}

		value_allocator_t const& get_value_allocator_() const
		{
			return _impl;
		}

		size_type advance_next_(size_type i) const
		{
			for (++i; i < _impl.size; ++i)
			{
				if (_impl.index[i].hash)
				{
					break;
				}
			}
			return i;
		}

		size_type local_advance_next_(size_type i) const
		{
			return _impl.index[i].next;
		}

		size_type local_find_equal_(size_type i) const
		{
			hash_t hash = _impl.index[i].hash;
			size_type next = _impl.index[i].next;
			while (next != offset_empty && _impl.index[next].hash == hash &&
				get_key_equal()(get_key_t()(*_impl.value[i].value()), get_key_t()(*_impl.value[next].value())))
			{
				next = _impl.index[next].next;
			}
			return next;
		}

		size_type find_begin_() const
		{
			for (size_type i = 0; i < _impl.size; ++i)
			{
				if (_impl.index[i].hash)
				{
					return i;
				}
			}
			return _impl.size;
		}

		template<class iterator_t, class ...args_t>
		static void construct_one_(iterator_t where, args_t&&... args)
		{
			cache_affinity_hash_detail::construct_one(where, typename cache_affinity_hash_detail::get_tag<iterator_t>::type(), std::forward<args_t>(args)...);
		}

		template<class iterator_t>
		static void destroy_one_(iterator_t where)
		{
			cache_affinity_hash_detail::destroy_one(where, typename cache_affinity_hash_detail::get_tag<iterator_t>::type());
		}

		template<class iterator_from_t, class iterator_to_t>
		static void move_construct_and_destroy_(iterator_from_t move_begin, iterator_from_t move_end, iterator_to_t to_begin)
		{
			cache_affinity_hash_detail::move_construct_and_destroy(move_begin, move_end, to_begin, typename cache_affinity_hash_detail::get_tag<iterator_from_t>::type());
		}

		void dealloc_all_()
		{
			for (size_type i = 0; i < _impl.size; ++i)
			{
				if (_impl.index[i].hash)
				{
					destroy_one_(_impl.value[i].value());
				}
			}
			if (_impl.bucket_count != 0)
			{
				get_bucket_allocator_().deallocate(_impl.bucket, _impl.bucket_count);
			}
			if (_impl.capacity != 0)
			{
				get_index_allocator_().deallocate(_impl.index, _impl.capacity);
				get_value_allocator_().deallocate(_impl.value, _impl.capacity);
			}
		}

		void clear_all_()
		{
			for (size_type i = 0; i < _impl.size; ++i)
			{
				if (_impl.index[i].hash)
				{
					destroy_one_(_impl.value[i].value());
				}
			}
			if (_impl.bucket_count != 0)
			{
				std::memset(_impl.bucket, 0xFFFFFFFF, sizeof(offset_type) * _impl.bucket_count);
			}
			if (_impl.capacity != 0)
			{
				std::memset(_impl.index, 0xFFFFFFFF, sizeof(index_t) * _impl.capacity);
			}
			_impl.size = 0;
			_impl.free_count = 0;
			_impl.free_list = offset_empty;
		}

		template<bool move>
		void copy_all_(impl_t const* other)
		{
			/*root_.capacity = root_.size = other->size - other->free_count;
			root_.setting_load_factor = other->setting_load_factor;
			root_.bucket_count = size_type(std::ceil(root_.size / root_.setting_load_factor));
			root_.free_count = 0;
			root_.free_list = offset_empty;
			root_.bucket = get_bucket_allocator_().allocate(root_.bucket_count);
			root_.index = get_index_allocator_().allocate(root_.capacity);
			root_.value = get_value_allocator_().allocate(root_.capacity);
			std::memset(root_.bucket, 0xFFFFFFFF, sizeof(offset_type) * root_.bucket_count);
			std::memset(root_.index, 0xFFFFFFFF, sizeof(index_t) * root_.capacity);
			for (size_type i = 0, other_i = 0; other_i < other->size; ++other_i)
			{
			if (other->index[other_i].hash)
			{
			size_type bucket = other->index[other_i].hash % root_.capacity;
			if (root_.bucket[bucket] != offset_empty)
			{
			root_.index[root_.bucket[bucket]].prev = offset_type(i);
			}
			root_.index[i].prev = offset_empty;
			root_.index[i].next = root_.bucket[bucket];
			root_.index[i].hash = other->index[other_i].hash;
			root_.bucket[bucket] = offset_type(i);
			if (move)
			{
			construct_one_(root_.value[i].value(), std::move(*other->value[other_i].value()));
			}
			else
			{
			construct_one_(root_.value[i].value(), *other->value[other_i].value());
			}
			++i;
			}
			}*/
			_impl.bucket_count = 0;
			_impl.capacity = 0;
			_impl.size = 0;
			_impl.free_count = 0;
			_impl.free_list = offset_empty;
			_impl.setting_load_factor = other->setting_load_factor;
			_impl.bucket = nullptr;
			_impl.index = nullptr;
			_impl.value = nullptr;
			size_type size = other->size - other->free_count;
			if (size > 0)
			{
				rehash_(std::true_type(), size);
				realloc_(size);
				for (size_type other_i = 0; other_i < other->size; ++other_i)
				{
					if (other->index[other_i].hash)
					{
						size_type i = _impl.size;
						size_type bucket = other->index[other_i].hash % _impl.bucket_count;
						if (_impl.bucket[bucket] != offset_empty)
						{
							_impl.index[_impl.bucket[bucket]].prev = offset_type(i);
						}
						_impl.index[i].prev = offset_empty;
						_impl.index[i].next = _impl.bucket[bucket];
						_impl.index[i].hash = other->index[other_i].hash;
						_impl.bucket[bucket] = offset_type(i);
						if (move)
						{
							construct_one_(_impl.value[i].value(), std::move(*other->value[other_i].value()));
						}
						else
						{
							construct_one_(_impl.value[i].value(), *other->value[other_i].value());
						}
						++_impl.size;
					}
				}
			}
		}

		static bool is_prime_(size_type candidate)
		{
			if ((candidate & 1) != 0)
			{
				size_type limit = size_type(std::sqrt(candidate));
				for (size_type divisor = 3; divisor <= limit; divisor += 2)
				{
					if ((candidate % divisor) == 0)
					{
						return false;
					}
				}
				return true;
			}
			return (candidate == 2);
		}

		static size_type get_prime_(size_type size)
		{
			static size_type const prime_array[] =
			{
				7, 11, 17, 23, 29, 37, 47, 59, 71, 89, 107, 131, 163, 197, 239, 293, 353, 431, 521, 631, 761, 919,
				1103, 1327, 1597, 1931, 2333, 2801, 3371, 4049, 4861, 5839, 7013, 8419, 10103, 12143, 14591,
				17519, 21023, 25229, 30293, 36353, 43627, 52361, 62851, 75431, 90523, 108631, 130363, 156437,
				187751, 225307, 270371, 324449, 389357, 467237, 560689, 672827, 807403, 968897, 1162687, 1395263,
				1674319, 2009191, 2411033, 2893249, 3471899, 4166287, 4999559, 5999471, 7199369
			};
			for (size_type prime : prime_array)
			{
				if (prime >= size)
				{
					return prime;
				}
			}
			for (size_type prime = (size | 1); prime < std::numeric_limits<uint32_t>::max(); prime += 2)
			{
				if (is_prime_(prime) && ((prime - 1) % 101 != 0))
				{
					return prime;
				}
			}
			return size;
		}

		void rehash_(std::true_type, size_type size)
		{
			size = std::min(get_prime_(size), max_size());
			if (_impl.bucket_count != 0)
			{
				get_bucket_allocator_().deallocate(_impl.bucket, _impl.bucket_count);
			}
			_impl.bucket = get_bucket_allocator_().allocate(size);
			std::memset(_impl.bucket, 0xFFFFFFFF, sizeof(offset_type) * size);

			for (size_type i = 0; i < _impl.size; ++i)
			{
				if (_impl.index[i].hash)
				{
					size_type bucket = _impl.index[i].hash % size;
					if (_impl.bucket[bucket] != offset_empty)
					{
						_impl.index[_impl.bucket[bucket]].prev = offset_type(i);
					}
					_impl.index[i].prev = offset_empty;
					_impl.index[i].next = _impl.bucket[bucket];
					_impl.bucket[bucket] = offset_type(i);
				}
			}
			_impl.bucket_count = size;
		}

		void rehash_(std::false_type, size_type size)
		{
			size = std::min(get_prime_(size), max_size());
			offset_type* new_bucket = get_bucket_allocator_().allocate(size);
			std::memset(new_bucket, 0xFFFFFFFF, sizeof(offset_type) * size);

			if (_impl.bucket_count != 0)
			{
				for (size_type i = 0; i < _impl.bucket_count; ++i)
				{
					if (_impl.bucket[i] != offset_empty)
					{
						size_type j = _impl.bucket[i], nj;
						do
						{
							nj = _impl.index[j].next;
							size_type bucket = _impl.index[j].hash % size;
							if (new_bucket[bucket] != offset_empty)
							{
								_impl.index[new_bucket[bucket]].prev = offset_type(j);
							}
							_impl.index[j].prev = offset_empty;
							_impl.index[j].next = new_bucket[bucket];
							new_bucket[bucket] = offset_type(j);
							j = nj;
						} while (j != offset_empty);
					}
				}
				get_bucket_allocator_().deallocate(_impl.bucket, _impl.bucket_count);
			}
			_impl.bucket_count = size;
			_impl.bucket = new_bucket;
		}

		void realloc_(size_type size)
		{
			if (size * sizeof(value_t) > 0x1000)
			{
				size = ((size * sizeof(value_t) + std::max<size_type>(sizeof(value_t), 0x1000) - 1) & (~size_type(0) ^ 0xFFF)) / sizeof(value_t);
			}
			else if (size * sizeof(value_t) > 0x100)
			{
				size = ((size * sizeof(value_t) + std::max<size_type>(sizeof(value_t), 0x100) - 1) & (~size_type(0) ^ 0xFF)) / sizeof(value_t);
			}
			else
			{
				size = ((size * sizeof(value_t) + std::max<size_type>(sizeof(value_t), 0x10) - 1) & (~size_type(0) ^ 0xF)) / sizeof(value_t);
			}
			size = std::min(size, max_size());
			index_t* new_index = get_index_allocator_().allocate(size);
			value_t* new_value = get_value_allocator_().allocate(size);

			std::memset(new_index + _impl.capacity, 0xFFFFFFFF, sizeof(index_t) * (size - _impl.capacity));
			if (_impl.capacity != 0)
			{
				std::memcpy(new_index, _impl.index, sizeof(index_t) * _impl.capacity);
				move_construct_and_destroy_(_impl.value->value(), _impl.value->value() + _impl.capacity, new_value->value());
				get_index_allocator_().deallocate(_impl.index, _impl.capacity);
				get_value_allocator_().deallocate(_impl.value, _impl.capacity);
			}
			_impl.capacity = size;
			_impl.index = new_index;
			_impl.value = new_value;
		}

		void check_grow_()
		{
			size_type new_size = size() + 1;
			if (new_size > _impl.bucket_count * _impl.setting_load_factor)
			{
				if (_impl.bucket_count >= max_size())
				{
					throw std::length_error("contiguous_hash too long");
				}
				rehash_(typename params_t::unique_type(), size_type(std::ceil(_impl.bucket_count * params_t::grow_proportion(_impl.bucket_count))));
			}
			if (new_size > _impl.capacity)
			{
				if (_impl.capacity >= max_size())
				{
					throw std::length_error("contiguous_hash too long");
				}
				realloc_(size_type(std::ceil(std::max<float>(_impl.capacity * params_t::grow_proportion(_impl.capacity), _impl.bucket_count * _impl.setting_load_factor))));
			}
		}

		template<class ...args_t>
		pair_posi_t insert_value_(args_t&&... args)
		{
			check_grow_();
			return insert_value_uncheck_(typename params_t::unique_type(), std::forward<args_t>(args)...);
		}

		// set
		template<class in_t, class ...args_t>
		typename std::enable_if<std::is_same<key_type, value_type>::value && !std::is_same<typename std::remove_reference<in_t>::type, key_type>::value, pair_posi_t>::type
			insert_value_uncheck_(std::true_type, in_t&& in, args_t&&... args)
		{
			key_type key = get_key_t()(in, args...);
			hash_t hash = get_hasher()(key);
			size_type bucket = hash % _impl.bucket_count;
			for (size_type i = _impl.bucket[bucket]; i != offset_empty; i = _impl.index[i].next)
			{
				if (_impl.index[i].hash == hash && get_key_equal()(get_key_t()(*_impl.value[i].value()), get_key_t()(key)))
				{
					return std::make_pair(i, false);
				}
			}
			size_type offset = _impl.free_list == offset_empty ? _impl.size : _impl.free_list;
			construct_one_(_impl.value[offset].value(), std::move(key));
			if (offset == _impl.free_list)
			{
				_impl.free_list = _impl.index[offset].next;
				--_impl.free_count;
			}
			else
			{
				++_impl.size;
			}
			_impl.index[offset].hash = hash;
			_impl.index[offset].next = _impl.bucket[bucket];
			_impl.index[offset].prev = offset_empty;
			if (_impl.bucket[bucket] != offset_empty)
			{
				_impl.index[_impl.bucket[bucket]].prev = offset_type(offset);
			}
			_impl.bucket[bucket] = offset_type(offset);
			return std::make_pair(offset, true);
		}

		// map
		template<class in_t, class ...args_t>
		typename std::enable_if<!std::is_same<key_type, value_type>::value || std::is_same<typename std::remove_reference<in_t>::type, key_type>::value, pair_posi_t>::type
			insert_value_uncheck_(std::true_type, in_t&& in, args_t&&... args)
		{
			hash_t hash = get_hasher()(get_key_t()(in, args...));
			size_type bucket = hash % _impl.bucket_count;
			for (size_type i = _impl.bucket[bucket]; i != offset_empty; i = _impl.index[i].next)
			{
				if (_impl.index[i].hash == hash && get_key_equal()(get_key_t()(*_impl.value[i].value()), get_key_t()(in, args...)))
				{
					return std::make_pair(i, false);
				}
			}
			size_type offset = _impl.free_list == offset_empty ? _impl.size : _impl.free_list;
			construct_one_(_impl.value[offset].value(), std::forward<in_t>(in), std::forward<args_t>(args)...);
			if (offset == _impl.free_list)
			{
				_impl.free_list = _impl.index[offset].next;
				--_impl.free_count;
			}
			else
			{
				++_impl.size;
			}
			_impl.index[offset].hash = hash;
			_impl.index[offset].next = _impl.bucket[bucket];
			_impl.index[offset].prev = offset_empty;
			if (_impl.bucket[bucket] != offset_empty)
			{
				_impl.index[_impl.bucket[bucket]].prev = offset_type(offset);
			}
			_impl.bucket[bucket] = offset_type(offset);
			return std::make_pair(offset, true);
		}

		// multiset/multimap
		template<class in_t, class ...args_t>
		pair_posi_t insert_value_uncheck_(std::false_type, in_t&& in, args_t&&... args)
		{
			size_type offset = _impl.free_list == offset_empty ? _impl.size : _impl.free_list;
			construct_one_(_impl.value[offset].value(), std::forward<in_t>(in), std::forward<args_t>(args)...);
			if (offset == _impl.free_list)
			{
				_impl.free_list = _impl.index[offset].next;
				--_impl.free_count;
			}
			else
			{
				++_impl.size;
			}
			hash_t hash = get_hasher()(get_key_t()(*_impl.value[offset].value()));
			size_type bucket = hash % _impl.bucket_count;
			size_type where;
			for (where = _impl.bucket[bucket]; where != offset_empty; where = _impl.index[where].next)
			{
				if (_impl.index[where].hash == hash && get_key_equal()(get_key_t()(*_impl.value[where].value()), get_key_t()(*_impl.value[offset].value())))
				{
					break;
				}
			}
			_impl.index[offset].hash = hash;
			if (where == offset_empty)
			{
				_impl.index[offset].next = _impl.bucket[bucket];
				_impl.index[offset].prev = offset_empty;
				if (_impl.bucket[bucket] != offset_empty)
				{
					_impl.index[_impl.bucket[bucket]].prev = offset_type(offset);
				}
				_impl.bucket[bucket] = offset_type(offset);
			}
			else
			{
				_impl.index[offset].next = _impl.index[where].next;
				_impl.index[offset].prev = offset_type(where);
				_impl.index[where].next = offset_type(offset);
				if (_impl.index[offset].next != offset_empty)
				{
					_impl.index[_impl.index[offset].next].prev = offset_type(offset);
				}
			}
			return std::make_pair(offset, true);
		}

		size_type find_value_(key_type const& key) const
		{
			hash_t hash = get_hasher()(key);
			size_type bucket = hash % _impl.bucket_count;

			for (size_type i = _impl.bucket[bucket]; i != offset_empty; i = _impl.index[i].next)
			{
				if (_impl.index[i].hash == hash && get_key_equal()(get_key_t()(*_impl.value[i].value()), get_key_t()(key)))
				{
					return i;
				}
			}
			return _impl.size;
		}

		size_type remove_value_(std::true_type, key_type const& key)
		{
			size_type offset = find_value_(key);
			if (offset != _impl.size)
			{
				remove_offset_(offset);
				return 1;
			}
			else
			{
				return 0;
			}
		}

		size_type remove_value_(std::false_type, key_type const& key)
		{
			size_type offset = find_value_(key);
			if (offset != _impl.size)
			{
				hash_t hash = _impl.index[offset].hash;
				size_type count = 1;
				while (true)
				{
					size_type next = _impl.index[offset].next;
					remove_offset_(offset);
					if (next == offset_empty || _impl.index[next].hash != hash || !get_key_equal()(get_key_t()(*_impl.value[next].value()), get_key_t()(key)))
					{
						return count;
					}
					offset = next;
					++count;
				}
			}
			else
			{
				return 0;
			}
		}

		void remove_offset_(size_type offset)
		{
			if (_impl.index[offset].prev != offset_empty)
			{
				_impl.index[_impl.index[offset].prev].next = _impl.index[offset].next;
			}
			else
			{
				_impl.bucket[_impl.index[offset].hash % _impl.bucket_count] = _impl.index[offset].next;
			}
			if (_impl.index[offset].next != offset_empty)
			{
				_impl.index[_impl.index[offset].next].prev = _impl.index[offset].prev;
			}
			_impl.index[offset].next = _impl.free_list;
			_impl.index[offset].hash.clear();
			_impl.free_list = offset_type(offset);
			++_impl.free_count;

			destroy_one_(_impl.value[offset].value());
		}

	};

}
