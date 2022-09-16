#ifndef FUNCTION_WRAPPER_H
#define FUNCTION_WRAPPER_H


namespace LNet
{

//	class


	template<typename Func_Type>
	class Function_Wrapper
	{
	private:
		Func_Type m_func = nullptr;

		void(*m_call_func_fptr)(void* _obj, Func_Type _func) = nullptr;

	private:
		Function_Wrapper();

	public:
		Function_Wrapper(Function_Wrapper&& _other);

	public:
		operator Func_Type();

	};


	template<typename Func_Type>
	Function_Wrapper<Func_Type>::Function_Wrapper(Function_Wrapper&& _other)
	{
		m_func = _other.m_func;
		_other.m_func = nullptr;

//		m_call_func_fptr = [](void* _obj, Func_Type _func)
//		{
//			if(_obj)
//				_obj->_func();
//		}
		m_call_func_fptr = _other.m_call_func_fptr;
		_other.m_call_func_fptr = nullptr;
	}

}

#endif // FUNCTION_WRAPPER_H
