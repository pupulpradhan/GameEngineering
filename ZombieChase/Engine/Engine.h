#pragma once

class Point2D {
private:
    char name;
    int xPos;
    int yPos;

public:
    Point2D(int x, int y);
    int getx();
    int gety();

    void setx(int x);
    void sety(int y);
    Point2D operator+=(Point2D& B);
    Point2D operator-=(Point2D& B);
    Point2D operator*=(int b);
    Point2D operator/=(int b);
    Point2D operator-(Point2D& B);

};

Point2D operator+(Point2D& A, Point2D& B);
Point2D operator-(Point2D& A, Point2D& B);
Point2D operator*(Point2D& A, int b);
Point2D operator/(Point2D& A, int b);
Point2D operator*(int b, Point2D& A);
bool operator==(Point2D& A, Point2D& B);
bool operator!=(Point2D& A, Point2D& B);
