#ifndef OOP_SINGLETONTEMPLATE_H
#define OOP_SINGLETONTEMPLATE_H

//source : https://github.com/mcmarius/poo/tree/master/tema-3#exemplu-de-singleton-cu-crtp :)
template <typename T>
class Singleton {
    class derived_T_Instance : public T {};
protected:
    Singleton() = default;
    ~Singleton() = default;
public:
    static T& getInstance() {
        static derived_T_Instance instance;
        return instance;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

};


#endif //OOP_SINGLETONTEMPLATE_H
//Usage
// class test : public Singleton<test> {
//     int x;
// protected:
//     test() : x(0) {}
//
// public:
//     int getX() const {return x; }
//     void setX(int i) { x = i; }
// };
//
// //test t1; // da eroare pt constructor non-public
// test &t1 = test::getInstance();
//
// test &t2 = test::getInstance();
// t2.setX(10);
//
// std::cout << t1.getX() << std::endl;