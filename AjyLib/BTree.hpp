#pragma once

namespace Ajy
{

	template<class config_t> class b_tree;

	template<class key_t, class comparator_t, class allocator_t, class unique_t>
	struct btree_set_config_t
	{
		typedef key_t key_type;
		typedef key_t const mapped_type;
		typedef key_t const value_type;
		typedef key_t storage_type;
		typedef comparator_t key_compare;
		typedef allocator_t allocator_type;
		typedef unique_t unique_type;
		typedef std::false_type status_type;
		template<class in_type>
		static key_type const& get_key(in_type&& value)
		{
			return value;
		}
	};
	template<class value_t, class comparator_t = std::less<value_t>, class allocator_t = std::allocator<value_t>>
	using btree_set = b_tree<btree_set_config_t<value_t, comparator_t, allocator_t, std::true_type>>;
	template<class value_t, class comparator_t = std::less<value_t>, class allocator_t = std::allocator<value_t>>
	using btree_multiset = b_tree<btree_set_config_t<value_t, comparator_t, allocator_t, std::false_type>>;

	template<class key_t, class value_t, class comparator_t, class allocator_t, class unique_t>
	struct btree_map_config_t
	{
		typedef key_t key_type;
		typedef value_t mapped_type;
		typedef std::pair<key_t const, value_t> value_type;
		typedef std::pair<key_t, value_t> storage_type;
		typedef comparator_t key_compare;
		typedef allocator_t allocator_type;
		typedef unique_t unique_type;
		typedef std::false_type status_type;
		template<class in_type>
		static key_type const& get_key(in_type&& value)
		{
			return value.first;
		}
	};
	template<class key_t, class value_t, class comparator_t = std::less<key_t>, class allocator_t = std::allocator<std::pair<key_t const, value_t>>>
	using btree_map = b_tree<btree_map_config_t<key_t, value_t, comparator_t, allocator_t, std::true_type>>;
	template<class key_t, class value_t, class comparator_t = std::less<key_t>, class allocator_t = std::allocator<std::pair<key_t const, value_t>>>
	using btree_multimap = b_tree<btree_map_config_t<key_t, value_t, comparator_t, allocator_t, std::false_type>>;


	namespace b_tree_detail
	{

	}

	template<class config_t>
	class b_tree
	{
	public:
		typedef typename config_t::key_type key_type;
		typedef typename config_t::mapped_type mapped_type;
		typedef typename config_t::value_type value_type;
		typedef typename config_t::storage_type storage_type;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;
		typedef typename config_t::key_compare key_compare;
		typedef typename config_t::allocator_type allocator_type;
		typedef value_type& reference;
		typedef value_type const& const_reference;
		typedef value_type* pointer;
		typedef value_type const* const_pointer;
	};
}
