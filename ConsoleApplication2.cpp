#include <iostream>
#include <memory>
using namespace std;

class Point {
private:
    int x;
    int y;
public:
    Point():x(0),y(0){
        cout << "Point()" << int(this) % 1000 << endl;
    }
    Point(int x, int y):x(x), y(y){
        cout << "Point(int x, int y)" << int(this) % 1000 << endl;
    }
    Point(const Point& p):x(p.x), y(p.y){
        cout << "Point(const Point& p)" << int(this) % 1000 << endl;
    }
    ~Point() {
        cout << "~Point()" << int(this)%1000 << endl;
    }
public:
    void change_coords(int new_x, int new_y) {
        x = new_x;
        y = new_y;
    }
    void get_coords() {
        cout << x << " " << y << endl;
    }
};

void change_coords_object_delete_uniq_ptr (unique_ptr<Point> p) {
    p->change_coords(40, 40);
}

unique_ptr<Point> change_coords_uniq_ptr(unique_ptr<Point> p) {
    p->change_coords(10, 10);
    return p;
}

shared_ptr<Point> change_coords_shared_ptr(shared_ptr<Point> p) {
    p->change_coords(20, 20);
    return p;
}

int main()
{
    unique_ptr<Point>p0 = make_unique<Point>(0, 0);
    change_coords_object_delete_uniq_ptr(move(p0));
    //p0->get_coords(); - Ошибка, т.к объект удалился при завершении функции change_coords_object_delete();

    unique_ptr<Point>p = make_unique<Point>(4, 4);
    p = change_coords_uniq_ptr(move(p));//Нужно прописывать move(), т. к при передаче указателя в функцию, создается ее копия на тот же объект,\
    что противоречит сути unique_ptr. move() здесь передает контроль за время жизни объекта функции
    p->get_coords();


    shared_ptr<Point>p1 = make_shared<Point>(30, 30);
    p1 = change_coords_shared_ptr(p1);//Прописывать move() здесь не нужно, т.к при передаче указателя функции, счетчик ссылок на обьект просто увеличивается\
    на 2 и при завершении функции уменьшается на 1, т.к локальный объект-указатель удаляется
    p1->get_coords();
    {
        shared_ptr<Point>p2 = p1;
    }
    //После выхода из блока кода удалится только p2, но останется p1. Это озночает, что объект еще не удалился
    p1->get_coords();
    // p2->get_coords() - ошибка

}

