#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
using namespace std;


bool gameOver;
bool gameWon;
const int width = 30;
const int height = 20;
int x, y;
int fruitX, fruitY;
int score;
int tailX[1000], tailY[1000];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;
int difficulty;
time_t startTime;
int gameCount = 0;
int pCount = 0;


string getSeconds(int seconds) {
    if (seconds % 10 == 1 && seconds % 100 != 11) {
        return "секунда";
    }
    else if ((seconds % 10 >= 2 && seconds % 10 <= 4) && (seconds % 100 < 10 || seconds % 100 >= 20)) {
        return "секунды";
    }
    else {
        return "секунд";
    }
}


void Setup() {
    gameOver = false;
    gameWon = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
    startTime = time(0);
    pCount = 0;
}


void Draw() {
    system("cls");
    for (int i = 0; i < width + 2; i++)
        cout << "=";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                cout << "|";
            if (i == y && j == x)
                cout << "O";
            else if (i == fruitY && j == fruitX)
                cout << "*";
            else {
                bool print = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << "o";
                        print = true;
                    }
                }
                if (!print) cout << " ";
            }

            if (j == width - 1)
                cout << "|";
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++)
        cout << "=";
    cout << endl;


    time_t currentTime = time(0);
    int Time = difftime(currentTime, startTime);

    cout << "Количество очков: " << score << endl;
    cout << "Время игры: " << Time << " " << getSeconds(Time) << endl;
    cout << "Игра " << gameCount << endl;
    cout << "Нажмите 'x' для выхода" << endl;
}


void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
            dir = LEFT;
            break;
        case 'd':
            dir = RIGHT;
            break;
        case 'w':
            dir = UP;
            break;
        case 's':
            dir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        case 'p':
            pCount++;
            if (pCount == 3) {
                gameWon = true;
                gameOver = true;
            }
            break;
        default:
            pCount = 0;
            break;
        }
    }
}


void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    switch (dir) {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }

    if (difficulty == 3) {
        if (x >= width || x < 0 || y >= height || y < 0) {
            gameOver = true;
        }
    }
    else {
        if (x >= width) x = 0;
        else if (x < 0) x = width - 1;

        if (y >= height) y = 0;
        else if (y < 0) y = height - 1;
    }

    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;

    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }


    if (nTail == width * height - 1) {
        gameWon = true;
        gameOver = true;
    }
}


void ShowGameOver() {
    system("cls");
    time_t currentTime = time(0);
    int Time = difftime(currentTime, startTime);
    if (gameWon) {
        cout << "####################################" << endl;
        cout << "#                                  #" << endl;
        cout << "#         CONGRATULATIONS!         #" << endl;
        cout << "#         YOU WON THE GAME         #" << endl;
        cout << "#                                  #" << endl;
        cout << "####################################" << endl;
    }
    else {
        cout << "####################################" << endl;
        cout << "#                                  #" << endl;
        cout << "#           GAME OVER              #" << endl;
        cout << "#                                  #" << endl;
        cout << "####################################" << endl;
    }
    cout << "Количество очков: " << score << endl;
    cout << "Время игры: " << Time << " " << getSeconds(Time) << endl;
    cout << "Количество игр: " << gameCount << endl;
    cout << "Нажмите 'r' для рестарта или 'x' для выхода" << endl;
}

int main() {
    setlocale(0, "");

    do {

        do {
            cout << "Выберите уровень сложности\n 1 - Легкий\n 2 - Средний\n 3 - Сложный\n";
            cin >> difficulty;

            if (difficulty < 1 || difficulty > 3) {
                cout << "Ошибка: неверный уровень сложности. Попробуйте снова." << endl;
            }
        } while (difficulty < 1 || difficulty > 3);

        int sleepTime;
        switch (difficulty) {
        case 1: sleepTime = 100; break;
        case 2: sleepTime = 65; break;
        case 3: sleepTime = 25; break;
        }

        gameCount++;
        Setup();
        while (!gameOver) {
            Draw();
            Input();
            Logic();
            Sleep(sleepTime);
        }
        ShowGameOver();

        char choice;
        do {
            choice = _getch();
            if (choice == 'x') {
                return 0;
            }
        } while (choice != 'r');

    } while (true);

    return 0;
}
