#ifndef FUNCPTR_HPP
#define FUNCPTR_HPP

template<typename T>
union FuncPtr {
	T func;
	void* voidPtr;
};

#endif
