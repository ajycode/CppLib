#pragma once

namespace Ajy
{

	namespace memory
	{

		struct alloc_malloc
		{
			static size_t remain(void) { return 0; }
			static void clear(void) {}

			static void* alloc(size_t size)
			{
				return (size ? ::malloc(size) : nullptr);
			}
			static void free(void* p)
			{
				if (p != nullptr) ::free(p);
			}
			static void free(void* p, size_t /*size*/) { free(p); }
		};

		struct alloc_new
		{
			static size_t remain(void) { return 0; }
			static void clear(void) {}

			static void* alloc(size_t size)
			{
				return (size ? ::operator new(size, std::nothrow) : nullptr);
			}
			static void free(void* p)
			{
				::operator delete(p, std::nothrow);
			}
			static void free(void* p, size_t /*size*/) { free(p); }
		};

		struct alloc_std
		{
			static size_t remain(void) { return 0; }
			static void clear(void) {}

			static void* alloc(size_t size)
			{
				if (size)
				{
					std::allocator<char> aa;
					try
					{
						return aa.allocate(size);
					}
					catch (const std::bad_alloc&)
					{
						return nullptr;
					}
				}
				else return nullptr;
			}
			static void free(void* p, size_t size)
			{
				std::allocator<char> aa;
				using ptr_t = std::allocator<char>::pointer;
				aa.deallocate(static_cast<ptr_t>(p), size);
			}
		};

		////////////////////////////////////////////////////////////////
		/// Construct an object, just like placement new
		////////////////////////////////////////////////////////////////

		namespace detail_construct
		{
			template <class T>
			struct impl
			{
				template <class... Args>
				static T* construct(T* p, Args&&... args)
				{
					return ::new (p) T(std::forward<Args>(args)...);
				}
			};

			template <class T, size_t N>
			struct impl<T[N]>
			{
				using type = T[N];
				template <class... Args>
				static type* construct(type* p, Args&&... args)
				{
					for (size_t i = 0; i < N; ++i)
						impl<T>::construct((*p) + i, std::forward<Args>(args)...);
					return p;
				}
			};
			template <class T, class... Args>
			T* construct(T* p, Args&&... args)
			{
				return detail_construct::impl<T>::construct(p, std::forward<Args>(args)...);
			}

			template <class T, class... Args>
			T* construct(void* p, Args&&... args)
			{
				return construct(static_cast<T*>(p), std::forward<Args>(args)...);
			}
		}

		////////////////////////////////////////////////////////////////
		/// Destruct an object, but not free the memory
		////////////////////////////////////////////////////////////////

		namespace detail_destruct
		{
			template <class T>
			struct impl
			{
				static void destruct(T* p)
				{
					reinterpret_cast<T*>(p)->~T();
				}
			};

			template <class T, size_t N>
			struct impl<T[N]>
			{
				using type = T[N];
				static void destruct(type* p)
				{
					for (size_t i = 0; i < N; ++i)
						impl<T>::destruct((*p) + i);
				}
			};
			template <class T>
			void destruct(T* p)
			{
				return detail_destruct::impl<T>::destruct(p);
			}

			template <class T>
			void destruct(void* p)
			{
				destruct(static_cast<T*>(p));
			}
		}

		template<class T, class AllocP>
		class allocator_wrapper
		{
			template<class U, class AllocU>
			friend class allocator_wrapper;
		public:
			static_assert(!std::is_const<T>::value,
				"The C++ Standard forbids containers of const elements "
				"because allocator<const T> is ill-formed.");

			typedef T                 value_type;
			typedef value_type*       pointer;
			typedef value_type const* const_pointer;
			typedef value_type&       reference;
			typedef value_type const& const_reference;
			typedef size_t            size_type;
			typedef std::ptrdiff_t	  difference_type;
			typedef AllocP            alloc_policy;
		private:
			alloc_policy alloc_;
		public:
			allocator_wrapper() noexcept {}
			allocator_wrapper(allocator_wrapper<T, AllocP> const& rhs) noexcept
				: alloc_(rhs.alloc_)
			{}

			template <class U>
			allocator_wrapper(allocator_wrapper<U, AllocP> const& rhs) noexcept
				: alloc_(rhs.alloc_)
			{}

			allocator_wrapper(allocator_wrapper<T, AllocP>&& rhs) noexcept
				: alloc_(std::move(rhs.alloc_))
			{}

			template <class U>
			allocator_wrapper(allocator_wrapper<U, AllocP>&& rhs) noexcept
				: alloc_(std::move(rhs.alloc_))
			{}

			allocator_wrapper(AllocP const& rhs) noexcept
				: alloc_(rhs)
			{}

			allocator_wrapper(AllocP&& rhs) noexcept
				: alloc_(std::move(rhs))
			{}

			allocator_wrapper& operator=(allocator_wrapper const& rhs)
			{
				if (this == &rhs)
				{
					return *this;
				}
				alloc_ = rhs.alloc_;
				return *this;
			}

			allocator_wrapper& operator=(allocator_wrapper&& rhs)
			{
				if (this == &rhs)
				{
					return *this;
				}
				alloc_ = std::move(rhs.alloc_);
				return *this;
			}

			template <class U>
			allocator_wrapper& operator=(allocator_wrapper<U, AllocP> const& rhs)
			{
				if (this == &rhs)
				{
					return *this;
				}
				alloc_ = rhs.alloc_;
				return *this;
			}

			template <class U>
			allocator_wrapper& operator=(allocator_wrapper<U, AllocP>&& rhs)
			{
				if (this == &rhs)
				{
					return *this;
				}
				alloc_ = std::move(rhs.alloc_);
				return *this;
			}

		public:
			// the other type of std_allocator
			template <class U>
			struct rebind { typedef allocator_wrapper<U, AllocP> other; };

			size_type max_size() const noexcept
			{
				return (static_cast<size_type>(-1) / sizeof(T));
			}

			pointer allocate(size_type count)
			{
				void* p = nullptr;
				if (count == 0) /* Do nothing */;
				else
					if ((count > this->max_size()) || (p = alloc_.alloc(count * sizeof(T))) == nullptr)
						throw std::bad_alloc(); // Report no memory
				return static_cast<pointer>(p);
			}

			void deallocate(pointer p, size_type count)
			{
				alloc_.free(p, count * sizeof(T));
			}

			template <class... Args>
			static void construct(pointer p, Args&&... args)
			{
				detail_construct::construct<value_type>(p, std::forward<Args>(args)...);
			}

			static void destroy(pointer p)
			{
				detail_destruct::destruct<value_type>(p);
			}
		};

		template <class AllocP>
		class allocator_wrapper<void, AllocP>
		{
		public:
			typedef void    value_type;
			typedef AllocP  alloc_policy;
		};

		template <class T, class U, class AllocP>
		constexpr bool operator==(allocator_wrapper<T, AllocP> const& lhs, allocator_wrapper<U, AllocP> const& rhs) noexcept
		{
			return true;
		}

		template <class T, class U, class AllocP>
		constexpr bool operator!=(allocator_wrapper<T, AllocP> const& lhs, allocator_wrapper<U, AllocP> const& rhs) noexcept
		{
			return false;
		}

		template <class T>
		using allocator = allocator_wrapper<T, alloc_malloc>;
	}

}
