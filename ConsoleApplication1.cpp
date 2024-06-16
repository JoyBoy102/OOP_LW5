#include <iostream>
#include <string>
using namespace std;

class Base {
public:
	int x;
	int y;
public:
	Base() : x(0), y(0) {
		cout << "Base()" << int(this)%1000 << endl;
	}
	Base(const Base* b) :x(b->x),y(b->y) {
		cout << "Base(const Base* d)" << int(this) % 1000 << endl;
	}
	Base(const Base& b) :x(b.x), y(b.y) {
		cout << "Base(const Base& b)" << int(this) % 1000 << endl;
	}
	virtual ~Base() {
		cout << "~Base()" << int(this) % 1000 << endl;
	}
public:
	virtual string classname() {
		return "Base";
	}

};

class Desc :public Base {
public:
	int z;
public:
	Desc() : Base(), z(0) {
		cout << "Desc()" << int(this) % 1000 << endl;
	}
	Desc(const Desc* d): Base(d), z(d->z) {
		cout << "Desc(const Desc* d)" << int(this) % 1000 << endl;
	}
	Desc(const Desc& d) : Base(d), z(d.z) {
		cout << "Desc(const Desc& d)" << int(this) % 1000 << endl;
	}
	~Desc() override {
		cout << "~Desc()" << int(this) % 1000 << endl;
	}
public:
	string classname() override {
		return "Desc";
	}

};


void func1(Base  obj) {
	cout << "func1()" << endl;
};
void func2(Base* obj) {
	cout << "func2()" << endl;
	Desc* d = dynamic_cast<Desc*>(obj);
	if (d != nullptr)
		obj = d;
	cout << obj->classname() << endl;
};
void func3(Base& obj) {
	cout << "func3()" << endl;
	Desc* d = dynamic_cast<Desc*>(&obj);
	if (d != nullptr)
		obj = *d;
	cout << obj.classname() << endl;
};

Base func1() {
	cout << "func1() returns object" << endl;
	Base b;
	return b;
};

Base* func2() {
	cout << "func2() returns ptr on object" << endl;
	Base b;
	return &b;
};

Base& func3() { 
	cout << "func3() returns link on object" << endl;
	Base b;
	return b;
};
Base func4() { 
	cout << "func4() returns object" << endl;
	Base* b = new Base();
	return *b;
};
Base* func5() {
	cout << "func5() returns ptr on object" << endl;
	Base* b = new Base();
	return b;
};
Base& func6() {
	cout << "func6() returns link on object" << endl;
	Base* b = new Base();
	return *b;
};

int main() {
	Base b1;
	Desc d1;
	Base* b2 = new Base();
	Desc* d2 = new Desc();
	cout << "-----------------" << endl;
	func1(b1);
	func2(b2);
	func3(b1);
	cout << "-----------------" << endl;
	func1(d1);
	func2(d2);
	func3(d1);
	cout << "-----------------" << endl;
	
	Base base1;
	base1 = func1(); //Объект создается в main(), затем создается локальный объект в функции, затем конструктором копирования создается временный\
	объект для возврата из функции, уничтожается локальный объект функции, затем результат функции побитово копируется в base1 и уничтожается\
	временный объект.
	Base* base2;
	base2 = func2();
	// delete base2; - Произойдет ошибка, т.к происходит попытка удаления уже несуществующего объекта
	Base& base3 = func3();
	// delete &base3; - Произойдет ошибка, т.к происходит попытка удаления уже несуществующего объекта
	Base base4;
	base4 = func4(); // Утечка памяти, т.к динамически созданный в функции объект нигде не удаляется
	Base* base5;
	base5 = func5(); // В функции создается объект динамически и возвращается указатель на него, тем самым управление объектом передается\
	вызывающему коду и предоставляется возможность удалить его.
	delete base5;
	Base& base6 = func6();
	delete& base6;
	cout << "-----------------" << endl;
	delete d2;
	delete b2;
}