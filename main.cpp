//Naval Warfare
//Alex von Allemann 55678580
//Creative Project - CMPG121_Group1

#include <iostream>
#include <iomanip>
#include <cstdlib> //For rand() and srand() functions.
#include <ctime> // For time() function.
#include <limits>
#include <fstream> //For input file capability.

using namespace std;

//Create Boat Struct.
struct Boat
{
    string name;
    int size;
    int numHits; //How much the boat has been hit.
    bool sunken; //Check if the boat is sunk or not.
    int rowStart;
    int colStart;
    bool horizontal; //Checks the orientation.
};

//Prototype functions.
void initializeBoats(Boat boats[]);
void initializeBoard(char board[10][10]);
void printBoard(char board[10][10], bool revealTheBoats);
bool placeBoat(char board[10][10], Boat &boat, int row, int col, bool horizontal);
void placeAllBoatsRandomly(char board[10][10], Boat boats[], int boatCount);
bool takeShot(char board[10][10], int row, int col, Boat boats[], int boatCount, int &totalShots, int &totalHits, int &totalMisses);
bool allBoatsSunk(Boat boats[], int boutCount);
void displayMenu();

int main()
{
    cout << "=========================================================" << endl;
    cout << "=========================================================" << endl;
    cout << left << setw(15) << "" << setw(15) << "WELCOME TO NAVAL WARFARE" << endl;
    cout << "=========================================================" << endl;
    cout << "=========================================================" << endl;

    char board[10][10]; // ~ for the ocean, @ for the ships, X for hit, O for a missed shot.
    Boat boats[5]; //Array for the boats.
    int choice = 0; //Select a menu option.
    bool shotSuccess;
    int row;
    int col;

    int totalShots = 0;
    int totalHits = 0;
    int totalMisses = 0;

    //Create boats instances.
    initializeBoats(boats);
    displayMenu();
    cout << "\nChoose a option above: ";
    cin >> choice;

    while (choice != 4)
    {

        while (cin.fail() || choice < 1 || choice > 3)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout << "Choose a option above (1-3): ";
            cin >> choice;
        }
        //Switch Statement for users-choice.
        switch (choice)
        {
        case 1:
            {
                 initializeBoard(board); //Create the board filled with water.
            placeAllBoatsRandomly(board, boats, 5); //Attempt to place all the boats.

            ofstream outFile("GameBoards.txt", ios::app); //Append the file.

            if (outFile.is_open())
            {
                int countGame = 1;

                outFile << "\n";
                outFile << "Game " << countGame << ":\n";
                for (int row = 0; row < 10; row++)
                {
                    for (int col = 0; col < 10; col++)
                    {
                        outFile << board[row][col] << " ";
                    }
                    outFile << "\n";
                }
                countGame++;

                outFile << "=========================================\n\n";
                outFile.close();

            }

            cout << "\nLocate those Boats!" << endl;
            while (!allBoatsSunk(boats, 5))
            {
                printBoard(board, false);
                cout << "\nEnter the row: ";
                cin >> row;
                cout << "Enter the column: ";
                cin >> col;
                shotSuccess = takeShot(board, row, col, boats, 5, totalShots, totalHits, totalMisses);
                if (!shotSuccess)
                {
                    cout << "\nTry again!" << endl;
                }
            }
            cout << "\nCongratulations! You sunk all the ships!" << endl;
            break;

            }

        case 2:
            cout << "\nWelcome to Naval Warfare!" << endl;
            cout << "In Naval Warfare you are in command of a naval fleet trying to sink enemy boats hidden in the ocean." << endl;
            cout << "The game board is a 10x10 grid:" << endl;
            cout << "~ represents the ocean (water)." << endl;
            cout << "@ represents enemy boats." << endl;
            cout << "X marks a hit on a boats." << endl;
            cout << "O marks a missed shot." << endl;
            cout << endl;
            cout << "You will be given multiple boats to find and sink, each with various sizes:" << endl;
            cout << left << setw(20) << "Boat Name" << setw(20) << "Size (Cells)" << endl;
            for (int i = 0; i < 5; i++)
            {
                cout << left << setw(20) << boats[i].name << boats[i].size << endl;
            }
            cout << "\nGoodluck! and have fun." << endl;
            break;

        case 3:
            cout << "\nStatistics:" << endl;
            cout << "Total Shots: " << totalShots << endl;
            cout << "Total Hits: " << totalHits << endl;
            cout << "Total Misses: " << totalMisses << endl;

            ofstream outFileStats("GameStatistics.txt", ios::app);

            if (outFileStats.is_open())
            {
                int countStatistics = 1;

                outFileStats << "\n";
                outFileStats << "Statistics for Game " << countStatistics << ":\n";
                outFileStats << "Total Shots: " << totalShots << "\n";
                outFileStats << "Total Hits: " << totalHits << "\n";
                outFileStats << "Total Misses: " << totalMisses << "\n";
                countStatistics++;
                outFileStats << "=========================================\n\n";
                outFileStats.close();
            }

            break;
        }
        displayMenu();
        cout << "\nChoose a option above: ";
        cin >> choice;


    }
    cout << "\nThank you for playing!" << endl;

    return 0;
}

void initializeBoats(Boat boats[])
{
    boats[0] = {"Frigate", 4, 0, false}; //Frigate boat.
    boats[1] = {"Flying Dutchman", 5, 0, false}; //Flying Dutchman boat.
    boats[2] = {"Sub Hunter", 3, 0, false}; //Sub Hunter boat.
    boats[3] = {"Patrol Boat", 2, 0, false}; //Patrol boat.
    boats[4] = {"Bismark", 3, 0, false}; // Bismark boat.

}

void initializeBoard(char board[10][10])
{
    for (int row = 0; row < 10; row++)
    {
        for (int col = 0; col < 10; col++)
        {
            board[row][col] = '~'; // Generate the 10 by 10 ocean.
        }
    }
}

void printBoard(char board[10][10], bool revealTheBoats)
{
    cout << endl;
    cout << "  ";
    for (int col = 0; col < 10; col++)
    {
        cout << col << " "; //We are creating the column headers.
    }
    cout << endl;

    for (int row = 0; row < 10; row++)
    {
        cout << row << " "; //Row heading.

        for (int col = 0; col < 10; col++)
        {
            char currentCell = board[row][col];
            if (currentCell == '@' && !revealTheBoats)
            {
                cout << "~ "; //Hiding the ships if not revealed.
            }
            else
            {
                cout << currentCell << " ";
            }
        }
        cout << endl;
    }
}

bool placeBoat(char board[10][10], Boat &boat, int row, int col, bool horizontal)
{

    if (horizontal)
    {
        if (col + boat.size > 10)
        {
            return false; //As the boat is out of bounds.
        }
        for (int i = 0; i < boat.size; i++)
        {
            if (board[row][col + i] == '@')
            {
                return false; //As there is already a boat in that position.
            }
        }

        //Create boat placement.
        for (int i = 0; i < boat.size; i++)
        {
            board[row][col+i] = '@';
        }
    }
    else //The player has choosen a vertical placement.
    {
        if (row + boat.size > 10) //Checking for out of bounds.
        {
            return false;
        }
        for (int i = 0; i < boat.size; i++)
        {
            if (board[row + i][col] == '@')
            {
                return false;
            }
        }
        //Place boat.
        for (int i = 0; i < boat.size; i++)
        {
            board[row+i][col] = '@';
        }
    }
    boat.rowStart = row;
    boat.colStart = col;
    boat.horizontal = horizontal;
    return true; //The boat has been placed correctly.
}

void placeAllBoatsRandomly(char board[10][10], Boat boats[], int boatCount)
{
    srand(time(0)); // Seeding the random number creater

    for (int i = 0; i < boatCount; i++)
    {
        bool placedBoat = false;

        while (!placedBoat)
        {
            int row = rand() % 10;
            int col = rand() % 10;
            bool horizontal = rand() % 2; // 0 = Vertical placement, 1 = Horizontal Placement.
            placedBoat = placeBoat(board, boats[i], row, col, horizontal);
        }
    }
}

//Function to take a shot.
bool takeShot(char board[10][10], int row, int col, Boat boats[], int boatCount, int &totalShots, int &totalHits, int &totalMisses)
{

    if ( row < 0 || row >= 10 || col < 0 || col >= 10)
    {
        cout << "Invalid coordinates!.\n";
        return false;
    }
    if (board[row][col] == 'X' || board[row][col] == 'O')
    {
        cout << "This location has already been shot at.\n";
        return false;
    }
     totalShots++;

    if (board[row][col] == '@')
    {
        cout << "\nHit!\n";
        board[row][col] = 'X'; //As the boat has been hit now.
        totalHits++;


        for (int i = 0; i < boatCount; i++)
        {
            Boat &b = boats[i];
            bool hitThisBoat = false;

            for (int j = 0; j < b.size; j++)
            {
                int r;
                int c;
                if (b.horizontal)
                {
                    r = b.rowStart;
                    c = b.colStart + j;
                }
                else
                {
                    r = b.rowStart + j;
                    c = b.colStart;
                }

                if (r == row && c == col)
                {
                    hitThisBoat = true;
                    break;
                }
            }

            if (hitThisBoat)
            {
                b.numHits++;
                if (b.numHits >= b.size && !b.sunken)
                {
                    b.sunken = true;
                    cout << endl;
                    cout << "\nYou sunk the " << b.name << "!\n" << endl;
                }
            }

        }
    }
    else
    {
        cout << "\nMiss!\n";
        board[row][col] = 'O'; //Mark the board with a miss.
        totalMisses++;
    }
    return true; //Show was a success.
}

//Display the menu.
void displayMenu()
{
    cout << "\nChoose a option:" << endl;
    cout << "1. Play Game" << endl;
    cout << "2. Tutorial" << endl;
    cout << "3. View Statisitics" << endl;
    cout << "4. Exit" << endl;
}

//Function to check if all boats have been sunken.
bool allBoatsSunk(Boat boats[], int boutCount)
{
    for (int i = 0; i < boutCount; i++)
    {
        if (!boats[i].sunken)
        {
            return false;
        }
    }
    return true;
}
