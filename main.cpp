#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>


using namespace std;

class Simulation
{
public:
    //конструктор
    Simulation(int length, int heigth, bool isAlive = false, string cellChar = "<>")
        :length{ length }, heigth{ heigth }, cellChar{cellChar}
    {
        if (length <= 0 || heigth <= 0) {
            cout << "Error board size!\n";
        }

        board = new Cell * [heigth];    //выделяем памть для доски
        boardCopy = new Cell * [heigth];//выделяем памть для копии доски
        for (int i = 0; i < heigth; i++)
        {
            board[i] = new Cell[length];    //выделяем памть для доски
            boardCopy[i] = new Cell[length];//выделяем памть для копии доски
            for (int j = 0; j < length; j++)
            {
                board[i][j].status = isAlive;//заполняем значением по умолчанию
            }
        }

        srand(time(NULL));  //настройка рандомайзера
    }

    void ShowSettingBoard() {   //поках доски с цифровым статусом
        cout << setw(3) << " ";
        for (int i = 0; i < length; i++)
        {
            cout << setw(3) << i;   //выводим номера столбиков
        }
        cout << endl;
        cout << setw(3) << " ";

        for (int i = 0; i < length; i++)
        {
            cout << setw(3) << "_";
        }
        cout << endl;
        for (int i = 0; i < heigth; i++)
        {
            cout << setw(3) << i << "|";//выводим номер строк
            for (int j = 0; j < length; j++)
            {
                cout << setw(3) << board[i][j].status;  //выводим статус
            }
            cout << endl;
        }
    }

    void Drow() {
        for (int i = 0; i < (length * cellChar.size()); i++)
        {
            cout << "#";    //рисуем верхнюю границу
        }
        cout << endl;

        for (int i = 0; i < heigth; i++)
        {
            
            for (int j = 0; j < length; j++)
            {
                if (j == 0) {    //рисуем боковые границы
                    cout << "#";
                }
                if (board[i][j].status) {
                    cout << cellChar;   //выводим клетку
                }
                else {
                    cout << "  ";
                }
                if (j == length - 1)
                {
                    cout << "#";
                }
            }
            cout << endl;
        }

        for (int i = 0; i < (length * cellChar.size()); i++)
        {
            cout << "#";    //выводим нижнюю границу
        }
        cout << endl;
    }

    void SetAliveCell(int x, int y) {
        if (!isValidIndex(x, y)) {  //если координаты неправильные
            cout << "Invalid coords!\n";
            return;         //завершаем работу метода
        }

        board[y][x].status = 1; //по указаным координатам оживляем клетку
    }
    void SetDiedCell(int x, int y) {
        if (isValidIndex(x, y)) {
            return;
        }
        board[y][x].status = 0; //аналогичный метод, только тут мы убиваем клетку
    }

    void RandomFill() {//случайное заполнение
        for (int i = 0; i < heigth; i++)
        {
            for (int j = 0; j < length; j++)
            {
                int value = rand() % 100;
                board[i][j].status = (value % 3 == 0 || value % 7 == 0);    //условие(сделано на угад) для оживления клетки 

            }
        }
    }
    void Cals() {

        for (int y = 0; y < heigth; y++)    //копируем доску в переменную копии
            for (int x = 0; x < length; x++)
                boardCopy[y][x] = board[y][x];

        for (int y = 0; y < heigth; y++)
        {
            for (int x = 0; x < length; x++)
            {
                int isAlive = 0;
                for (int z = 0; z < 8; z++) //цикл для прохода по соседям
                {
                    if (isValidIndex(x + coordsX[z], y + coordsY[z])) { //если координата соседней клетки валидная
                        if (board[y + coordsY[z]][x + coordsX[z]].status) { //считаем сколько живых клеток рядом
                            isAlive++;
                        }
                    }
                }

                if (((board[y][x].status && (isAlive == 2 || isAlive == 3)) ||      //условие из задания
                    (!board[y][x].status && isAlive == 3)) && !board[y][x].isBlock()) {

                    board[y][x].status = 1;
                }
                if ((board[y][x].status && isAlive < 2) ||
                    (board[y][x].status && isAlive > 3)) {
                    board[y][x].status = 0;
                }

                board[y][x].count++;
            }
        }

    }

    bool IsStop() {
        bool isEquels = true;
        for (int i = 0; i < heigth; i++)    //проверка на то что копия и главная доска одинаковые
        {
            for (int j = 0; j < length; j++)
            {
                if (board[i][j] != boardCopy[i][j]) {
                    isEquels = false;
                    break;
                }
            }
            if (!isEquels) {
                break;
            }
        }
        bool isHaveCell = false;
        for (int i = 0; i < heigth; i++)    //проверка на то что все клетки живые
        {
            for (int j = 0; j < length; j++)
            {
                if (board[i][j].status) {
                    isHaveCell = true;
                }
            }
        }

        if(isEquels || !isHaveCell){
            return true;
        }
        else {
            return false;
        }
    }
    ~Simulation() {//очищаем динамическую память
        for (int i = 0; i < heigth; i++)
        {
            delete[] board[i];
            delete[] boardCopy[i];
        }
        delete[] board;
        delete[] boardCopy;
    }
private:
    struct Cell//струтура клетки, она используется только этим классам, по этому она инкапсулирована в секцию прайват
    {
        int status = 0;
        int count = 0;
        bool isBlock() { return count > 15; }

        bool operator ==(const Cell& other) {
            return other.status == status;
        }

        bool operator !=(const Cell& other) {
            return !(other.status == status);
        }
    };

    int length, heigth; //размеры поля
    Cell** board;       //главная доска
    Cell** boardCopy;   //копия доски
    string cellChar;    //обозначение кдетки

    int coordsX[8] = {-1, -1, -1, 0, 0, 1, 1, 1};   //координаты соседних клеток
    int coordsY[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    bool isValidIndex(int x, int y) {   //проверка на валидность координат
        
       
    }
};