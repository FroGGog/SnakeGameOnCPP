#include <iostream>
#include <vector>
#include <Windows.h>
#include <chrono>
#include <conio.h>

class Player {

public:
    short direction_x{ 0 };
    short saved_x{ 0 };
    char playerChar{ 'O' };
    short vert_speed{ 10 };

    short lenght{ 1 };

    void Move(short dir) {
        if (direction_x + dir >= 50 || direction_x + dir < 0) {
            return;
        }
        saved_x = direction_x;
        direction_x += dir;
    }

    void PrintInfo() {
        std::cout << "x : " << direction_x << '\n';
    }

};


void printGameField(std::vector<char> gameField){
	system("cls");
    for (short i{ 0 }; i < gameField.size(); i++) {
        if (i!= 0 && i % 10 == 0) {
            std::cout << "\n";
        }
        std::cout << gameField[i];
	}
    std::cout << "\n";
}

bool Input(Player &player)
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'a':
            player.Move(-1);
            return true;
        case 'd':
            player.Move(1);
            return true;
        case 'w':
            player.Move(-player.vert_speed);
            return true;
        case 's':
            player.Move(player.vert_speed);
            return true;
        case 'x':
            return true;
        default:
            break;
        }
    }
    return false;
    
}


int main() {


    Player player{};

	std::vector<char> gameField(50, '*');
    gameField[0] = player.playerChar;

	printGameField(gameField);
    while (true) {
        if (Input(player)) {
            gameField[player.direction_x] = player.playerChar;
            gameField[player.saved_x] = '*';
            printGameField(gameField);
        }
        
    }
	


	return 0;
}