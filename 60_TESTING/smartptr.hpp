namespace GTC {

template<typename T>
class smart_ptr
{
private:
    T* ptr;

public:
    explicit smart_ptr(T* ptr): ptr(ptr) {}
    ~smart_ptr() {delete (ptr);}
    T& operator*() {return *ptr;}
    T* operator->() {return ptr;}
};

};