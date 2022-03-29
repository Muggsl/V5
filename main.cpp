//Libraries
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <fstream>

//Headers
#include "Gameplay.h"


using namespace std;

void menuGraphics();


int main()
{
    Gameplay newGame;
    menuGraphics();
    
    char select;
    cin >> select;
    select = tolower(select);

    if(select == 'p')
    {
        newGame.Play();
    }
    if(select == 'q')
    {
        cout << "Thanks for playing, goodbye!" << endl;
    }
}

void menuGraphics()
{
        cout << "                    _____________________________                              " << endl;
        cout << "                        |~~_| BATTLESHIP |_~~|                                 " << endl;
        cout << "                    -----------------------------                              " << endl;
        cout << "                        Press P to Play                                        " << endl;
        cout << "                        Press H to view High-scores                            " << endl;
        cout << "                        Press Q to Quit                                        " << endl;
        cout << "                    -----------------------------                              " << endl;
        cout << "                                       # #  ( )                                " << endl;
        cout << "                                    ___#_#___|__                               " << endl;
        cout << "                                _  |____________|  _                           " << endl;
        cout << "                        _=====| | |            | | |==== _                     " << endl;
        cout << "                    =====| |.---------------------------. | |====              " << endl;
        cout << "    <--------------------'   .  .  .  .  .  .  .  .   '--------------/         " << endl;
        cout << "      V                                                             /          " << endl;
        cout << "       V___________________________________________________________V           " << endl;
        cout << "    wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww      " << endl;
        cout << "    wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww   " << endl;
        cout << "    wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww        " << endl;
        cout << "                                                                               " << endl;
        cout << endl;
}
