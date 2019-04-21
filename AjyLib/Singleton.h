#pragma once

namespace Ajy
{

#ifdef SINGLETON_DISABLE_AUTO_DESTROY
	template<class T>
	class Singleton
	{
	protected:
		static T* instance;
		Singleton() = default;
		Singleton(Singleton const&) = delete;
		Singleton& operator=(Singleton const&) = delete;
	public:
		static T* GetInstance()
		{
			assert(instance);
			return instance;
		}
		template<class... Args>
		static void InitInstance(Args&&... args)
		{
			assert(!instance);
			instance = new T(std::forward<Args>(args)...);
		}
		static void DestroyInstance()
		{
			assert(instance);
			delete instance;
			instance = nullptr;
		}
	};
	template<class T>
	T* Singleton<T>::instance = nullptr;
#else
	template<class T>
	class Singleton
	{
	protected:
		static std::unique_ptr<T> instance;
		Singleton() = default;
		Singleton(Singleton const&) = delete;
		Singleton& operator=(Singleton const&) = delete;
	public:
		static T* GetInstance()
		{
			assert(instance);
			return instance.get();
		}
		template<class... Args>
		static void InitInstance(Args&&... args)
		{
			assert(!instance);
			instance.reset(new T(std::forward<Args>(args)...));
		}
		static void DestroyInstance()
		{
			assert(instance);
			instance.reset();
		}
	};
	template<class T>
	std::unique_ptr<T> Singleton<T>::instance;
#endif

}
