#ifndef __POINTER_WRAPPER
#define __POINTER_WRAPPER


namespace LNet
{

	template<typename Type>
	class Pointer_Wrapper
	{
	private:
		Type* m_raw_ptr = nullptr;

		using free_func_t = void(*)(Type* _data_to_free);
		free_func_t m_free_func = nullptr;

	private:
		Pointer_Wrapper(const Pointer_Wrapper& _other) = delete;
		void operator=(const Pointer_Wrapper& _other) = delete;

	public:
		Pointer_Wrapper(Type* _data);
		Pointer_Wrapper(Type* _data, free_func_t _free_func);
		Pointer_Wrapper(Pointer_Wrapper&& _other);
		~Pointer_Wrapper();

	public:
		Type& operator->();
		const Type& operator->() const;
		Type* extract_pointer();
		void free();

	};


	template<typename Type>
	Pointer_Wrapper<Type>::Pointer_Wrapper(Type* _data)
	{
		m_raw_ptr = _data;
	}

	template<typename Type>
	Pointer_Wrapper<Type>::Pointer_Wrapper(Type* _data, free_func_t _free_func)
	{
		m_raw_ptr = _data;
		m_free_func = _free_func;
	}

	template<typename Type>
	Pointer_Wrapper<Type>::Pointer_Wrapper(Pointer_Wrapper&& _other)
	{
		m_raw_ptr = _other.m_raw_ptr;
		m_free_func = _other.m_free_func;
		_other.m_raw_ptr = nullptr;
		_other.m_free_func = nullptr;
	}

	template<typename Type>
	Pointer_Wrapper<Type>::~Pointer_Wrapper()
	{
		free();
	}



	template<typename Type>
	Type& Pointer_Wrapper<Type>::operator->()
	{
		return *m_raw_ptr;
	}

	template<typename Type>
	const Type& Pointer_Wrapper<Type>::operator->() const
	{
		return *m_raw_ptr;
	}

	template<typename Type>
	Type* Pointer_Wrapper<Type>::extract_pointer()
	{
		Type* ptr = m_raw_ptr;
		m_raw_ptr = nullptr;
		m_free_func = nullptr;
		return ptr;
	}

	template<typename Type>
	void Pointer_Wrapper<Type>::free()
	{
		if(m_free_func != nullptr)
			m_free_func(m_raw_ptr);
		else
			delete m_raw_ptr;
		m_raw_ptr = nullptr;
		m_free_func = nullptr;
	}



}


#endif // __POINTER_WRAPPER
