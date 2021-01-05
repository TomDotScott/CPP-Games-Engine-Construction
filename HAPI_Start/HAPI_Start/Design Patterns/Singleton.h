#pragma once

template <typename T>
class Singleton
{
public:
	static T& GetInstance()
	{
		static T instance{};
		return instance;
	}

	Singleton(const Singleton&)                      = delete;
	Singleton& operator=(const Singleton&)           = delete;
	Singleton(Singleton&& other) noexcept            = delete;
	Singleton& operator=(Singleton&& other) noexcept = delete;

protected:
	Singleton()  = default;
	~Singleton() = default;
};
