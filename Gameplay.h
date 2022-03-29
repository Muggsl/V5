#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cctype>
#include <time.h>
#include <string>

using namespace std;

class Gameplay
{
    public:
        Gameplay(string playerName)
        {
            this->playerName = playerName;
        }
        Gameplay(){}
        virtual ~Gameplay() {}
        int submarine = 4;
        
        enum Tile { empty, ship, bombed, checked };
        
        void Play();
        void printVector();
        void printEnemyVector();
        void AIaddShipRandom(vector<vector<Tile>> &vec, int size);
        void playerAddShip(int size);
        bool placeShip(int y, int x, bool horizontal, int size);
        void populateEmptyTiles(vector<vector<Tile>> &vec, int x, int y);
        void checkForBombedShips(vector<vector<int>> &shipLocations);
        void countShips(vector<vector<Tile>> &vec);
        void bombPoint(int x, int y);
        void playerMove();
        void AIMove();
        void AIBomb();
        void AISmartBomb();
        void saveGame();
        void epicText();
        

        int numberOfShips(vector<vector<Tile>> &vec);
        bool isEmptyCheck(vector<vector<Tile>> &vec, int size, int x, int y, bool horizontal);

        vector<vector<Tile>> playerGrid;
        vector<vector<Tile>> AIGrid;
        vector<vector<int>> playerShipLocations;
        vector<vector<int>> AIShipLocations;
        // keeps track of the orignal bomb that hit
        // we then make a smaller hit box made up from them
        int lastBombed[2] = {-1,-1};
        int smartBombCount = 0;
        bool amISmart = false;
        
        
        
        friend ostream &operator << (ostream& strm, Tile t)
        {
            const string nameT[] = { "_", "S", "#", "O"};
            return strm << nameT[t];
        }

        private:
            string playerName;

};

class SaveGame : public Gameplay
{
    public:
        void saveGame(vector<vector<Tile>> AIgridCopy, vector<vector<Tile>> playerGridCopy, vector<vector<int>> AILocations, vector<vector<int>> playerLocations);
        void loadGrid(vector<vector<Tile>> &gridVector, string filename);
        void loadLocation(vector<vector<int>> &locationVector, string filename);
        
};


#endif //GAMEPLAY_H