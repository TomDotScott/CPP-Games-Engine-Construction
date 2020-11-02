#pragma once

template <typename T>
class Singleton {
public:
    static T& GetInstance() {
        static T instance;
        return instance;
    }
protected:
    Singleton() = default;
    ~Singleton() {}
public:
    Singleton(Singleton const&) = delete;
    Singleton& operator=(Singleton const&) = delete;
};