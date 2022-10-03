#include "Engine.h"
#include <iostream>
#include <string.h>

GameObject::GameObject() {
    name = { 0 };
    pos = Point2D();
}
GameObject::GameObject(char objName, Point2D objPos) {
    name = &objName;
    pos = objPos;
}
char* GameObject::getname() { return name; }
Point2D GameObject::getpos() { return pos; }

void GameObject::setname(char* objName) {
    name = (char*)malloc((strlen(objName) + 1));
    memcpy(name, objName, strlen(objName) + 1);
}
void GameObject::setpos(Point2D objPos) { pos = objPos; }


int Point2D::getx() { return xPos; }
int Point2D::gety() { return yPos; }

void Point2D::setx(int x) { xPos = x; }
void Point2D::sety(int y) { yPos = y; }

Point2D::Point2D(int x, int y) {
    xPos = x;
    yPos = y;
}

Point2D::Point2D() {
    xPos = 0;
    yPos = 0;
}

Point2D operator+(Point2D A, Point2D B) {
    return Point2D(A.getx() + B.getx(), A.gety() + B.gety());
}
Point2D operator-(Point2D A, Point2D B) {
    return Point2D(A.getx() - B.getx(), A.gety() - B.gety());
}
Point2D operator*(Point2D A, int b) {
    return Point2D(A.getx() * b, A.gety() * b);
}
Point2D operator*(int b, Point2D A) {
    return Point2D(b * A.getx(), b * A.gety());
}
Point2D operator/(Point2D A, int b) {
    return Point2D(A.getx() / b, A.gety() / b);
}
Point2D Point2D::operator+=(Point2D B) {
    return Point2D(xPos += B.getx(), yPos += B.gety());
}
Point2D Point2D::operator-=(Point2D B) {
    return Point2D(xPos -= B.getx(), yPos -= B.gety());
}
Point2D Point2D::operator*=(int b) {
    return Point2D(xPos *= b, yPos *= b);
}
Point2D Point2D::operator/=(int b) {
    return Point2D(xPos /= b, yPos /= b);
}
Point2D Point2D::operator-(Point2D B) {
    return Point2D(xPos = -B.getx(), yPos = -B.gety());
}

bool operator==(Point2D A, Point2D B) {
    return A.getx() == B.getx() && A.gety() == B.gety();
}
bool operator!=(Point2D A, Point2D B) {
    return A.getx() != B.getx() && A.gety() != B.gety();
}
