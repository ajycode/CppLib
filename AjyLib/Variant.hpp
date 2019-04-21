#pragma once

namespace Ajy
{

	template<class... Args>
	struct VariantHelper;

	template<class T, class... Args>
	struct VariantHelper<T, Args...> 
	{
		inline static void Destroy(std::type_index const& id, void* val)
		{
			if (id == std::type_index(typeid(T)))
				reinterpret_cast<T*>(val)->~T();
			else
				VariantHelper<Args...>::Destroy(id, val);
		}

		inline static void Move(std::type_index const& id, void* old_val, void* new_val)
		{
			if (id == std::type_index(typeid(T)))
				new (new_val) T(std::move(*reinterpret_cast<T*>(old_val)));
			else
				VariantHelper<Args...>::Move(id, old_val, new_val);
		}

		inline static void Copy(std::type_index const& id, void* old_val, void* new_val)
		{
			if (id == std::type_index(typeid(T)))
				new (new_val) T(*reinterpret_cast<T*>(old_val));
			else
				VariantHelper<Args...>::Copy(id, old_val, new_val);
		}
	};

	template<> 
	struct VariantHelper<> 
	{
		inline static void Destroy(std::type_index const& id, void* val) {}
		inline static void Move(std::type_index const& id, void* old_val, void* new_val) {}
		inline static void Copy(std::type_index const& id, void* old_val, void* new_val) {}
	};

	template<class... Types>
	class Variant
	{
	public:
		Variant():typeIndex(std::type_index(typeid(void)))
		{
		}

		template<class T, class = typename std::enable_if<Ajy::TypeContains<typename std::decay<T>::type, Types...>::value>::type>
		Variant(T&& value): typeIndex(std::type_index(typeid(typename std::decay<T>::type)))
		{
			typedef typename std::decay<T>::type U;
			new (&data) U(std::forward<T>(value));
		}

		Variant(Variant const& copy) :typeIndex(copy.typeIndex)
		{
			Helper_t::Copy(copy.typeIndex, (void*)&copy.data, (void*)&data);
		}

		Variant(Variant&& copy) :typeIndex(copy.typeIndex)
		{
			Helper_t::Move(copy.typeIndex, (void*)&copy.data, (void*)&data);
			copy.typeIndex = std::type_index(typeid(void));
		}

		template<class T, class = typename std::enable_if<Ajy::TypeContains<typename std::decay<T>::type, Types...>::value>::type>
		Variant& operator=(T&& value)
		{
			typedef typename std::decay<T>::type U;
			new (&data) U(std::forward<T>(value));
			typeIndex = std::type_index(typeid(U));
			return *this;
		}

		Variant& operator=(Variant const& copy)
		{
			if (this != &copy)
			{
				Helper_t::Copy(copy.typeIndex, (void*)&copy.data, (void*)&data);
				typeIndex = copy.typeIndex;
			}
			return *this;
		}

		Variant& operator=(Variant&& copy)
		{
			if (this != &copy)
			{
				Helper_t::Move(copy.typeIndex, (void*)&copy.data, (void*)&data);
				typeIndex = copy.typeIndex;
				copy.typeIndex = std::type_index(typeid(void));
			}
			return *this;
		}

		~Variant()
		{
			Clear();
		}

		template<class T>
		bool IsType() const
		{
			return typeIndex == std::type_index(typeid(T));
		}

		bool Empty() const
		{
			return typeIndex == std::type_index(typeid(void));
		}

		std::type_index type() const
		{
			return typeIndex;
		}

		template<class T, class = typename std::enable_if<Ajy::TypeContains<typename std::decay<T>::type, Types...>::value>::type>
		typename std::decay<T>::type& Get()
		{
			using U = typename std::decay<T>::type;
			if (!IsType<U>())
			{
#ifdef Ajy_DEBUG
				std::cout << std::type_index(typeid(U)).name() << " is not defined. " << "current type is " << typeIndex.name() << std::endl;
#endif
				throw std::bad_cast{};
			}
			return *(U*)(&data);
		}

		template<size_t idx>
		typename std::enable_if<idx < sizeof...(Types), typename TypesAt<idx, Types...>::type>::type& Get()
		{
			using U = typename TypesAt<idx, Types...>::type;
			if (!IsType<U>())
			{
#ifdef Ajy_DEBUG
				std::cout << std::type_index(typeid(U)).name() << " is not defined. " << "current type is " << typeIndex.name() << std::endl;
#endif
				throw std::bad_cast{};
			}
			return *(U*)(&data);
		}
		

		void Clear()
		{
			if (typeIndex != std::type_index(typeid(void)))
			{
				Helper_t::Destroy(typeIndex, &data);
				typeIndex = std::type_index(typeid(void));
			}
		}
	private:
		typedef VariantHelper<Types...> Helper_t;
		typedef typename std::aligned_storage<Ajy::MaxSizeOf<Types...>::value, Ajy::MaxAlignOf<Types...>::value>::type dataType_t;
		dataType_t data;
		std::type_index typeIndex;
	};

}
