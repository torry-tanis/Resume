/*
Creator: TORRYANA_TANIS
Project: TREASURE_MAP
*/
#include <iostream>
#include <cstdlib>

using namespace std;

//constant declarations
const int MAX_X = 4;
const int MAX_Y = 5;

//intialization of each function
void initializeMap(char map[MAX_Y][MAX_X]);
void showMap(char map[MAX_Y][MAX_X]);
bool movePlayer(char map[MAX_Y][MAX_X], char direction);
bool checkWin(char map[MAX_Y][MAX_X]);

int main(int argc, char* argv[]) 
{
    
    char direction; //variable declaration
    direction = tolower(direction);//prevents caps lock errors

    if (argc != 2) //Errror message if user inputs more than 2 arguments
    {
        cout << "Usage: ./game <seed>\n"; //Error message that will display on console
        return 0;
    }
    int seed = stoi(argv[1]); //declaration of seed and converts string to integer
    char map[MAX_Y][MAX_X]; //declaration of map
    

    srand(seed); // randomizes the seed
    initializeMap(map);// Initialize the map 
    showMap(map); // calling showMap fuction

   do 
   {
        cout << "Move (u)p, (d)own, (l)eft, (r)ight or (q)uit\n >> "; //prompts user for direction input 
        cin>>direction; //recieves input from user

        if (movePlayer(map, direction)) //if fucntion is movePlayer is called do this
        {
            bool won = checkWin(map); //made flag equal checkWin function
            showMap(map); //calls moveplayer function

            if(won)//if flag is set off do this
            {
                cout<<"YOU WIN!!"; //Winner message outputs
                break; //then break out of code
            }
        } 
        else 
        {
            cout << "Can't move that way..."<<endl; //Message for Out of bounds movements
        }

    }while(direction != 'q' && checkWin(map) == false); //keep doing loop until these conditions are met
    
    return 0;

}

void showMap(char map[MAX_Y][MAX_X]) //funtion displays treausre map
{
    cout << "\n";
    for (int y = 0; y < MAX_Y; y++) //map format
    {
        for (int x = 0; x < MAX_X; x++) 
        {
            cout << "  " << map[y][x];
        }
        cout << endl;
    }
    cout << "\n";
}

void initializeMap(char map[MAX_Y][MAX_X])  //function gives player and treasure random coordinates
{
    //variable declaartions
    int x_player = 0;
    int y_player = 0;

    int x_treasure = 0;
    int y_treasure = 0;
    bool equal = false; //flag for if P and T coordinates are the same
    do
    {
        x_player = rand() % MAX_X;//randomizes X for Player
        y_player = rand() % MAX_Y;//randomizes Y for Player

        x_treasure = rand() % MAX_X; //randomizes X for Treasure
        y_treasure = rand() % MAX_Y; //ranomizes Y for Treasure

        if (x_player == x_treasure && y_player == y_treasure) //if Player and Treasure coorindates are the same flag is true
        {
            equal = true; 
        }
    }while(equal);//keep doing loop until condition is met


    for(int y = 0;  y < MAX_Y; y++) //finds position of player
    {
        for(int x = 0; x < MAX_X; x++)
        {
            if (y== y_player && x==x_player)
            {
                map[y][x] = 'P'; //puts P in place of the player starting position
            } 
            else if (y== y_treasure && x==x_treasure)
            {
                map[y][x] = 'X'; //puts X in place of the treasures starting position
            }
            else
            {
                map[y][x] = '.'; //fills rest of code with '.'
            }
        }
    }
    


}

bool movePlayer(char map[MAX_Y][MAX_X], char direction) //function the controls player movements
{
    //declaration of variables
    int x_player = 0;
    int y_player = 0;

    bool move = true;//flag
    
    for(int y = 0; y < MAX_Y; y++)//finds position of player
    {
        for(int x = 0; x < MAX_Y; x++)
        {
            if (map[y][x] == 'P')
            {
                x_player = x;
                y_player = y;
            }
        }
    }
    switch (direction) 
    {
        case 'u': //case for "up" direction
            if (y_player-1 < 0) //put into place if user goes out of bounds
            {
                move = false;
            } 
            else //moves the player's position up
            {
                map[y_player][x_player] = '.';
                map[--y_player][x_player] = 'P';
            }
            break;

        case 'd': //case for "down" direction 
            if (y_player+1 >= MAX_Y)//put into place if user goes out of bounds
            {
                 move = false;//flag triggers Error message
            } 
             else //moves the player's position down
            {
                map[y_player][x_player] = '.';
                map[++y_player][x_player] = 'P';
            }
            break;

        case 'l': //case for "left" direction
            if (x_player-1 < 0)//put into place if user goes out of bounds
            {
                move = false;
            } 
            else //moves the player's position left
            {
                map[y_player][x_player] = '.';
                map[y_player][--x_player] = 'P';
            }
            break;

        case 'r': //case for "right" direction
            if (x_player+1 >= MAX_X) //put into place if user goes out of bounds
            {
                move = false;
            } 
            else //moves the player's position right
            {
                map[y_player][x_player] = '.';
                map[y_player][++x_player] = 'P';
            }
            break;


        case 'q': //case if user quits
            break;

        default: //case for invalid input
            move = false;
            break;
    }
    return move; //returs move
}

bool checkWin(char map[MAX_Y][MAX_X]) //function that checks if user  won game
{
    //declaration of variables
    int x_player = 0;
    int y_player = 0;

    for(int y = 0; y < MAX_Y; y++) //finds treasure(x)
    {
        for(int x = 0; x < MAX_Y; x++)
        {
            if (map[y][x] == 'X') //takes X off of map 
            {
                return false;
            }
        }
    }

    for(int y = 0; y < MAX_Y; y++) //replaces P with W when X and P are the same coorindates
    {
        for(int x = 0; x < MAX_Y; x++)
        {
            if (map[y][x] == 'P')
            {
                map[y][x] = 'W'; //diplays W instead of P
            }
        }
    }
    
    
    return true;
}



