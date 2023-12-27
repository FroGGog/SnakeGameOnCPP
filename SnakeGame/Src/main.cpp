#include <iostream>
#include <vector>
#include <chrono>
#include <conio.h>
#include <string>

enum class Dirs { Left = 1, Right, Up, Down };

const double FPS{ 0.003 }; //updates per period of time
const int FIELDSIZE{ 100 };//all gamefield size
const int ROWCUT{ 20 };//get field into new line. = FIELDSIZE / N 

class Head {

public:
    short direction_x{ 0 };
    short saved_x{ 0 };
    char playerChar{ 'X' };
    short vert_speed{ ::ROWCUT };
    
    Dirs dir{ Dirs::Right };

    short lenght{ 1 };

    void Move(short dir) {
        if (direction_x + dir >= ::FIELDSIZE) {
            if (this->dir == Dirs::Right) {
                direction_x = 0;
                saved_x = ::FIELDSIZE - 1;
                return;
            }
            else if (this->dir == Dirs::Down) {
                saved_x = direction_x;
                direction_x = direction_x % 10;
                return;
            }
            else {
                return;
            }
        }
        else if (direction_x + dir < 0) {
            if (this->dir == Dirs::Left) {
                direction_x = ::FIELDSIZE - 1;
                saved_x = 0;
                return;
            }
            else if (this->dir == Dirs::Up) {
                saved_x = direction_x;
                direction_x += ( ( ::FIELDSIZE / ROWCUT) - 1 ) * ROWCUT;
                return;
            }
            else {
                return;
            }
        }
        else {
            saved_x = direction_x;
            direction_x += dir;
        }
        
    }

};

struct Body {

public:
    short direction_x{0};
    short saved_x{};
    char bodyChar{ 'O' };

    Body(Head &head) {
        direction_x = head.saved_x;

    }
    Body(const Body &part) {
        direction_x = part.saved_x;
    }
    void UpdatePos(Head &head, std::vector<char> &gameField) {
        saved_x = direction_x;
        direction_x = head.saved_x;
        gameField[direction_x] = bodyChar;
        gameField[saved_x] = '*';
    }
    void UpdatePos(Body &body, std::vector<char>& gameField) {
        saved_x = direction_x;
        direction_x = body.saved_x;
        gameField[direction_x] = bodyChar;
        gameField[saved_x] = '*';
    }
};


void printGameField(std::vector<char> gameField){
	system("cls");
    for (short i{ 0 }; i < gameField.size(); i++) {
        if (i!= 0 && i % ::ROWCUT == 0) {
            std::cout << "\n";
        }
        std::cout << gameField[i];
	}
    std::cout << "\n";
}

bool Input(Head &player)
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'a':         
            player.dir = Dirs::Left;
            return true;
        case 'd':
            player.dir = Dirs::Right;
            return true;
        case 'w':
            player.dir = Dirs::Up;
            return true;
        case 's':
            player.dir = Dirs::Down;
            return true;
        case 'x':
            return true;
        default:
            break;
        }
    }
    return false;
    
}

bool Update(std::chrono::system_clock::time_point tp) {
    auto time_check = std::chrono::system_clock::now();
    std::chrono::duration<float> timeToUPD{ (time_check - tp) / 60 };
    if (timeToUPD.count() > ::FPS) {
        return true;
    }
    return false;
}


int main() {

    auto time_start = std::chrono::system_clock::now();

    Head player{};
    Body part1{player};
    Body part2{ part1 };
    Body part3{ part2 };
    Body part4{ part3 };
    Body part5{ part4 };

    std::vector<Body> SnakeBody{part1};

	std::vector<char> gameField(::FIELDSIZE, '*');
    gameField[0] = player.playerChar;

    while (true) {
        
        if (Update(time_start)) {
            if (!Input(player)) {
                switch (player.dir) {
                case Dirs::Left:
                    player.Move(-1);
                    break;
                case Dirs::Right:
                    player.Move(1);
                    break;
                case Dirs::Up:
                    player.Move(-player.vert_speed);
                    break;
                case Dirs::Down:
                    player.Move(player.vert_speed);
                    break;
                default:
                    break;
                }
                gameField[player.direction_x] = player.playerChar;
                if (player.lenght == 0) {
                    gameField[player.saved_x] = '*';
                }
                for (int i{ 0 }; i < player.lenght; i++) {
                    if (i == 0) {
                        SnakeBody[i].UpdatePos(player, gameField);
                    }
                    else {
                        SnakeBody[i].UpdatePos(SnakeBody[i - 1], gameField);
                    }
                    
                }
               
                
            }
            time_start = std::chrono::system_clock::now();
            printGameField(gameField);
        }
        
    }
	


	return 0;
}