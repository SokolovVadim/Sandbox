#include <iostream>


template<typename T>
class CBaseValue {
public:

	CBaseValue() { init(); }
	virtual ~CBaseValue() {}
	const T& GetValue() const { return value; }
protected:
	virtual T Default() const {return 1;};
private:
	T value;
	void init() { value = Default(); }
};

class CIntValue : public CBaseValue<int> {
public:
	CIntValue() {}
	virtual ~CIntValue() {}
protected:
	virtual int Default() const override { return 42; }
};


int main()
{
	try {
		CIntValue intValue;

		std::cout << intValue.GetValue() << std::endl;
	}
	catch (const std::exception& e) {
	std::cout << e.what() << std::endl;
	}
	return 0;
}


/*
1 да
2 throwing std::exception -> abort
3 метод Default объявлен чисто виртуальным, причем 
в базовом классе его определение не описано
4 написать определение метода default в базовом классе:
virtual T Default() const {return 1;};




*/