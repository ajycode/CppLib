#pragma once


namespace Ajy
{

	template<class ReturnType, class... Args>
	class func_base_t
	{
	public:
		virtual func_base_t* clone() = 0;
		virtual ~func_base_t() { }
		virtual ReturnType call(Args&&... args) = 0;
	};

	template<class func, class ReturnType, class... Args>
	class func_t : public func_base_t<ReturnType, Args...>
	{
	public:
		typedef func_base_t<ReturnType, Args...> my_base;
	public:
		func_t(func const& f) :fun(f) { }
		func_t(func&& f) :fun(std::move(f)) { }
		func_t(func_t const& func) :fun(func.fun) { }
		func_t(func_t&& func) :fun(std::move(func.fun)) { }
		my_base* clone()
		{
			return new func_t(*this);
		}
		ReturnType call(Args&&... args)
		{
			return InvokeDetail::invoke<ReturnType>(fun, std::forward<Args>(args)...);
		}
		~func_t() { }
	private:
		func fun;
	};

	template <class ReturnType, class... Args>
	class function;
	template <class ReturnType, class... Args>
	class function<ReturnType(Args...)>
	{
	public:
		function() noexcept : function(nullptr) { }
		function(std::nullptr_t) noexcept : _functor(nullptr) { }
		function(function const& other) { _functor = other._functor->clone(); }
		function(function&& other) noexcept
		{
			_functor = other._functor;
			other._functor = nullptr;
		}

		template<class F>
		function(F f)
		{
			_functor = new func_t<typename std::decay<F>::type, ReturnType, Args...>(f);
		}

		function& operator=(function const& other)
		{
			if (this == &other)
			{
				return *this;
			}
			if (_functor)
			{
				delete _functor;
			}
			_functor = other._functor->clone();
			return *this;
		}
		function& operator=(function&& other) noexcept
		{
			_functor = other._functor;
			other._functor = nullptr;
			return *this;
		}
		function& operator=(std::nullptr_t) noexcept
		{
			if (_functor)
			{
				delete _functor;
			}
			_functor = nullptr;
			return *this;
		}
		template<class F>
		function& operator=(F&& f)
		{
			if (_functor)
			{
				delete _functor;
			}
			_functor = new func_t<typename std::decay<F>::type, ReturnType, Args...>(f);
			return *this;
		}
		template<class F>
		function& operator=(std::reference_wrapper<F> other) noexcept
		{
			if (_functor)
			{
				delete _functor;
			}
			_functor = other._functor->clone();
			return *this;
		}

		~function()
		{
			if (_functor)
			{
				delete _functor;
				_functor = nullptr;
			}
		}

		void swap(function& other) noexcept
		{
			functor* tmp = _functor;
			_functor = other._functor;
			other._functor = tmp;
		}

		explicit operator bool() const noexcept
		{
			return _functor != nullptr;
		}

		ReturnType operator()(Args... args) const
		{
			if (_functor)
			{
				return _functor->call(std::forward<Args>(args)...);
			}
			else
			{
				throw std::exception();
			}
		}

	public:
		typedef func_base_t<ReturnType, Args...> functor;
	private:
		functor* _functor{ nullptr };
	};


}
