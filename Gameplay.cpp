#include "Gameplay.h"
#include <limits>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <errno.h>
#include <stdio.h>
#include <algorithm>

#define GRID_SIZE 10
using namespace std;

#include<fstream>
bool newGame = true;

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

void Gameplay::epicText()
{
    system(CLEAR);
    cout << "------------------------------------------------------------------" << endl;
    cout << "War... War never changes" << endl;
    cout << "The Romans waged war to gather slaves and wealth." << endl;
    cout << "Spain built an empire from its lust for gold and territory." << endl;
    cout << "Hitler shaped a battered Germany into an economic superpower." << endl << endl;
    cout << "But war never changes." << endl << endl;
    cout << "In the 21st century, war was still waged over the resources that could be acquired." << endl;
    cout << "Only this time, the spoils of war were also its weapons: Petroleum and Uranium." << endl;
    cout << "For these resources, China would invade Alaska, the US would annex Canada," << endl;
    cout << "and the European Commonwealth would dissolve into quarreling, bickering nation-states, bent on controlling the last remaining resources on Earth." << endl << endl;          
    cout << "In 2077, the storm of world war had come again." << endl;
    cout << "In two brief hours, most of the planet was reduced to cinders." << endl;
    cout << "And from the ashes of nuclear devastation, a new civilization would struggle to arise." << endl;
    cout << "In those two brief hours, two fleets full of high-tech battleships get ready to eliminate each other" << endl << endl;
    cout << "------------------------------------------------------------------" << endl;
}

void Gameplay::Play()
{
    srand(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));

    populateEmptyTiles(playerGrid, GRID_SIZE, GRID_SIZE);
    populateEmptyTiles(AIGrid, GRID_SIZE, GRID_SIZE);

    string loadGame;
    cout << "Would you like to continue from last time? y/n:  ";
    cin >> loadGame;
    cout << endl;

    int ships[] = {5,2,3,4,2};
    // We start a new game, the old saved game will be over written
    if(loadGame != "y")
    {
        epicText();
        cout << "Enter your name: ";
        cin >> playerName;
        system(CLEAR);

        cout << "Welcome to the battlefield commander " << playerName << ", please place your ships with haste, the nukes have already launched" << endl << endl;
        printVector();
        for(int i = 0; i < 5; i++)
        {
            //AIaddShipRandom(playerGrid,ships[i]);
    
            playerAddShip(ships[i]);
            printVector();
        }
        
        for(int i = 0; i < 5; i++)
        {
            AIaddShipRandom(AIGrid, ships[i]);
        }
    }

    // We load last game into memory
    else
    {
        SaveGame *sg = new SaveGame();
        sg->loadGrid(AIGrid, "aigrid.txt");
        sg->loadLocation(AIShipLocations, "ailocations.txt");
        sg->loadGrid(playerGrid, "playergrid.txt");
        sg->loadLocation(playerShipLocations, "playerlocations.txt");
        sg->loadPlayerName(playerName);
        delete sg;
        
    }
    system(CLEAR);
    printEnemyVector();
    cout << endl;
    printVector();            
    while(true)
    {
        playerMove();
        AIMove();
        

        printEnemyVector();
        cout << endl;
        printVector();
        checkForBombedShips(AIShipLocations);
        checkForBombedShips(playerShipLocations);
        turns++;
        
        SaveGame *sg = new SaveGame();
        sg->saveGame(AIGrid, playerGrid, AIShipLocations, playerShipLocations, playerName);
        delete sg;
        
    }

}

void Gameplay::checkForBombedShips(vector<vector<int>> &shipLocations)
{
    int vec_index = 0;
    int shipSize = 0;
    vector<vector<int>> tmp_vector;
    for(auto &vec: shipLocations)
    {
        bool isBombed = true;
        int x = vec[1];
        int y = vec[2];
        int xy2 = vec[3];
        int bombCount = 0;

       /*
        Here we are checking all the if any ship is completely bombed
       */ 

        if(vec[0] == -1)
        {
            for(int i = x; i < (xy2 + 1); i++)
            {
                if (AIGrid[y][i] == bombed)
                {
                    bombCount++;
                }
                shipSize = (vec[3] - vec[1]) + 1;
                
            }
        }
        // vertical
        else
        {
            for(int i = y; i < (xy2 + 1); i++)
            {
                if (AIGrid[i][x] == bombed)
                {
                    bombCount++;
                }
                shipSize = (vec[3] - vec[2]) + 1;
            }
        }

        
        // If we find a bombed ship we remove it from ship locations
        // so not to check it again
        if(bombCount == shipSize)
        {
            cout << "Ship Down!!" << endl;
        }
        else
        {
            tmp_vector.push_back(vec);
        }
        
    }
    shipLocations = tmp_vector;

    // Is no ships are left in the locationVector, then that
    // player has lost.
    if(shipLocations.size() == 0)
    {
        cout << "Victory!" << endl;
        Score *score = new Score();
        score->calculateScore(playerGrid, playerShipLocations, turns, playerName);
        delete score;
        quitGame();

        
    }
}

void Gameplay::printEnemyVector()
{
    char column = 'A';
    int row = 1;
    cout << "  ";
    for(auto itY: AIGrid)
    {
        cout << column << " ";
        column++;
    }
    cout << endl;
    for(auto itY: AIGrid)
    {
        if(row < 10)
        {
            cout << row << " ";
        }
        else
        {
            cout << row;
        }
        for(auto itX: itY)
        {
            if (itX == empty)
            {
                cout << itX << " ";
            }
            else if(itX == ship)
            {
                cout << ship << " ";
            }
            else if (itX == bombed)
            {
                cout << itX << " ";
            }
            else if (itX == checked)
            {
                cout << itX << " ";
            }
        }
        cout << endl;
        row++;
    }
    cout << endl;
}

void Gameplay::printVector()
{
    char column = 'A';
    int row = 1;
    cout << "  ";
    for(auto itY: playerGrid)
    {
        cout << column << " ";
        column++;
    }
    cout << endl;
    for(auto itY: playerGrid)
    {
        if(row < 10)
        {
            cout << row << " ";
        }
        else
        {
            cout << row;
        }
        for(auto itX: itY)
        {
            cout << itX << " ";
        }
        cout << endl;
        row++;
    }
    cout << endl;
}

bool Gameplay::isEmptyCheck(vector<vector<Tile>> &vec, int size, int y, int x, bool horizontal)
{
    // Not so simple check
    // Checking if all points are empty for the ship
    // to be put down
    for (int i = 0; i < size; i++)
        {
            if (horizontal)
            {
                if (vec[y][x+i] != empty)
                {
                    return false;
                }
            }
            else
            {
                if (vec[y+i][x] != empty)
                {
                    return false;
                }
            }
        }
        return true;
}



        /*
            Here we are Adding all a ship somewere random on the grid
            for the AI. 
        */
void Gameplay::AIaddShipRandom(vector<vector<Tile>> &vec, int size)
{

    bool isEmpty = true;
    int x,y;
    //Randomly setting a ship to a horizontal placement 
    bool horizontal = (rand() % 2 == 1) ? true : false;
    
    // Setting random coordinations for the ships
    if(horizontal)
    {
        x = rand() % (vec.size() - size);
        y = rand() % vec.size();
    }
    else
    {
        x = rand() % vec.size();
        y = rand() % (vec.size() - size);
    }
  
    isEmpty = isEmptyCheck(vec, size, y, x, horizontal);

    if(horizontal && isEmpty)
    {
        for(int i = 0; i < size; i++)
        {
            vec[y][x+i] = ship;
        }
        
    }
    else if(!horizontal && isEmpty)
    {
        for(int i = 0; i < size; i++)
        {
            vec[y+i][x] = ship;
        }
         
    }

    /*
        Here we are adding the points to the locationVector for
        We use that to see if the ships are downed or not.
    */
    if(isEmpty)
    {
        vector<int> shipCoordinates;
        if(horizontal)
        {
            shipCoordinates.push_back(-1);
            shipCoordinates.push_back(x);
            shipCoordinates.push_back(y);
            shipCoordinates.push_back(x + (size - 1));
            AIShipLocations.push_back(shipCoordinates);
        }
        else
        {
            shipCoordinates.push_back(-2);
            shipCoordinates.push_back(x);
            shipCoordinates.push_back(y);
            shipCoordinates.push_back(y + (size - 1));
            AIShipLocations.push_back(shipCoordinates);
        }  
    }
    else
    {
        // if the random points assinged are not empty we just run this funtion again
        // intill it makes random points that are empty
        AIaddShipRandom(vec, size);
    }
}

/*
    Place ship is for placement of a single ship of the human player.
    It returns true if the placement is valid else false,
    that is so we error handeling in the UI
*/

bool Gameplay::placeShip(int y, int x, bool horizontal, int size)
{
    if(isEmptyCheck(playerGrid , size, y, x, horizontal))
    {
        if(horizontal)
        {
            for(int i = 0; i < size; i++)
            {
                playerGrid[y][x+i] = ship;
            }
        }
        else
        {
            for(int i = 0; i < size; i++)
            {
                playerGrid[y+i][x] = ship;
            }
        }
        
    }
    else
    {
        cout << "Invalid placement" << endl;
        return false;
    }

    /*
        Here we are adding the points to the locationVector for
        We use that to see if the ships are downed or not.
    */

    vector<int> shipCoordinates;
    if(horizontal)
    {
        shipCoordinates.push_back(-1);
        shipCoordinates.push_back(x);
        shipCoordinates.push_back(y);
        shipCoordinates.push_back(x + (size - 1));
        playerShipLocations.push_back(shipCoordinates);
    }
    else
    {
        shipCoordinates.push_back(-2);
        shipCoordinates.push_back(x);
        shipCoordinates.push_back(y);
        shipCoordinates.push_back(y + (size - 1));
        playerShipLocations.push_back(shipCoordinates);
    }
    return true;
}

void Gameplay::playerAddShip(int size)
{
    bool horizontal;
    string choice;

    bool is_ship_down = false;
    while (is_ship_down == false)
    {
        cout << "your ship is size: " << size << endl;
        cout << "(v)ertical or (h)orizontal?: ";
        cin >> choice;
        while(true)
        {
            if(choice == "v")
            {
                horizontal = false;
                break;
            }
            else if(choice == "h")
            {
                horizontal = true;
                break;
            }
            else
            {
                cout << "invalid choice" << endl;
                cout << "your ship is size: " << size << endl;
                cout << "(v)ertical or (h)orizontal?: ";
                cin >> choice;
                cout << choice << endl;
            }
        }
        cout << "x = 0-9, y = A-J" << endl;
        int x;
        char y;
        int y_int;
        cout << "x: ";
        cin >> x;
        while(!cin.good())
        {
            cout << "Error" << endl;
            cin.clear();
            cin.ignore(256,'\n');
            cout << "x: ";
            cin >> x;
        }
        // human gives letters, computer needs numbers
        cout << "y: ";
        cin >> y;
        
        while(!cin.good())
        {
            cout << "Error" << endl;
            cin.clear();
            cin.ignore(256,'\n');
            cin >> y;
        }
        y_int = convertYCharToInt(y);
        if(placeShip(y_int,x,horizontal,size)){break;}
        
    }
    system(CLEAR);
}
// filling enemy vector with empty tiles.
void Gameplay::populateEmptyTiles(vector<vector<Tile>> &vec, int x, int y)
{
    for(auto i = 0; i < y; ++i)
    {
        vector<Tile> xVector;
        for(auto j = 0; j < x; ++j)
        {
            xVector.push_back(empty);
        }
        vec.push_back(xVector);
    }
}
// a funtion to bomb the AI
void Gameplay::bombPoint(int x, int y)
{
    if(AIGrid[x][y] == ship)
    {
        AIGrid[x][y] = bombed;
        cout << "----------------------------------" << endl;
        cout << "***-*** --- Its a hit! --- ***-***" << endl;
        cout << "----------------------------------" << endl;
        
    }
    else if(AIGrid[x][y] == empty)
    {
        AIGrid[x][y] = checked;        
    }

    else if(AIGrid[x][y] == bombed)
    {
        cout << "Location already bombed!" << endl;
    }
}
// Player Move, where we ask the player to select where he wants to bomb
void Gameplay::playerMove()
{
    int row, column;
    char col;
    cout << "Enter the coordinance you want to bomb" << endl;
    do
    {
        cout << "column: ";
        cin >> col;
        column = toupper(col) - 65;
        if(column > 9 || column < 0)
        {
            cout << "Please select a valid column coordinance!" << endl;
        }
    }while((column > 9 || column < 0));
    do
    {
        cout << "row: ";
        cin >> row;
        row = --row;
        while(!cin.good() || row > 9 || row < 0)
        {
            cout << "Please select a valid row coordinance!" << endl;
            cin.clear();
            cin.ignore(256,'\n');
            cin >> row;
        }
    }while((row > 9 || row < 0));
    
    bombPoint(row, column);
    

}

void Gameplay::quitGame()
{
    string staller;
    cout << "Press any key to quit" << endl;
    cin >> staller;
    exit(0);
}

void Gameplay::AIMove()
{
    // Was the previous move a hit?
    // If not, we will randomly select a point to bomb
    //if (!AiHit & AiMoves.size() == 0){
        int x = rand() % GRID_SIZE;
        int y = rand() % GRID_SIZE;
        AIBomb(x,y);
    //}
    //if (TheLoop.size() =! 0){
        //AIBomb(TheLoop.back().x, TheLoop.back().y);
    //}
    // Are we in a loop checking the surrounding tiles?

   // AIBomb(); 
}

// just random bomb for now
void Gameplay::AIBomb(int x, int y)
{
    
    if (playerGrid[x][y] == bombed || playerGrid[x][y] == checked)
    {
        AIMove(); // try again
    }
    else if (playerGrid[x][y] == ship) // whapow
    {
        cout << "----------------------------------" << endl;
        cout << "*- WARNING A SHIP HAS BEEN HIT -**" << endl;
        cout << "----------------------------------" << endl;

        playerGrid[x][y] = bombed;
        amISmart = true;
        lastBombed[0] = x;
        lastBombed[1] = y;
        //AiHit = true;
        
    }
    else if (playerGrid[x][y] == empty)
    {
        playerGrid[x][y] = checked; // sploosh
    }
}
// not ready
void Gameplay::AISmartBomb()
{
    int x,y;
    x = rand() % lastBombed[0];
    y = rand() % lastBombed[1];

   
    if (playerGrid[x][y] == bombed || playerGrid[x][y] == checked)
    {
        AISmartBomb();
    }
    else if (playerGrid[x][y] == ship)
    {
        playerGrid[x][y] = bombed;
        smartBombCount++;
        
    }
    else if (playerGrid[x][y] == empty)
    {
        playerGrid[x][y] = checked;
        smartBombCount++;
    }

    if(smartBombCount >= 5)
    {
        amISmart = false;
    }
}

// Saves the game to text files
void SaveGame::saveGame(vector<vector<Tile>> AIgridCopy, vector<vector<Tile>> playerGridCopy, vector<vector<int>> AILocations, vector<vector<int>> playerLocationsCopy, string name)
{
    ofstream  fAIGrid("aigrid.txt");
    
    for(auto &r : AIgridCopy)
    {
        for(auto &point: r)
        {
            if(point == empty)
            {
                fAIGrid << "_";
            }
            else if(point == checked)
            {
                fAIGrid << "O";
            }
            else if(point == bombed)
            {
                fAIGrid << "#";
            }
            else if(point == ship)
            {
                fAIGrid << "S";
            }
        }
        fAIGrid << '\n';
    }
    fAIGrid.close();

    ofstream fPlayerGrid ("playergrid.txt");

    for(auto &p: playerGridCopy)
    {
        for(auto &point: p)
        {
            
            if(point == empty)
            {
                fPlayerGrid << "_";
            }
            else if(point == checked)
            {
                fPlayerGrid << "O";
            }
            else if(point == bombed)
            {
                fPlayerGrid << "#";
            }
            else if(point == ship)
            {
                fPlayerGrid << "S";
            }

        }
        fPlayerGrid << endl;
    }

    fPlayerGrid.close();

    ofstream fPlayerLocations ("playerlocations.txt");

    for(auto &playerLocVec: playerLocationsCopy)
    {
        for(auto &i: playerLocVec)
        {
            fPlayerLocations << i;
        }
        fPlayerLocations << endl;
    }

    fPlayerLocations.close();

    ofstream fAILocations ("ailocations.txt");

    for(auto &AIlocVec: AILocations)
    {
        for(auto &i: AIlocVec)
        {
            fAILocations << i;
        }
        fAILocations << endl;
    }
    
    fAILocations.close();

    ofstream f ("name.txt");
    f << name;
    f.close();
    
}

void SaveGame::loadGrid(vector<vector<Tile>> &grid, string filename)
{
    string myText;
    ifstream f (filename);
    vector<vector<Tile>> y;
    while(getline (f, myText))
    {
        vector<Tile> x;
        for(int i = 0; i < 10; i++)
        {
            if(myText[i] == '#')
            {
                x.push_back(bombed);
            }
            else if(myText[i] == 'S')
            {
                x.push_back(ship);
            }
            else if(myText[i] == 'O')
            {
                x.push_back(checked);
            }
            else if(myText[i] == '_')
            {
                x.push_back(empty);
            }
            
        }
        y.push_back(x);
    }
    grid = y;
}

void SaveGame::loadLocation(vector<vector<int>> &locVec, string filename)
{
    string myText;
    ifstream f (filename);
    vector<vector<int>> yRange;
    while(getline (f, myText))
    {
        string s;
        stringstream ss;
        vector<int> locVec;
        int horizontalCheck, x, y, xy;
        for(int i = 0; i < 5; i++)
        {
            switch (i)
            {
            case 0:
                ss << myText[i];
                break;
            case 1:
                ss << myText[i];
                ss >> s;
                horizontalCheck = stoi(s);
                locVec.push_back(horizontalCheck);
                ss.str("");
                ss.clear();
                break;
            case 2:
                ss << myText[i];
                ss >> s;
                x = stoi(s);
                locVec.push_back(x);
                ss.str("");
                ss.clear();
                break;
            case 3:
                ss << myText[i];
                ss >> s;
                y = stoi(s);
                locVec.push_back(y);
                ss.str("");
                ss.clear();
                break;
            case 4:
                ss << myText[i];
                ss >> s;
                xy = stoi(s);
                locVec.push_back(xy);
                ss.str("");
                ss.clear();
                break;
            }
            
        }
        yRange.push_back(locVec);
    }

    locVec = yRange;

    
}

void SaveGame::loadPlayerName(string &name)
{
    string myText;
    ifstream file("name.txt");
    string content;

    while(file >> content) 
    {
        name = content;
    }
    cout << name << endl;
}


void SaveGame::writeHighScore(ScoreStruct s)
{
    string data = s.name + " " + to_string(s.score) + "\n";
    ofstream myfile("highscore.txt", std::ofstream::out | std::ofstream::app);
    
    myfile << data;
    

}


void Score::calculateScore(vector<vector<Tile>> playerVec, vector<vector<int>> playerShipLocations, int turns, string name)
{
    int shipsDown = 5 - playerShipLocations.size();
    int bombedPoints = 0;
    for(auto &it: playerVec)
    {
        for(auto &tile: it)
        {
            if(tile == bombed)
            {
                bombedPoints++;
            }
        }
    }
    int totalScore = 1000 - (shipsDown + bombedPoints + turns);
    ScoreStruct s = ScoreStruct(totalScore, name);
    if(name != "")
        writeHighScore(s);
    cout << "total score: " << totalScore << " name: " << name << endl;
}

void Score::printHighScore()
{

   
    string myText;
    
    ifstream file("highscore.txt");
    string content;
    vector<ScoreStruct> scoreVec;

    while(file >> content) 
    {
        ScoreStruct s;
        s.name = content;
        file >> content;
        s.score = stoi(content);
        scoreVec.push_back(s);
    }
   
    sort(scoreVec.begin(), scoreVec.end(), my_cmp);

    cout << "High Scores!" << endl;

    for(auto &score: scoreVec)
    {
        cout << score.name << " " << score.score << endl;
    }
}

bool SaveGame::my_cmp(const ScoreStruct& a, const ScoreStruct& b)
{
    return a.score > b.score;
}

int Gameplay::convertYCharToInt(char y){

    if (y == 'A' || y == 'a'){
        return 0;
    }
    else if (y == 'B' || y == 'b'){
        return 1;
    }
    else if (y == 'C' || y == 'c'){
        return 2;
    }
    else if (y == 'D' || y == 'd'){
        return 3;
    }
    else if (y == 'E' || y == 'e'){
        return 4;
    }
    else if (y == 'F' || y == 'f'){
        return 5;
    }
    else if (y == 'G' || y == 'g'){
        return 6;
    }
    else if (y == 'H' || y == 'h'){
        return 7;
    }
    else if (y == 'I' || y == 'i'){
        return 8;
    }
    else if (y == 'J' || y == 'j'){
        return 9;
    }
    else{
        return -1;
    } 
  
}