#include <iostream>
#include <string>
#include <math.h>
using namespace std;

class Object {
public:
    virtual string classname() = 0;
    virtual bool isA(const string& who) = 0;
};

class Point: public Object {
protected:
    int x;
    int y;
public:
    Point(int x, int y): x(x), y(y) {}
    virtual ~Point() {
        cout << "~Point()" << endl;;
    }
public:
    string classname() override{
        return "Point";
    }
    double get_dist(const Point& p) {
        return sqrt((p.x - x) * (p.x - x) + (p.y - y) * (p.y - y));
    }
    bool isA(const string& who) override {
        if (who == "Point" || who == "Object") return true;
        return false;
    }
    void change_coords(int new_x, int new_y) {
        x = new_x;
        y = new_y;
    }
};

class ColoredPoint : public Point {
private:
    int color;
public:
    ColoredPoint(int x, int y, int color): Point(x, y), color(color){}
    ~ColoredPoint() override {
        cout << "~ColoredPoint()" << endl;
    }
public:
    string classname() override {
        return "ColoredPoint";
    }

    bool isA(const string& who) override {
        if (who == "ColoredPoint" || Point::isA(who)) return true;
        return false;
    }
};

class SizedPoint : public Point {
private:
    int size;
public:
    SizedPoint(int x, int y, int size): Point(x,y), size(size){}
    ~SizedPoint() override {
        cout << "~SizedPoint()" << endl;
    }
public:
    string classname() override {
        return "SizedPoint";
    }
    
    void change_size(int new_size) {
        size = new_size;
    }

    bool isA(const string& who) override {
        if (who == "SizedPoint" || Point::isA(who)) return true;
        return false;
    }

};

class Section : public Object {
private:
    Point* p1;
    Point* p2;
public:
    Section(int x1, int y1, int x2, int y2): p1(new Point(x1,y1)), p2(new Point(x2,y2)){}
    virtual ~Section() {
        cout << "~Section()";
        delete p1;
        delete p2;
    }
public:
    string classname() override {
        return "Section";
    }

    bool isA(const string& who) override {
        if (who == "Section" || who == "Object") return true;
        return false;
    }

    
};

int main()
{   
    Point* p = new ColoredPoint(5, 8, 10);
    ColoredPoint* p1 = new ColoredPoint(3, 6, 15);
    cout << p->classname() << endl;// Вызов перекрываемого метода
    cout << p->Point::classname() << endl; // Вызов метода базового класса
    cout << p1->get_dist(*p) << endl; // Вызов наследуемого метода
    Point* p2 = new SizedPoint(2, 6, 2);

    Object* arr[10];
    for (int i = 0; i < 10; i++) {
        int random_number = rand() % 4;
        switch (random_number) {
        case 0:
            arr[i] = new Point(5, 5);
            break;

        case 1:
            arr[i] = new ColoredPoint(5, 5, 10);
            break;

        case 2:
            arr[i] = new SizedPoint(3, 3, 6);
            break;
        case 3:
            arr[i] = new Section(3, 5, 9, 6);
            break;
        }
    }

    // Опасное приведение типа
    p = static_cast<ColoredPoint*>(p);

    // Безопасные приведения типов (Вручную и с помощью dynamic_cast)
    for (int i = 0; i < 10; i++) {
        // Использование метода classname для проверки класса и приведение типа с помощью static_cast
        // Недостаток: при увеличении кол-ва потомков Point условие будет расширяться
        if (arr[i]->classname() == "Point" || arr[i]->classname() == "SizedPoint" || arr[i]->classname() == "ColoredPoint") 
            static_cast<Point*> (arr[i])->change_coords(100, 100);
    }

    for (int i = 0; i < 10; i++) {
        // Использование метода isA для проверки класса и приведение типа с помощью static_cast
        if (arr[i]->isA("Point")) 
            static_cast<Point*> (arr[i])->change_coords(100, 100);
    }

    for (int i = 0; i < 10; i++) {
        // Использование dynamic_cast для проверки класса и безопасного приведения типа
        Point* p = dynamic_cast<Point*>(arr[i]);
        if (p != nullptr)
            p->change_coords(100, 100);
    }

    delete p2;
    delete p1;
    delete p;
}
