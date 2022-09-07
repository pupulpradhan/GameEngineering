// MonsterChase.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h> 
#include <string.h>
using namespace std;

class Point2D {
public:
    char name;
    int xPos, yPos;
    int velocity;
    void move(int x, int y) {
        xPos += x * velocity;
        yPos += y * velocity;
    }
    Point2D operator+(const Point2D& B) const {
        Point2D C;
        C.xPos = this->xPos + B.xPos;
        C.yPos = this->yPos + B.yPos;
        return C;
    }
    Point2D operator-(const Point2D& B) const {
        Point2D C;
        C.xPos = this->xPos - B.xPos;
        C.yPos = this->yPos - B.yPos;
        return C;
    }
    Point2D operator*(const int b) const {
        Point2D C;
        C.xPos = this->xPos * b;
        C.yPos = this->yPos * b;
        return C;
    }
    Point2D operator/(const int b) const {
        Point2D C;
        C.xPos = this->xPos / b;
        C.yPos = this->yPos / b;
        return C;
    }
    Point2D operator+=(const Point2D& B) {
        this->xPos = this->xPos + B.xPos;
        this->yPos = this->yPos + B.yPos;
        return *this;
    }
    Point2D operator-=(const Point2D& B) {
        this->xPos = this->xPos + B.xPos;
        this->yPos = this->yPos + B.yPos;
        return *this;
    }
    Point2D operator*=(const int b) {
        this->xPos = this->xPos * b;
        this->yPos = this->yPos * b;
        return *this;
    }
    Point2D operator/=(const int b) {
        this->xPos = this->xPos / b;
        this->yPos = this->yPos / b;
        return *this;
    }

    bool operator==(const Point2D& B) const {
        return this->xPos == B.xPos && this->yPos == B.yPos;
    }
    bool operator!=(const Point2D& B) const {
        return this->xPos != B.xPos || this->yPos != B.yPos;
    }

    Point2D operator-() {
        this->xPos = -this->xPos;
        this->yPos = -this->yPos;
        return *this;
    }
 };


    int main(int argc, char* argv[])
    {
        int monsterCount;
        char input[4096];
        char playerName[20];
        char nameArray[10][10] = { "dracula", "monster1", "gotham", "monster2", "hellboy", "joker", "monster3", "naruto", "monster4", "monster5" };
        int nameArrayPointer = 0;
        int xDir = 0;
        int yDir = 0;
        int xDirPlayer = 0;
        int yDirPlayer = 0;
        char** new_monsterName;
        cout << "Welcome to Monsters Chase!\n";
        cout << "How many Monsters do you want to play with?\n";
        cin >> monsterCount;
        char** monsterName = (char**)malloc(monsterCount * sizeof(char*));
        cout << "\nEnter the name for Player:\t";
        cin >> playerName;
        for (int i = 0; i < monsterCount; i++) {
            cout << "\nEnter the name for Monster " << i << ":\t";
            cin >> input;
            monsterName[i] = (char*)malloc((strlen(input) + 1) * sizeof(char));
            memcpy(monsterName[i], input, strlen(input) + 1);
        }
        /*for (int i = 0; i < monsterCount; i++) {
            cout << monsterName[i];
        }*/
        int value;
        do {
            if (xDir >= 50)
                monsterCount--;
            cout << "Monsters: \n";
            for (int i = 0; i < monsterCount; i++) {
                xDir = xDir == 60 ? 0 : xDir + 2;
                yDir = yDir == 60 ? 0 : yDir + 3;
                cout << monsterName[i] << " is at " << "[ " << xDir << ", " << yDir << "]\n";
            }
            cout << "Player: \n";
            cout << playerName << " is at " << "[ " << xDirPlayer << ", " << yDirPlayer << "]\n";
            cout << "Press A to move left, D to move right, W to move up, S to move down or Q to quit.";
            char inputChar;
            cin >> inputChar;
            value = inputChar;
            if (value == 97) { //a move left
                xDirPlayer = xDirPlayer - 1;
            }
            if (value == 100) { //d move right
                xDirPlayer = xDirPlayer + 1;
            }
            if (value == 115) { //s move down
                yDirPlayer = yDirPlayer - 1;
            }
            if (value == 119) { //w move up
                yDirPlayer = yDirPlayer + 1;
            }
            if (value == 109) { //m add monster
                //add monster
                monsterCount++;
                new_monsterName = (char**)realloc(monsterName, sizeof(char*) * monsterCount);
                if (new_monsterName == NULL) {
                    exit(1);
                }
                else {
                    monsterName = new_monsterName;
                    monsterName[monsterCount - 1] = nameArray[nameArrayPointer];
                    nameArrayPointer = nameArrayPointer == 9 ? 0 : nameArrayPointer + 1;
                }
            }
            if (value == 113) { //q quit
                return 0;
            }
        } while (value != 113);


    }
