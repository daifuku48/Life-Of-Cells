#include <iostream>
#include "Simulation.h"
#include "windows.h"
using namespace std;

int main() {
    cout << "Enter length and heigth(rec: 20x20): ";
    int l, h;
    cin >> l >> h;

    Simulation simul(l, h, false, "<>");   //создаем симуляцию, указывая размеры поля

    simul.ShowSettingBoard();   //выводим доску с клетками в виде их цифрового статуса: 0 -мертва 1 - жива
    cout << "\nDo you want to random filling(y/n)?";    //хотим ли мы заполнять случаным образом все поле?*
    char ch; cin >> ch;                                 
    if (ch == 'y') {
        simul.RandomFill();         //заполняется автоматически
        simul.ShowSettingBoard();   //выводим доску с клетками в виде их цифрового статуса: 0 -мертва 1 - жива
        cout << "Press key for start\n";    //*для продолжения нажмите кнопку*
        cin.get();  //ожидание нажатия на кнопку


    }
    else {                  //иначе заполняем вручную
        while (true) {      //вечный цикл
            simul.ShowSettingBoard();   //выводим на экран доску с цифровым статусом
            cout << "Enter the x y for the alive cell(-1 -1 => the end): ";
            int x, y;
            cin >> x >> y;  //вводим х у
            if (x == -1 || y == -1) {   //если хотя бы один параметр равен -1, перестаем заполнять
                break;                  //выходим из цикла
            }
            simul.SetAliveCell(x, y);   //устанавливаем клетку как живую

            system("cls");      //очистка экрана
        }

    }
    int count = 0;  //счетчик шагов
    while (!simul.IsStop()) {   //пока можем двигаться
        simul.Drow();           //рисуем доску с клетками
        simul.Cals();           //расчитываем какая клетка умирает, какая оживает и можм ли мы продолжать
        cout << "\n\t\t\tSTEPS: " << ++count;   //выводим кол-во шагов на экран
        if (!simul.IsStop()) {  //если больше двигаться не можем не очищаем консоль
            Sleep(100); 
            system("cls");  //очистка консоли
        }
    }
    cin.get();

    return 0;
}