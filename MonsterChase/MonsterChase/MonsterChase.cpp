// MonsterChase.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h> 
#include <string.h>
using namespace std;

struct Monster
{
    char name[20];
    int xCoord;
    int yCoord;
};

int main(int argc, char* argv[])
{
    int monsterCount;
    char input[4096];
    char playerName[20];
    char nameArray[10][10] = {"dracula", "monster1", "gotham", "monster2", "hellboy", "joker", "monster3", "naruto", "monster4", "monster5"};
    int nameArrayPointer = 0;
    int xDir = 0;
    int yDir = 0;
    int xDirPlayer = 0;
    int yDirPlayer = 0;
    cout << "Welcome to Monsters Chase!\n";
    cout << "How many Monsters do you want to play with?\n";
    cin >> monsterCount;
    char** monsterName = new char *[monsterCount];
    /*char** monsterName1 = malloc(monsterCount * sizeof(char));*/
    /*for (int i = 0; i < monsterCount; i++)
        monsterName[i] = new char[10];*/
    cout << "\nEnter the name for Player:\t";
    cin >> playerName;
    for (int i = 0; i < monsterCount; i++) {
        cout << "\nEnter the name for Monster " << i << ":\t";
        cin >> input;
        monsterName[i] = new char[strlen(input) + 1];
        memcpy(monsterName[i], input, strlen(input) + 1);
    }
    for (int i = 0; i < monsterCount; i++) 
        cout << monsterName[i] << "\n";
    int value;
    do {
        if (monsterCount >= 5)
            monsterCount--;
        cout << "Monsters: \n";
        for (int i = 0; i < monsterCount; i++) {
            xDir = xDir + 2;
            yDir = yDir + 3;
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
            char** new_monsterName = new char* [monsterCount];
            memcpy(new_monsterName, monsterName, monsterCount);
            delete[] monsterName;
            char** monsterName = new char* [monsterCount];
            memcpy(monsterName, new_monsterName, monsterCount);
            delete[] new_monsterName;
            monsterName[monsterCount - 1] = nameArray[nameArrayPointer];
            nameArrayPointer = nameArrayPointer == 9 ? 0 : nameArrayPointer++;
        }
        if (value == 113) { //q quit
            //Delete the array created
            for (int i = 0; i < monsterCount; ++i)    //To delete the inner arrays
                delete monsterName[i];
            delete[] monsterName;
            
            return 0;
        }
    } while (value != 113);


}
