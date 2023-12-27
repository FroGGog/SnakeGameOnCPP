#include <iostream>
#include <vector>
#include <chrono>
#include <conio.h>
#include <string>

enum class Dirs { Left = 1, Right, Up, Down };

const double FPS{ 0.005 }; //updates per period of time
const int FIELDSIZE{ 100 };//all gamefield size
const int ROWCUT{ 20 };//get field into new line. = FIELDSIZE / N 


//main snake head class
class Head {

public:
    short direction_x{ 0 };//x coordinates because our game is one big vector of chars
    short saved_x{ 0 }; //previous x coord of head
    char playerChar{ 'X' };
    short vert_speed{ ::ROWCUT }; //snake 'verctical' speed 
    
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

    void SetLenght(size_t BodySize) {
        this->lenght = static_cast<short>(BodySize);
    }

    bool EatFruit(std::vector<char>& gameField) {
        if (gameField[this->direction_x] == '@') {
            return true;
        }
        else {
            return false;
        }
    }



};

struct Body {

public:
    short direction_x{0};
    short saved_x{0};
    char bodyChar{ 'O' };

    Body(Head &head) {
        direction_x = head.saved_x;

    }
    Body(short _dir_x, short s_x) {
        this->direction_x = _dir_x;
        this->saved_x = s_x;
    }
    void UpdatePos(Head &head, std::vector<char> &gameField) {
        saved_x = direction_x;
        direction_x = head.saved_x;
        gameField[direction_x] = bodyChar;
        if (head.lenght == 1) {
            gameField[saved_x] = '*';
        }
        
        
    }
    void UpdatePos(short saved_x, std::vector<char>& gameField) {
        this->saved_x = direction_x;
        direction_x = saved_x;
        gameField[direction_x] = bodyChar;
        gameField[this->saved_x] = '*';
    }

};




struct Fruit{

public:
    int pos{25};
    char fruitChar{ '@' };

    void SpawnFruit() {
        return;
    };

};


void printGameField(std::vector<char> gameField){
	system("cls");
    for (short i{ 0 }; i < gameField.size(); i++) {
        if (i!= 0 && i % ::ROWCUT == 0) {
            std::cout << "\n\n";
        }
        std::cout << gameField[i] << ' ';
	}
}

bool Input(Head &player)
{
    /*if (player.direction_x > 6) {
        player.dir = Dirs::Down;
        return false;
    }*/
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'a':         
            if (player.dir == Dirs::Right) {
                return false;
            }
            player.dir = Dirs::Left;
            return true;
        case 'd':
            if (player.dir == Dirs::Left) {
                return false;
            }
            player.dir = Dirs::Right;
            return true;
        case 'w':
            if (player.dir == Dirs::Down) {
                return false;
            }
            player.dir = Dirs::Up;
            return true;
        case 's':
            if (player.dir == Dirs::Up) {
                return false;
            }
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


void AddTail(std::vector<Body>& SnakeBody) {
    short a = SnakeBody[SnakeBody.size() - 1].direction_x - 1;
    short b = SnakeBody[SnakeBody.size() - 1].saved_x - 1;
    Body new_tail{ a, b };
    SnakeBody.push_back(new_tail);
    return;
}


int main() {

    auto time_start = std::chrono::system_clock::now();

    Head player{};
    Body part1{ player };
    Fruit fruit{};
    Fruit fruit2{};

    std::vector<Body> SnakeBody{part1};

	std::vector<char> gameField(::FIELDSIZE, '*');
    gameField[0] = player.playerChar;
    gameField[4] = fruit.fruitChar;
    gameField[25] = fruit2.fruitChar;

    while (true) {
        
        if (Update(time_start)) {
            //player.SetLenght(SnakeBody.size());
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
                if (player.EatFruit(gameField)) {
                    AddTail(SnakeBody);
                    player.SetLenght(SnakeBody.size());
                }
                gameField[player.direction_x] = player.playerChar;
                if (player.lenght == 0) {
                    gameField[player.saved_x] = '*';
                }
                for (auto i{ 0 }; i < SnakeBody.size(); i++) {
                    if (i == 0) {
                        SnakeBody[i].UpdatePos(player, gameField);
                    }
                    else {
                        SnakeBody[i].UpdatePos(SnakeBody[i-1].saved_x, gameField);
                    }
                    
                }
               
                
            }
            time_start = std::chrono::system_clock::now();
            printGameField(gameField);
        }
        
    }
	


	return 0;
}