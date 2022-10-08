#pragma once

class Point2D {
private:
    int xPos;
    int yPos;

public:
    Point2D(int x, int y);
    Point2D();
    int getx();
    int gety();

    void setx(int x);
    void sety(int y);
    Point2D operator+=(Point2D B);
    Point2D operator-=(Point2D B);
    Point2D operator*=(int b);
    Point2D operator/=(int b);
    Point2D operator-(Point2D B);

};
class GameObject {
private:
    char* name;
    Point2D pos;
public:
    GameObject(char objName, Point2D objPos);
    GameObject();
    Point2D getpos();
    char* getname();

    void setpos(Point2D x);
    void setname(char* objName);
};

Point2D operator+(Point2D A, Point2D B);
Point2D operator-(Point2D A, Point2D B);
Point2D operator*(Point2D A, int b);
Point2D operator/(Point2D A, int b);
Point2D operator*(int b, Point2D A);
bool operator==(Point2D A, Point2D B);
bool operator!=(Point2D A, Point2D B);
