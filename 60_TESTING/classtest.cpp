#include <iostream>

class A {
    public:
    int data1;
    A(A &a) : data1(a.data1) {};
    A(int d) : data1(d) {};
    A() : data1(0) {};
};

class B : public A {
    public:
    int data2;
    B(B &a) : A(a), data2(a.data2) {};
    B(int a, int b) : A(a), data2(b) {};
    B() : A(), data2(0) {};
};

static void addAtoArr(A *arr, int indx, A &inst) {
    A cpy(inst);
    arr[indx] = cpy;
}

static void addBtoBrr(A *arr, int indx, B &inst) {
    B cpy(inst);
    arr[indx] = cpy;
}

int main(void) {
    
    A test1{1};
    B test2{2, 3};

    addAtoArr(arr, 0, test1);
    addBtoBrr(arr, 1, test2);
    addAtoArr(arr, 2, test2);

    for(int i = 0; i < sizeof(arr)/sizeof(A); i++) {
        std::cout << "Index " << i << " content " << (static_cast<B>(arr[i])).data2 << " test."
    }
}
