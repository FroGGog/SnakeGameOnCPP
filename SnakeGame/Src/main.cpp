#include <iostream>
#include <vector>
#include <chrono>
#include <conio.h> // for user inputs 
#include <string>
#include <ctime>
#include <cstdlib>

enum class Dirs { Left = 1, Right, Up, Down };

const double FPS{ 0.003 }; //updates per period of time
const int FIELDSIZE{ 100 };//all gamefield size
const int ROWCUT{ 20 };//get field into new line. = FIELDSIZE / N 


class Head {

public:
    short direction_x{ 0 };//x coordinates because our game is one big vector of chars
    short saved_x{ 0 }; //previous x coord of head
    char playerChar{ 'X' };
    short vert_speed{ ::ROWCUT }; //snake 'verctical' speed 
    
    Dirs dir{ Dirs::Right };

    short lenght{ 1 };

    void Move(short dir) {
        if (direction_x + dir >= ::FIELDSIZE) {//check if player if outside field and going down
            if (this->dir == Dirs::Down) {
                saved_x = direction_x;
                direction_x -= ((::FIELDSIZE / ROWCUT) - 1) * ROWCUT;
                return;
            }
        }
        else if (direction_x + dir < 0) { // and going up
            if (this->dir == Dirs::Up) {
                saved_x = direction_x;
                direction_x += ( ( ::FIELDSIZE / ROWCUT) - 1 ) * ROWCUT;
                return;
            }
        }
        if ((direction_x + 1) % ::ROWCUT == 0 && this->dir == Dirs::Right) {//going right
            saved_x = direction_x;
            direction_x -= ::ROWCUT-1;
            return;
        }
        else if (direction_x % ::ROWCUT == 0 && this->dir == Dirs::Left) { // going left
            saved_x = direction_x;
            direction_x += ::ROWCUT - 1;
            return;
        }
        else {//if player is on field he goes toward dir
            saved_x = direction_x;
            direction_x += dir;
        }
        
    }

    void SetLenght(size_t BodySize) {
        this->lenght = static_cast<short>(BodySize);
    }

    bool EatFruit(std::vector<char>& gameField) {//check if player's head is on fruit
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

    Body(Head &head) {//we need to get last head position for first body part
        direction_x = head.saved_x;

    }
    Body(short _dir_x, short s_x) {//and for other body parts get forward body position
        this->direction_x = _dir_x;
        this->saved_x = s_x;
    }
    void UpdatePos(Head &head, std::vector<char> &gameField) {//update position for first body part
        saved_x = direction_x;
        direction_x = head.saved_x;
        gameField[direction_x] = bodyChar;
        if (head.lenght == 1) {
            gameField[saved_x] = '*';
        }
        
        
    }
    void UpdatePos(short saved_x, std::vector<char>& gameField) {//update position for other body parts
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
    short fruit_count{ 0 };
    short fruit_pos{ 0 };

    void SpawnFruit() {
        fruit_pos = rand() % (::FIELDSIZE - 1);
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
    if (_kbhit())//check if player pressed any buttons 
    {
        switch (_getch())
        {
        case 'a':         
            if (player.dir == Dirs::Right) {//prevent player from going opposite direction immediately
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


bool Update(std::chrono::system_clock::time_point tp) {//check if programm must update game screen
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
    Body new_tail{ a, b };//add new tail part which get forward tail part saved_x position
    SnakeBody.push_back(new_tail);
    return;
}


void GameOver(short snakeLenght, std::chrono::system_clock::time_point tp) {
    system("cls");
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<float> timeForScore{ (now - tp) / 60 };
    std::cout << "You lose!\nYou eat : " << snakeLenght << " fruits!\n";
    std::cout << "Your total score is : " << (int)(timeForScore.count() + snakeLenght * 20);
}


int main() {
    srand(time(NULL));//get seed for random num
    auto time_start = std::chrono::system_clock::now();
    auto score_timer = time_start;//time for score
    bool inGame{ true };

    Head player{};//player head object
    Body part1{ player }; //firt added body part
    Fruit fruit{};

    std::vector<Body> SnakeBody{part1};
	std::vector<char> gameField(::FIELDSIZE, '*');

    gameField[0] = player.playerChar;//start player pos

    while (inGame) {
        
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
                if (gameField[player.direction_x] == 'O') {
                    inGame = false;
                    continue;
                }
                if (player.EatFruit(gameField)) {//if player eat fruit add one tail, delte fruit 
                    fruit.fruit_count--;
                    AddTail(SnakeBody);
                    player.SetLenght(SnakeBody.size());
                }
                if (fruit.fruit_count == 0) {//if there are no fruits on field spawn new one 
                    fruit.SpawnFruit();
                    while(gameField[fruit.fruit_pos] != '*') {//prevent fruit from spawning on player tail\head
                        fruit.SpawnFruit();
                    }
                    gameField[fruit.fruit_pos] = fruit.fruitChar;
                    fruit.fruit_count++;
                    
                }
                gameField[player.direction_x] = player.playerChar;
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
	
    GameOver((player.lenght - 1), score_timer);

	return 0;
}