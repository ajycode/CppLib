#pragma once

namespace Ajy
{

	struct Any
	{
		Any() : typeIndex(std::type_index(typeid(void))) {}
		Any(Any const& that) : ptr(that.Clone()), typeIndex(that.typeIndex) {}
		Any(Any&& that) : ptr(std::move(that.ptr)), typeIndex(that.typeIndex) {}

		//创建智能指针时，对于一般的类型，通过std::decay来移除引用和cv符，从而获取原始类型
		template<class T, class = typename std::enable_if<!std::is_same<typename std::decay<T>::type, Any>::value, T>::type>
		Any(T&& value)
			: ptr(new AnyHolderDerived < typename std::decay<T>::type>(std::forward<T>(value)))
			, typeIndex(std::type_index(typeid(typename std::decay<T>::type)))
		{
		}

		bool IsNull() const
		{
			return !bool(ptr);
		}

		template<class T>
		bool IsType() const
		{
			return typeIndex == std::type_index(typeid(T));
		}

		template<class T>
		T& TryGetValue()
		{
			if (!IsType<T>())
			{
#ifdef Ajy_DEBUG
				std::cout << "can not cast " << typeIndex.name() << " to " << std::type_index(typeid(T)).name() << std::endl;
#endif
				throw std::logic_error{ "bad cast" };
			}

			AnyHolderDerived<T>* derived = dynamic_cast<AnyHolderDerived<T>*> (ptr.get());
			return derived->value;
		}

		Any& operator=(Any const& that)
		{
			if (ptr == that.ptr)
				return *this;

			//m_ptr.reset();
			ptr = that.Clone();
			typeIndex = that.typeIndex;
			return *this;
		}

		Any& operator=(Any&& that)
		{
			if (ptr == that.ptr)
				return *this;

			//m_ptr.reset();
			ptr = std::move(that.ptr);
			typeIndex = that.typeIndex;
			return *this;
		}

	private:
		struct AnyHolderBase;
		typedef std::unique_ptr<AnyHolderBase> AnyHolderBasePtr;

		struct AnyHolderBase
		{
			virtual ~AnyHolderBase() {}
			virtual AnyHolderBasePtr Clone() const = 0;
		};

		template<class T>
		struct AnyHolderDerived : AnyHolderBase
		{
			template<class T>
			AnyHolderDerived(T&& val) : value(std::forward<T>(val)) { }

			AnyHolderBasePtr Clone() const
			{
				return AnyHolderBasePtr(new AnyHolderDerived<T>(value));
			}

			T value;
		};

		AnyHolderBasePtr Clone() const
		{
			if (ptr != nullptr)
				return ptr->Clone();

			return nullptr;
		}

		AnyHolderBasePtr ptr;
		std::type_index typeIndex;
	};

}
