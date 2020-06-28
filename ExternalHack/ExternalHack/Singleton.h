#pragma once
template <typename T>
class Singleton
{
protected:
	Singleton() {};
	~Singleton() {};
public:
	static T& get() {
		static T inst{};
		return inst;
	}
};
