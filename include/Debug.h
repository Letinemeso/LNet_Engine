#ifndef DEBUG_FILE
#define DEBUG_FILE

#ifdef LNET_DEBUG
	#undef LNET_DEBUG
#endif
#define LNET_DEBUG true

#ifdef LNET_DEBUG
	#if LNET_DEBUG == true

		#include <iostream>
		#include <map>

		//	USE THIS ONLY WITH MACRO BELOW
		namespace ___LNetDebug
		{
			void ___debug_create_log_level(const std::string& _level_name);
			void ___debug_remove_log_level(const std::string& _level_name);
			void ___debug_log(const std::string& _level_name, const std::string& _message);
		}

		#ifdef LNET_LOG
			#undef LNET_LOG
		#endif
		#define LNET_LOG(level, message) ___LNetDebug::___debug_log(level, message)

		#ifdef LNET_CREATE_LOG_LEVEL
			#undef LNET_CREATE_LOG_LEVEL
		#endif
		#define LNET_CREATE_LOG_LEVEL(level) ___LNetDebug::___debug_create_log_level(level)

		#ifdef LNET_REMOVE_LOG_LEVEL
			#undef LNET_REMOVE_LOG_LEVEL
		#endif
		#define LNET_REMOVE_LOG_LEVEL(level) ___LNetDebug::___debug_remove_log_level(level)

		#ifdef LNET_DEBUG_FUNC_NOARG
			#undef LNET_DEBUG_FUNC_NOARG
		#endif
		#define LNET_DEBUG_FUNC_NOARG(func) func()

		#ifdef LNET_DEBUG_FUNC_1ARG
			#undef LNET_DEBUG_FUNC_1ARG
		#endif
		#define LNET_DEBUG_FUNC_1ARG(func, arg) func(arg)

		#ifdef LNET_DEBUG_FUNC_2ARG
			#undef LNET_DEBUG_FUNC_2ARG
		#endif
		#define LNET_DEBUG_FUNC_2ARG(func, arg1, arg2) func(arg1, arg2)

		#ifdef LNET_ASSERT
			#undef LNET_ASSERT
		#endif
		#define LNET_ASSERT(condition) if(!condition) { int a = 1; a /= 0; } 1 == 1


	#else	//LNET_DEBUG == true

		#ifdef LNET_LOG
			#undef LNET_LOG
		#endif
		#define LNET_LOG(level, message)

		#ifdef LNET_CREATE_LOG_LEVEL
			#undef LNET_CREATE_LOG_LEVEL
		#endif
		#define LNET_CREATE_LOG_LEVEL(level)

		#ifdef LNET_REMOVE_LOG_LEVEL
			#undef LNET_REMOVE_LOG_LEVEL
		#endif
		#define LNET_REMOVE_LOG_LEVEL(level)

		#ifdef LNET_DEBUG_FUNC_NOARG
			#undef LNET_DEBUG_FUNC_NOARG
		#endif
		#define LNET_DEBUG_FUNC_NOARG(func)

		#ifdef LNET_DEBUG_FUNC_1ARG
			#undef LNET_DEBUG_FUNC_1ARG
		#endif
		#define LNET_DEBUG_FUNC_1ARG(func, arg)

		#ifdef LNET_DEBUG_FUNC_2ARG
			#undef LNET_DEBUG_FUNC_2ARG
		#endif
		#define LNET_DEBUG_FUNC_2ARG(func, arg1, arg2)

		#ifdef LNET_ASSERT
			#undef LNET_ASSERT
		#endif
		#define LNET_ASSERT(condition)

	#endif	//LNET_DEBUG == true
#endif	//LNET_DEBUG

#endif	//DEBUG_FILE
