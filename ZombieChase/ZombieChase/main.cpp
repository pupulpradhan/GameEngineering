// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string.h>
#include "Engine.h"
using namespace std;


int main()
{
    int monsterCount;
    GameObject Player;
    char* input = new char[10];
    char* playerName = new char[10];
    char nameArray[10][10] = { "dracula", "monster1", "gotham", "monster2", "hellboy", "joker", "monster3", "naruto", "monster4", "monster5" };
    int nameArrayPointer = 0;
    cout << "Welcome to Monsters Chase!\n";
    cout << "How many Monsters do you want to play with?\n";
    cin >> monsterCount;
    GameObject* Monsters = (GameObject*)malloc(sizeof(GameObject) * monsterCount);
    cout << "\nEnter the name for Player:\t";
    cin >> playerName;
    Player.setname(playerName);
    Player.setpos(Point2D(0, 0));
    cout << "Player name is : " << Player.getname();

    for (int i = 0; i < monsterCount; i++) {
        cout << "\nEnter the name for Monster " << i << ":\t";
        cin >> input;
        Monsters[i].setname(input);
        Monsters[i].setpos(Point2D(i, i + 2));
    }
    int value;
    do {
        if (Monsters[0].getpos().getx() >= 30) {
            Monsters[0].setpos(Point2D(0, 0));
            monsterCount--;
        }
        cout << "Monsters: \n";
        for (int i = 0; i < monsterCount; i++) {
            cout << Monsters[i].getname() << " is at " << "[ " << Monsters[i].getpos().getx() << ", " << Monsters[i].getpos().gety() << "]\n";
            Monsters[i].setpos(Monsters[i].getpos() + Point2D(i + 3 * 2, i + 5 * 2));
        }
        cout << "Player: \n";
        cout << Player.getname() << " is at " << "[ " << Player.getpos().getx() << ", " << Player.getpos().gety() << "]\n";
        cout << "Press A to move left, D to move right, W to move up, S to move down or Q to quit.";
        char inputChar;
        cin >> inputChar;
        value = inputChar;
        if (value == 97) { //a move left
            Player.setpos((Player.getpos() -= Point2D(1, 0)));
        }
        if (value == 100) { //d move right
            Player.setpos(Player.getpos() += Point2D(1, 0));
        }
        if (value == 115) { //s move down
            Player.setpos(Player.getpos() -= Point2D(0, 1));
        }
        if (value == 119) { //w move up
            Player.setpos(Player.getpos() += Point2D(0, 1));
        }
        if (value == 109) { //m add monster
            //add monster
            monsterCount++;
            GameObject* Monsters_new = (GameObject*)realloc(Monsters, sizeof(GameObject) * monsterCount);
            if (Monsters_new == NULL) {
                exit(1);
            }
            else {
                Monsters = Monsters_new;
                Monsters[monsterCount-1].setname(nameArray[nameArrayPointer]);
                nameArrayPointer = nameArrayPointer == 9 ? 0 : nameArrayPointer + 1;
                Monsters[monsterCount-1].setpos(Point2D(0, 0));
            }
        }
        if (value == 113) { //q quit
            for (int i = 0; i < monsterCount; i++) {
                free(Monsters[i].name);
            }
            free(Player.name);
            free(Monsters);
            delete[] playerName;
            delete[] input;
            
            _CrtDumpMemoryLeaks();
            return 0;
        }
    } while (value != 113);
 
}
