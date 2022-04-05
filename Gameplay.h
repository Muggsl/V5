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
        int convertYCharToInt(char y);
        void AIaddShipRandom(vector<vector<Tile>> &vec, int size);
        void playerAddShip(int size);
        bool placeShip(int y, int x, bool horizontal, int size);
        void populateEmptyTiles(vector<vector<Tile>> &vec, int x, int y);
        void checkForBombedShips(vector<vector<int>> &shipLocations);
        void countShips(vector<vector<Tile>> &vec);
        void bombPoint(int x, int y);
        void playerMove();
        void AIMove();
        void AIBomb(int x, int y);
        void AISmartBomb();
        void saveGame();
        void epicText();
        void quitGame();
        

        int numberOfShips(vector<vector<Tile>> &vec);
        bool isEmptyCheck(vector<vector<Tile>> &vec, int size, int x, int y, bool horizontal);

        vector<vector<Tile>> playerGrid;
        vector<vector<Tile>> AIGrid;
        vector<vector<int>> playerShipLocations;
        vector<vector<int>> AIShipLocations;
       //vector<tuple<int, int>> TheLoop; // locations surrounding last AI hit
        //tuple<int, int> AiHit; // last location AI hit
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

        
            int turns = 0;
            string playerName;

};

class SaveGame : public Gameplay
{
    public:
        struct ScoreStruct
        {
            ScoreStruct()
            {
                this->score = 0;
                this->name = "";
            } 
            ScoreStruct(int score, string name)
            {
                this->score = score;
                this->name = name;
            }

            bool compereBySize(const ScoreStruct &a, const ScoreStruct &b)
            {
                return a.score < b.score;
            }

            int score;
            string name;
        };

        void saveGame(vector<vector<Tile>> AIgridCopy, vector<vector<Tile>> playerGridCopy, vector<vector<int>> AILocations, vector<vector<int>> playerLocations, string name);
        void loadGrid(vector<vector<Tile>> &gridVector, string filename);
        void loadLocation(vector<vector<int>> &locationVector, string filename);
        void loadPlayerName(string &playerName);
        void writeHighScore(ScoreStruct s);
        void loadHighScore();
        static bool my_cmp(const ScoreStruct& a, const ScoreStruct& b);
        
        
};

class Score : public SaveGame
{
    public:
        void calculateScore(vector<vector<Tile>> playerVec, vector<vector<int>> playerSipsLocations, int turns, string name);
        void printHighScore();
        
};



#endif //GAMEPLAY_H