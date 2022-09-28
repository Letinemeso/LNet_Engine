#ifndef FUNCTION_WRAPPER_H
#define FUNCTION_WRAPPER_H


namespace LNet
{

	template<typename Func_Return_Type, typename... Func_Args>
	class Function_Wrapper
	{
	public:
		Function_Wrapper() { }
		virtual ~Function_Wrapper() { }

	public:
		virtual void operator()(Func_Args... _args) const = 0;
	};



	template<typename Func_Return_Type, typename... Func_Args>
	class Global_Function_Wrapper : public Function_Wrapper<Func_Return_Type, Func_Args...>
	{
	private:
		using Function_Pointer_Type = Func_Return_Type(*)(Func_Args...);

	private:
		Function_Pointer_Type m_func_ptr = nullptr;

	public:
		static Function_Wrapper<Func_Return_Type, Func_Args...>* bind(Function_Pointer_Type _func_ptr);

	private:
		Global_Function_Wrapper(){ };

	public:
		Global_Function_Wrapper(const Global_Function_Wrapper<Func_Return_Type, Func_Args...>& _other);
		Global_Function_Wrapper(Global_Function_Wrapper<Func_Return_Type, Func_Args...>&& _other);
		void operator=(const Global_Function_Wrapper<Func_Return_Type, Func_Args...>& _other);
		void operator=(Global_Function_Wrapper<Func_Return_Type, Func_Args...>&& _other);
		void operator()(Func_Args... _args) const override;

	};


	template<typename Func_Return_Type, typename... Func_Args>
	Global_Function_Wrapper<Func_Return_Type, Func_Args...>::Global_Function_Wrapper(const Global_Function_Wrapper<Func_Return_Type, Func_Args...>& _other)
	{
		m_func_ptr = _other.m_func_ptr;
	}

	template<typename Func_Return_Type, typename... Func_Args>
	Global_Function_Wrapper<Func_Return_Type, Func_Args...>::Global_Function_Wrapper(Global_Function_Wrapper<Func_Return_Type, Func_Args...>&& _other)
	{
		m_func_ptr = _other.m_func_ptr;
		_other.m_func_ptr = nullptr;
	}

	template<typename Func_Return_Type, typename... Func_Args>
	void Global_Function_Wrapper<Func_Return_Type, Func_Args...>::operator=(const Global_Function_Wrapper<Func_Return_Type, Func_Args...>& _other)
	{
		m_func_ptr = _other.m_func_ptr;
	}

	template<typename Func_Return_Type, typename... Func_Args>
	void Global_Function_Wrapper<Func_Return_Type, Func_Args...>::operator=(Global_Function_Wrapper<Func_Return_Type, Func_Args...>&& _other)
	{
		m_func_ptr = _other.m_func_ptr;
		_other.m_func_ptr = nullptr;
	}


	template<typename Func_Return_Type, typename... Func_Args>
	void Global_Function_Wrapper<Func_Return_Type, Func_Args...>::operator()(Func_Args... _args) const
	{
		if(m_func_ptr)
			m_func_ptr(_args...);
	}


	template<typename Object_Type, typename Func_Return_Type, typename... Func_Args>
	class Member_Function_Wrapper : public Function_Wrapper<Func_Return_Type, Func_Args...>
	{
	private:
		using Function_Pointer_Type = Func_Return_Type(Object_Type::*)(Func_Args...);

	private:
		Function_Pointer_Type m_func_ptr = nullptr;
		Object_Type* m_object = nullptr;

	public:
		static Function_Wrapper<Func_Return_Type, Func_Args...>* bind(Object_Type* _object, Function_Pointer_Type _func_ptr);

	private:
		Member_Function_Wrapper(){ };

	public:
		Member_Function_Wrapper(const Member_Function_Wrapper<Object_Type, Func_Return_Type, Func_Args...>& _other);
		Member_Function_Wrapper(Member_Function_Wrapper<Object_Type, Func_Return_Type, Func_Args...>&& _other);
		void operator=(const Member_Function_Wrapper<Object_Type, Func_Return_Type, Func_Args...>& _other);
		void operator=(Member_Function_Wrapper<Object_Type, Func_Return_Type, Func_Args...>&& _other);
		void operator()(Func_Args... _args) const override;

	};


	template<typename Object_Type, typename Func_Return_Type, typename... Func_Args>
	Member_Function_Wrapper<Object_Type, Func_Return_Type, Func_Args...>::Member_Function_Wrapper(const Member_Function_Wrapper<Object_Type, Func_Return_Type, Func_Args...>& _other)
	{
		m_func_ptr = _other.m_func_ptr;
		m_object = _other.m_object;
	}

	template<typename Object_Type, typename Func_Return_Type, typename... Func_Args>
	Member_Function_Wrapper<Object_Type, Func_Return_Type, Func_Args...>::Member_Function_Wrapper(Member_Function_Wrapper<Object_Type, Func_Return_Type, Func_Args...>&& _other)
	{
		m_func_ptr = _other.m_func_ptr;
		m_object = _other.m_object;
		_other.m_func_ptr = nullptr;
		_other.m_object = nullptr;
	}

	template<typename Object_Type, typename Func_Return_Type, typename... Func_Args>
	void Member_Function_Wrapper<Object_Type, Func_Return_Type, Func_Args...>::operator=(const Member_Function_Wrapper<Object_Type, Func_Return_Type, Func_Args...>& _other)
	{
		m_func_ptr = _other.m_func_ptr;
		m_object = _other.m_object;
	}

	template<typename Object_Type, typename Func_Return_Type, typename... Func_Args>
	void Member_Function_Wrapper<Object_Type, Func_Return_Type, Func_Args...>::operator=(Member_Function_Wrapper<Object_Type, Func_Return_Type, Func_Args...>&& _other)
	{
		m_func_ptr = _other.m_func_ptr;
		m_object = _other.m_object;
		_other.m_func_ptr = nullptr;
		_other.m_object = nullptr;
	}


	template<typename Object_Type, typename Func_Return_Type, typename... Func_Args>
	void Member_Function_Wrapper<Object_Type, Func_Return_Type, Func_Args...>::operator()(Func_Args... _args) const
	{
		if(m_func_ptr && m_object)
			(m_object->*m_func_ptr)(_args...);
	}


	template<typename Object_Type, typename Func_Return_Type, typename... Func_Args>
	Function_Wrapper<Func_Return_Type, Func_Args...>* Member_Function_Wrapper<Object_Type, Func_Return_Type, Func_Args...>::bind(Object_Type* _object, Func_Return_Type(Object_Type::*_func_ptr)(Func_Args...))
	{
		Member_Function_Wrapper<Object_Type, Func_Return_Type, Func_Args...>* result = new Member_Function_Wrapper<Object_Type, Func_Return_Type, Func_Args...>;
		result->m_object = _object;
		result->m_func_ptr = _func_ptr;
		return result;
	}

	template<typename Func_Return_Type, typename... Func_Args>
	Function_Wrapper<Func_Return_Type, Func_Args...>* Global_Function_Wrapper<Func_Return_Type, Func_Args...>::bind(Func_Return_Type(*_func_ptr)(Func_Args...))
	{
		Global_Function_Wrapper<Func_Return_Type, Func_Args...>* result = new Global_Function_Wrapper<Func_Return_Type, Func_Args...>;
		result->m_func_ptr = _func_ptr;
		return result;
	}



	//	to use Function as intended, use this class and not ones above (you can though)

	template<typename Func_Return_Type, typename... Func_Args>
	class Function final
	{
	private:
		using Function_Wrapper_Ptr = Function_Wrapper<Func_Return_Type, Func_Args...>*;

	private:
		Function_Wrapper_Ptr m_function = nullptr;

	public:
		Function();
		Function(Function_Wrapper_Ptr _raw_func_wrapper);
		void operator=(Function_Wrapper_Ptr _raw_func_wrapper);

		Function(const Function& _other);
		Function(Function&& _other);
		void operator=(const Function& _other);
		void operator=(Function&& _other);

		~Function();

	public:
		void operator()(Func_Args... _args) const;

	};


	template<typename Func_Return_Type, typename... Func_Args>
	Function<Func_Return_Type, Func_Args...>::Function()
	{

	}

	template<typename Func_Return_Type, typename... Func_Args>
	Function<Func_Return_Type, Func_Args...>::Function(Function::Function_Wrapper_Ptr _raw_func_wrapper)
	{
		m_function = _raw_func_wrapper;
	}

	template<typename Func_Return_Type, typename... Func_Args>
	void Function<Func_Return_Type, Func_Args...>::operator=(Function::Function_Wrapper_Ptr _raw_func_wrapper)
	{
		delete m_function;
		m_function = _raw_func_wrapper;
	}


	template<typename Func_Return_Type, typename... Func_Args>
	Function<Func_Return_Type, Func_Args...>::Function(const Function& _other)
	{
		m_function = _other.m_function;
	}

	template<typename Func_Return_Type, typename... Func_Args>
	Function<Func_Return_Type, Func_Args...>::Function(Function&& _other)
	{
		m_function = _other.m_function;
		_other.m_function = nullptr;
	}

	template<typename Func_Return_Type, typename... Func_Args>
	void Function<Func_Return_Type, Func_Args...>::operator=(const Function& _other)
	{
		m_function = _other.m_function;
	}

	template<typename Func_Return_Type, typename... Func_Args>
	void Function<Func_Return_Type, Func_Args...>::operator=(Function&& _other)
	{
		m_function = _other.m_function;
		_other.m_function = nullptr;
	}


	template<typename Func_Return_Type, typename... Func_Args>
	Function<Func_Return_Type, Func_Args...>::~Function()
	{
		delete m_function;
	}


	template<typename Func_Return_Type, typename... Func_Args>
	void Function<Func_Return_Type, Func_Args...>::operator()(Func_Args... _args) const
	{
		(*m_function)(_args...);
	}

}

#endif // FUNCTION_WRAPPER_H
