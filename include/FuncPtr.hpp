#ifndef FUNCPTR_HPP
#define FUNCPTR_HPP

template<typename T>
union FuncPtr {
    T func;
    void* voidPtr;
};

// TODO: Make less terrible. I JUST WANT TO CAST IT TO A VOID* GOD DAMMIT
template<class TOut, class TIn>
FuncPtr<TOut> MakeFuncPtr(TIn in)
{
    static_assert(sizeof(TIn) == 4, "FuncPtr input must be of size 4");

    FuncPtr<TIn> ptrIn;
    ptrIn.func = in;

    FuncPtr<TOut> ptrOut;
    ptrOut.voidPtr = ptrIn.voidPtr;

    return ptrOut;
}

#endif
