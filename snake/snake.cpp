#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
using namespace std;

// Переменные состояния игры
bool gameOver;       // Указывает на конец игры
bool gameWon;        // Указывает на победу
const int width = 30;  // Ширина игрового поля
const int height = 20; // Высота игрового поля
int x, y;             // Координаты головы змейки
int fruitX, fruitY;   // Координаты фрукта
int score;            // Количество очков
int tailX[1000], tailY[1000]; // Координаты хвоста змейки
int nTail;            // Длина хвоста змейки
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN }; // Направления движения змейки
eDirection dir;       // Текущее направление движения змейки
int difficulty;       // Переменная для уровня сложности
time_t startTime;     // Время начала игры
int gameCount = 0;    // Счетчик количества игр
int pCount = 0;       // Счетчик нажатий на кнопку 'p'

// Функция для склонения слова секунда
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

// Настройки начала игры
void Setup() {
    gameOver = false;
    gameWon = false;
    dir = STOP;
    x = width / 2;  // Начальные координаты головы змейки
    y = height / 2; // Начальные координаты головы змейки
    fruitX = rand() % width;  // Случайные координаты фрукта
    fruitY = rand() % height; // Случайные координаты фрукта
    score = 0;     // Сброс очков
    nTail = 0;     // Сброс длины хвоста
    startTime = time(0); // Запоминаем время начала игры
    pCount = 0;    // Сбрасываем счетчик нажатий на 'p'
}

// Отрисовка поля
void Draw() {
    system("cls"); // Очистка экрана
    for (int i = 0; i < width + 2; i++)
        cout << "=";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                cout << "|"; // Левая граница
            if (i == y && j == x)
                cout << "O"; // Голова змейки
            else if (i == fruitY && j == fruitX)
                cout << "*"; // Фрукт
            else {
                bool print = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << "o"; // Хвост змейки
                        print = true;
                    }
                }
                if (!print) cout << " ";
            }

            if (j == width - 1)
                cout << "|"; // Правая граница
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++)
        cout << "=";
    cout << endl;

    // Вычисляем и отображаем время игры
    time_t currentTime = time(0);
    int Time = difftime(currentTime, startTime);

    cout << "Количество очков: " << score << endl;
    cout << "Время игры: " << Time << " " << getSeconds(Time) << endl;
    cout << "Игра " << gameCount << endl;
    cout << "Нажмите 'x' для выхода" << endl;
}

// Обработка ввода пользователя
void Input() {
    if (_kbhit()) { // Проверка, была ли нажата клавиша
        switch (_getch()) {
        case 'a':
            dir = LEFT; // Движение влево
            break;
        case 'd':
            dir = RIGHT; // Движение вправо
            break;
        case 'w':
            dir = UP; // Движение вверх
            break;
        case 's':
            dir = DOWN; // Движение вниз
            break;
        case 'x':
            gameOver = true; // Завершение игры
            break;
        case 'p':
            pCount++; // Увеличиваем счетчик нажатий на 'p'
            if (pCount == 3) {
                gameWon = true; // Автоматическая победа
                gameOver = true;
            }
            break;
        default:
            pCount = 0; // Сбрасываем счетчик нажатий на 'p' при любом другом нажатии
            break;
        }
    }
}

// Логика игры
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
        x--; // Движение влево
        break;
    case RIGHT:
        x++; // Движение вправо
        break;
    case UP:
        y--; // Движение вверх
        break;
    case DOWN:
        y++; // Движение вниз
        break;
    default:
        break;
    }

    if (difficulty == 3) { // Проверка на сложном уровне
        if (x >= width || x < 0 || y >= height || y < 0) {
            gameOver = true; // Столкновение с границей
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
            gameOver = true; // Столкновение с хвостом

    if (x == fruitX && y == fruitY) {
        score += 10; // Увеличение очков
        fruitX = rand() % width; // Новые координаты фрукта
        fruitY = rand() % height; // Новые координаты фрукта
        nTail++; // Увеличение длины хвоста
    }

    // Проверка на победу
    if (nTail == width * height - 1) {
        gameWon = true;
        gameOver = true;
    }
}

// Окончание игры
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
        // Выбор уровня сложности
        do {
            cout << "Выберите уровень сложности\n 1 - Легкий\n 2 - Средний\n 3 - Сложный\n";
            cin >> difficulty;

            if (difficulty < 1 || difficulty > 3) {
                cout << "Ошибка: неверный уровень сложности. Попробуйте снова." << endl;
            }
        } while (difficulty < 1 || difficulty > 3);

        int sleepTime;
        switch (difficulty) {
        case 1: sleepTime = 100; break; // легкий
        case 2: sleepTime = 65; break;  // средний
        case 3: sleepTime = 25; break;  // сложный
        }

        gameCount++; // Увеличение счетчика игр
        Setup(); // Настройка начальных параметров
        while (!gameOver) {
            Draw(); // Отрисовка игрового поля
            Input(); // Обработка ввода пользователя
            Logic(); // Логика игры
            Sleep(sleepTime); // Задержка
        }
        ShowGameOver(); // Отображение экрана окончания игры

        char choice;
        do {
            choice = _getch();
            if (choice == 'x') {
                return 0; // Выход из игры
            }
        } while (choice != 'r');

    } while (true);

    return 0;
}
