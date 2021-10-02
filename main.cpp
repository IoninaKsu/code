#include <iostream>
#include <windows.h>
#include <ctime>
#include <conio.h>
#include <string>
#include <vector>
//#include <locale>
// using-директива сообщает компилятору, что мы используем все объекты из пространства имен std
using namespace std;

const int N = 10 + 2; // +2 под границы
const int NUM_SHIPS = 10;// всего кораблей

int Ships_id = 1;
int Ships[10] = { 0 };


enum ConsoleColor {
    Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Magenta = 5,
    Brown = 6,
    LightGray = 7,
    DarkGray = 8,
    LightBlue = 9,
    LightGreen = 10,
    LightCyan = 11,
    LightRed = 12,
    LightMagenta = 13,
    Yellow = 14,
    White = 15
};

//Функция gotoxy() помещает курсор текстового экрана в точку с координатами х,у. Если хотя бы одна из координат недействительна, то никаких действий не выполняется.
void gotoxy(int x, int y)
{
    COORD p = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}


//Функция возвращает true, если корабль помещается на карте и false в обратном случае.
bool shipInMap(int x, int y, int dir, int sizeShip)
{
    bool in_map = 1;
    //проверка возможности постановки корабля
    for (int i = 0; i < sizeShip; i++)
    {
        if (x < 1 || y < 1 || x >= N - 1 || y >= N - 1)//кусок корабля в карте
            {
            in_map = 0;
            break;
        }

        //изменение координат в зависимости от направления
        switch (dir)
        {
            case 0:
                x++;
                break;
                case 1:
                    y++;
                    break;
                    case 2:
                        x--;
                        break;
                        case 3:
                            y--;
                            break;
        }
    }
    return in_map;
}


// Возвращает истину если постановка возможна и ложь, если не возможна.
// Функция не записывает корабль в массив, только проверяет возможность установки.
bool shipSettingIsPossible(int map[N][N], int x, int y, int dir, int sizeShip)
{
    bool setting_is_possible = 1;
   //передаётся один корабль??? и проходимся по каждой его палубе
    for (int i = 0; i < sizeShip; i++)
    {

        if (x <= 0 || y <= 0 || x >= N - 1 || y >= N - 1)  //точка не в пределах карты
            {
            setting_is_possible = 0;
            break;
            }

        //обязательно до изменения координат!
        //есои ячейка в поле зрения занята, то флаг=0 => нет возможности постановки корабля
        if (map[x][y] >= 0 ||
        map[x][y + 1] >= 0 ||
        map[x][y - 1] >= 0 ||
        map[x + 1][y] >= 0 ||
        map[x + 1][y + 1] >= 0 ||
        map[x + 1][y - 1] >= 0 ||
        map[x - 1][y] >= 0 ||
        map[x - 1][y + 1] >= 0 ||
        map[x - 1][y - 1] >= 0)
        {
            setting_is_possible = 0;
            break;
        }


        //откуда тут взялся дир???

        //изменение координат в зависимости от направления
        switch (dir)
        {
            case 0:
                x++;
                break;
                case 1:
                    y++;
                    break;
                    case 2:
                        x--;
                        break;
                        case 3:
                            y--;
                            break;
        }
    }
    return setting_is_possible;
}


bool setShipInMap(int map[N][N], int x, int y, int dir, int sizeShip, int id) // ставим один корабль
{
    if (shipSettingIsPossible(map, x, y, dir, sizeShip))  //если корабль не выходит за пределы карты и не соприкасается с другими
    {
        //как от целого корабля переходим к его ячейкам???
        for (int i = 0; i < sizeShip; i++)
        {
            map[x][y] = id;

            switch (dir)
            {
                case 0:
                    x++;
                    break;
                case 1:
                    y++;
                    break;
                case 2:
                     x--;
                     break;
                case 3:
                     y--;
                     break;
            }
        }
        return true;
    }
    return false;
}

void setRandShip(int map[N][N], int sizeShip, int ship_id)
{
    int x, y;//первичная координата корабля
    int dir = 0;//направление

    int count_tact = 0;//кол-во прохождений цикла while

    while (true)
    {

        count_tact++;

        if (count_tact > 1000)
        {
            break;
        }

        // первичная позиция
        x = rand() % (N - 2) + 1;
        y = rand() % (N - 2) + 1;

        // генерация направления
        dir = rand() % 4;


        //почему тут если возврачается тру, то брейк??? ведь если тру, то установка возможна??????????
        //???????
        if (setShipInMap(map, x, y, dir, sizeShip, ship_id))
        {
            break;
        }
    }
}

// в случайном месте и в случайном направлении (горизонтальном или вертикальном) устанавливает корабль
//заданного размера и с заданным номером. Номер корабля проставляется в соответствующие номером ячейки массива.
void setRandShips(int map[N][N], int ships[NUM_SHIPS])
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | LightGreen));

    //проходимся по "гаражу" с кораблями
    //проходимся по каждому кораблю NUM_SHIPS(10)
    for (int i = 0; i < NUM_SHIPS; i++)
    {
        //почему пишем просто map, а не map1
        setRandShip(map, ships[i], i);// генерируются случайные координаты и направление+ ???происходит ли setShipInMap????
    }
}

void mapShow(int map[N][N], int x, int y, int mask[N][N], bool useMask)//рисует обе карты   ??? сначла одну, потом вторую????????   см.1134-1135
//проверяем построчно
{
    for (int i = 0; i < N; i++)
    {
        gotoxy(x, y);// зачем это??

        for (int j = 0; j < N; j++)
        {
            if (mask[j][i] == 1 || useMask == 0)
            {
                if (map[j][i] == -3)// подбитая палуба
                    {
                    HANDLE hConsole5 = GetStdHandle(STD_OUTPUT_HANDLE);
                    SetConsoleTextAttribute(hConsole5, (WORD) ((Black << 4) | White));

                    std::cout << 'X';
                    }
                else if (map[j][i] == -2)// промах(уже стреляли)
                    {
                    HANDLE hConsole4 = GetStdHandle(STD_OUTPUT_HANDLE);
                    SetConsoleTextAttribute(hConsole4, (WORD) ((Black << 4) | LightBlue));

                    std::cout << '.';
                    }
                else if (map[j][i] == -1)// пустая клетка
                    {
                    std::cout << ' ';
                    }
                else if (map[j][i] == -100)// граница поля
                    {
                    HANDLE hConsole2 = GetStdHandle(STD_OUTPUT_HANDLE);
                    SetConsoleTextAttribute(hConsole2, (WORD) ((Black << 4) | LightRed));

                    std::cout << '#';

                    }
                else // корабль
                {
                    HANDLE hConsole1 = GetStdHandle(STD_OUTPUT_HANDLE);
                    SetConsoleTextAttribute(hConsole1, (WORD) ((Black << 4) | LightGreen));

                    std::cout << '@';
                }
            }
            else
            {
                if (map[j][i] == -100)// граница рисуется независимо от маски
                    {
                    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                    SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | LightRed));

                    std::cout << '#';
                    }
                else

                {
                    std::cout << ' ';
                }
            }
        }
        y++;//перевод на новую строку игрик сверху вниз
    }
}

void mapShowInHandMode(int map[N][N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (map[j][i] == -3)
            {
                HANDLE hConsole5 = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole5, (WORD) ((Black << 4) | White));

                std::cout << 'X';
            }
            else if (map[j][i] == -2)
            {
                HANDLE hConsole4 = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole4, (WORD) ((Black << 4) | LightBlue));

                std::cout << '.';
            }
            else if (map[j][i] == -1)
            {
                std::cout << ' ';
            }
            else if (map[j][i] == -100)
            {
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | LightRed));

                std::cout << '#';
            }
            else
            {
                HANDLE hConsole1 = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole1, (WORD) ((Black << 4) | LightGreen));

                std::cout << '@';
            }

        }
        std::cout << endl; //что делает эта строчка??
    }
}

//Корабль никуда не записывается, только отображается на поле.
void shipShowInHandMode(int x, int y, int dir, int sizeShip)
{
    for (int i = 0; i < sizeShip; i++)
    {
        //что такое сайзшип??
        gotoxy(x, y); //курсор ставится на начальные координаты корабля????
        HANDLE hConsole0 = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole0, (WORD) ((Black << 4) | LightGreen));


        std::cout << '@';


        switch (dir)
        {
            case 0:
                x++;
                break;
            case 1:
                y++;
                break;
            case 2:
                 x--;
                 break;
            case 3:
                 y--;
                 break;
        }
    }
}


void handSetShips(int map[N][N], int ships[NUM_SHIPS])
{
    system("cls");//очищает экран консоли

    int ch; //для считывания нажатия клавиш пользователем

    int x = 1, y = 1;
    bool dir = 0;

    //ручная расстановка кораблей
    int i = 0;
    while (i < NUM_SHIPS)//пока не расставили все корабли, i-количество поставленных кораблей
        {
        HANDLE hConsole1 = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole1, (WORD) ((Black << 4) | White));

        gotoxy(0, 12); std::cout << "                                                                 ";
        gotoxy(0, 12); std::cout << "Выбор позиции для корабля - стрелки. Поворот корабля - пробел. Установка - Enter" << std::endl;
        gotoxy(0, 0); //mapShowInHandMode(map) возвращаем курсор для отрисовки карты

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | LightGreen));


        mapShowInHandMode(map); //прорисовка карты игры   ?? как получается, что её сразу две выходит?

        //почему во многих функциях передаём именно мап, а не мап1 или мап2
        shipShowInHandMode(x, y, dir, ships[i]); //просто рисуем корабль, но ене записываем его никуда, а записываетя он в карту игры в SetShipInMap()

        int temp_x = x;
        int temp_y = y;
        int temp_dir = dir;

        ch = _getch();
        if (ch == 224)
        {
            ch = _getch();
        }

        // изменить координаты или направление
        switch (ch)
        {
            case 77://вправо
            x++;
            break;
            case 80://вниз
            y++;
            break;
            case 75://влево
            x--;
            break;
            case 72://вверх
            y--;
            break;
            case 32:// поворот-пробел     //как дир станоится НЕ дир, если у нас 4 направления?
            dir = !dir;
            break;
            case 13:// установка корабля
            if (setShipInMap(map, x, y, dir, ships[i], i))
                i++; //ставим следующий корабль
                break;
        }

        // проверка попадания корабля в поле после изменения координат
        // если не попал то откатить координаты и направление
        if (!shipInMap(x, y, dir, ships[i]))
        {
            x = temp_x;
            y = temp_y;
            dir = temp_dir;
        }

        system("cls");
    }
}

//рисуем область вокруг убитого корабля
void drawAround(int start_x, int start_y, int end_x, int end_y, int map[N][N], int mask[N][N]) {
    for (int i = start_x; i <= end_x; i++) {
        for (int j = start_y; j >= end_y; j--) {
            if (map[i][j] != -3 && map[i][j] != -100){ //проверяем, что точка - не точка убитого корабля и не точка границы поля
                map[i][j] = -2;
                //почем не конкретно маск1 или маск2??
                mask[i][j] = 1;//показываем (видим)
            }
        }
    }
}

//определяем координаты области, в которой не может быть других кораблей
void aroundShot(int map[N][N], int x, int y, int mask[N][N]) {

    int cur_x = x;
    int cur_y = y;
    int start_x = x;
    int start_y = y; //левый верхний угол области
    int end_x = x;
    int end_y = y; //правый нижний угол области

    if (map[x - 1][y] == -3 || map[x + 1][y] == -3) { //если слева или справа если "потопленные" точки, то корабль расположен по горизонтали
        while (map[cur_x][y] == -3) { //ищем крайний левый х корабля
            cur_x--;
        }
        start_x = cur_x;// нашли начальную координату горизонтальной рамки

        cur_x = x;//текущая координата

        while (map[cur_x][y] == -3) { //ищем крайний правый у корабля
            cur_x++;
        }
        end_x = cur_x;//нашли конечную координату горизонтальной рамки

        drawAround(start_x, y + 1, end_x, y - 1, map, mask); //ставим точки
    }

    else if (map[x][y - 1] == -3 || map[x][y + 1] == -3) { //если сверху или снизу если "потопленные" точки, то корабль расположен по вертикали
        while (map[x][cur_y] == -3) { //ищем крайний нижний у корабля
            cur_y--;
        }
        end_y = cur_y; //крайний нижний рамки
        cur_y = y;
        while (map[x][cur_y] == -3) { //ищем крайний верхний у корабля
            cur_y++;
        }
        start_y = cur_y; //крайний верхний рамки
        drawAround(x-1, start_y, x+1, end_y, map, mask); //ставим точки
    }
    else {
        drawAround(x - 1, y + 1, x + 1, y - 1, map, mask); //для случая, когда корабль был однопалубный
    }
}

int shot(int map[N][N], int x, int y, int ships[NUM_SHIPS], int mask[N][N])
{
    int result = 0;

    // выстрел по пустой клетке, промах
    if (map[x][y] == -1)
    {
        map[x][y] = -2; //пустая клетка по которой уже стреляли
        result = 1;
    }
    // выстрел по клетке по которой уже стреляли
    // это условие нужно для правильной работы бота чтобы он не стрелял по клеткам повторно
    else if (map[x][y] == -2 || map[x][y] == -3)
    {
        result = 2;// условный промах (неправильный выстрел)
    }
    // попадание по кораблю
    else if (map[x][y] >= 0)
    {
        ships[map[x][y]]--;

        if (ships[map[x][y]] > 0)
        {
            result = 3; //ранен
        }
        else
        {
            result = 4; //убит
        }

        map[x][y] = -3; //подбитая палуба

        //если убит, прорисовываем вокруг "мёртвую зону"
        if (result == 4) {
            aroundShot(map, x, y, mask);
        }
    }


    //опять. почему не маск1 или маск2, а просто маск
    mask[x][y] = 1;//можно прорисовывать данную ячейку

    return result;
}



//функция выбора ячейки выстрела
int selectTarget(int& x, int& y)  //передаём указатель, чтобы в случае неправильного выбора ячейки, ху вернулись в прежнее место
{

    // int dir = 0;
    // int sizeShip = 4;
    int ch;


    //??
    gotoxy(x, y);



    while (true)
    {

        int temp_x = x;
        int temp_y = y;

        static bool line_bombed = false;
        static bool locator = false;
        static bool triple = false;

        ch = _getch();
        if (ch == 224)//служебная клавиша для правильно
            {
            ch = _getch();//для правильной работы считывания
            }

        //что тут происходит??? координаты чего меняем?


        // изменить координаты
        switch (ch)
        {
            case 77://вправо
            x++;
            break;
            case 80://вниз
            y++;
            break;
            case 75://влево
            x--;
            break;
            case 72://вверх
            y--;
            break;
            case 13:// установка корабля
            return 0;
            break;
            case 9: //tab
                if (line_bombed == false) { //проверяем, использовалась ли бомба
                    line_bombed = true; //если нет, делаем, что да
                    return 1;
                }
                else {//если да, пишем, что бомбу уже использовали
                    gotoxy(0, 12);
                    std::cout << "                                                         \n                        \n                                                                \n                                                                                             ";
                    gotoxy(0, 12);
                    std::cout << "Бомба уже была использована в этой игре. " << endl;

                }
                break;
                case 8:
                    if (locator == false) { //проверяем, использовался ли локатор
                        locator = true; //если нет, делаем, что да
                        return 2;
                    }
                    else { //если да, пишем, что локатор уже использовали
                        gotoxy(0, 12);
                        std::cout << "                                                         \n                        \n                                                                \n                                                                                             ";
                        gotoxy(0, 12);
                        std::cout << "Локатор уже был использован в этой игре. " << endl;
                    }
                    break;
                    case 27://esc
                        if (triple == false) { //проверяем, использовалась ли тройная бомба
                            triple = true;
                            return 3;
                        }
                        else { //если да, пишем, что бомбу уже использовали
                            gotoxy(0, 12);
                            std::cout << "                                                         \n                        \n                                                                \n                                                                                             ";
                            gotoxy(0, 12);
                            std::cout << "Тройная бомба уже была использована в этой игре. " << endl;
                        }
                        break;
        }



             //почему????

        //без этой проверки при выстреле мы выходим за границы поля
        if (!shipInMap(x, y, 1, 1))
        {
            x = temp_x;
            y = temp_y;
        }
        gotoxy(x, y);
    }
}
//почему i j???
void mapInit(int map[N][N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (i == 0 || j == 0 || i == N - 1 || j == N - 1)
            {
                map[j][i] = -100;
            }
            else
            {
                map[j][i] = -1;
            }
        }
    }
}


//true если все корабли потоплены, false в обратном случае.
bool playerIsWin(int ships[NUM_SHIPS])
{
    int s = 0;
    bool win = 0;

    //проверка, если в массиве гараж осталось хоть сколько-то кораблей(жизней), то win=0, пользователь НЕ выиграл
    for (int i = 0; i < NUM_SHIPS; i++)
    {
        s = s + ships[i];
    }

    if (s <= 0)
    {
        win = 1;
    }

    return win;
}



//Функция обнаруживает и убивает один из кораблей на поле. Поиск корабля ведется сверху вниз слева направо.
int shipLocator(int map[N][N], int ships[NUM_SHIPS], int mask[N][N]) {
    int dir = 0; //для направления поиска корабля
    int id; //для хранения индекса корабля
    for (int i = 1; i < N - 1; i++) {
        for (int j = 1; j < N - 1; j++) { //обходим карту, пока не наткнемся на корабль
            if (map[i][j] >= 0) {
                id = map[i][j]; //если нашли корабль, запоминаем его номер
                if (map[i][j + 1] >= 0) { //если в клетке справа от корабля тоже есть палуба, то корабль горизонтальный, иначе - вертикальный (есть палуба внизу, т.к. клетки слева и сверху мы уже проверили)
                    dir = 1;
                }
                ships[id]--; //убираем одну жизнь
                map[i][j] = -3; //помечаем, что ранили
                mask[i][j] = 1; //помечаем, что клетку можно рисовать
                while (ships[id] > 0){      //пока у корабля есть жизни, повторяем предыдущие три строки
                    if (dir) { //смотрим, какой формы корабль и в какую сторону идти
                        j++;
                    }
                    else {
                        i++;
                    }
                    ships[id]--;
                    map[i][j] = -3;
                    mask[i][j] = 1;
                }
                aroundShot(map, i, j, mask); //обрисовываем "мертвую зону" вокруг корабля
                return 5;
            }
        }
    }
}


//Функция проверяет, можно ли применить бомбу, т.е. есть ли на поле три клеточки, в которые еще не стреляли
bool bombPossible(int mask[N][N]) {
    int possibleCount = 0;
    for (int i = 1; i < N - 1; i++) {
        for (int j = 1; j < N - 1; j++) {
            if (mask[i][j] == 0) { //считаем, сколько клеток еще не открыто
                possibleCount++;
                if (possibleCount == 3) { //если нашли хотя бы три такие клетки, можно использовать бомбу
                    return true;
                }
            }
        }
    }
    return false; //если не нашли, то нельзя
}

//Функция - тройная бомба - выстрел в три случайных места на карте
int tripleBomb(int map[N][N], int ships[NUM_SHIPS], int mask[N][N]) {
    int x, y;
    if (bombPossible(mask)) { //если бомбу возможно применить
        int bombs = 0;
        int result;
        while (bombs < 3) { //пока не сделаем три выстрела
            x = rand() % (N - 2) + 1;
            y = rand() % (N - 2) + 1;
            if (mask[y][x] == 0) { //если в эту клетку еще не стреляли
                result = shot(map, x, y, ships, mask); //стреляем
                bombs++; //увеличиваем счетчик выстрела
            }
        }
        return 7; //возвращаем 7 - сигнал, что успешно использовалась тройная бомба
    }
    else {
        return 8; //возвращаем 8 - сигнал, что бомбу применить нельзя :(
    }
}

//стреляем в столбик
int line_bomb(int map[N][N], int x, int y, int ships[NUM_SHIPS], int mask[N][N]) {

    int result;


    for (int j=1; j< N-1; j++){//перебираем все координаты на столбике
        if (mask[j][x] == 0) { //если в эту точку еще не стреляли, то стреляем
            result = shot(map, x, j, ships, mask);
        }
    }
    return 6; //возвращаем 6 - сигнал, что использовалась бомба для особого сообщения

}

int line_bomb2(int map[N][N], int x, int y, int ships[NUM_SHIPS], int mask[N][N]) //треляем в столбик
{

    int result;
    for (int i = 1; i < N - 1; i++) { //перебираем все координаты на линии
        if (mask[y][i] == 0) { //если в эту точку еще не стреляли, то стреляем
            result = shot(map, i, y, ships, mask);
        }
    }
    return 6; //возвращаем 6 - сигнал, что использовалась бомба для особого сообщения

}



//Функция humanTurn осуществляющую стрельбу человека до первого промаха.
//Функция возвращает 1 в случае продолжения игры, 2 в случае полной победы человека.
//После каждого попадания функция проверяет потоплены ли все корабли playerIsWin(ships)
int humanTurn(int map[N][N], int ships[NUM_SHIPS], int mask[N][N])
{
    bool win = false;
    int resultShot = 0;
    int action;
    int x = 1, y = 1;

    HANDLE hConsole1 = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole1, (WORD) ((Black << 4) | White));


    gotoxy(0, 12);
    std::cout << "Ваш ход. Выберите цель, используя клавиши стрелок.\n Выстрел Enter.\n Бомба Tab (простреливает ряд, можно использовать раз за игру).\n Локатор BackSpace (находит и убивает один корабль, можно использовать раз за игру). \n Тройная бомба Esc (стреляет в три случайные точки, можно использовать раз за игру).";

    while (true)
    {
        action = selectTarget(x, y);
        if (action == 1) { //если выбрана бомба, зовем бомбу


            resultShot=line_bomb(map, x, y, ships, mask);
            resultShot=line_bomb2(map, x, y, ships, mask);
        }
        else if (action == 2) {
            resultShot = shipLocator(map, ships, mask);
        }
        else if (action == 3) {
            resultShot = tripleBomb(map, ships, mask);
        }
        else { //если выбран выстрел, стреляем
            resultShot = shot(map, x, y, ships, mask);//возвращаем значение функции выстрела в переменную resultShot
        }

        gotoxy(0, 12);
        std::cout << "                                                                 ";
        gotoxy(0, 12);


        //resultShot-


        if (resultShot == 1 || resultShot == 2)
        {
            HANDLE hConsole2 = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole2, (WORD) ((Black << 4) | White));

            std::cout << "Промах. " << endl;
            break;
        }
        else if (resultShot == 3)
        {
            HANDLE hConsole3 = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole3, (WORD) ((Black << 4) | White));

            std::cout << "Ранен. " << endl;
        }
        else if (resultShot == 4)
        {
            HANDLE hConsole4 = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole4, (WORD) ((Black << 4) | White));

            std::cout << "Убит. ";

            //потоплены ли все корабли?
            if (playerIsWin(ships))
            {
                return 2; // победа
            }
        }
        else if (resultShot == 5) //если был использован локатор, пишем, что его использовали
            {
            HANDLE hConsole4 = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole4, (WORD)((Black << 4) | White));

            std::cout << "Был обнаружен и убит корабль. ";
            if (playerIsWin(ships)) //проверяем, не убили ли мы локатором последний корабль
                {
                    return 2; // победа
                }
            break;
            }
        else if (resultShot == 6) //если была использована бомба, пишем, что ее использовали
            {
            HANDLE hConsole4 = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole4, (WORD)((Black << 4) | White));

            std::cout << "Использована бомба. ";
            if (playerIsWin(ships)) //проверяем, не убили ли мы бомбой последний корабль
                {
                return 2; // победа
                }
            break;
            }
        else if (resultShot == 7) //если была успешно использована бомба, пишем, что ее использовали
            {
            HANDLE hConsole4 = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole4, (WORD)((Black << 4) | White));

            std::cout << "Использована тройная бомба. ";
            if (playerIsWin(ships)) //проверяем, не убили ли мы бомбой последний корабль
                {
                return 2; // победа
                }
            break;
            }
        else if (resultShot == 8) {
            HANDLE hConsole4 = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole4, (WORD)((Black << 4) | White));

            std::cout << "Невозможно применить тройную бомбу. ";
        }
        Sleep(1000);
        mapShow(map, 0, 0, mask, 1);



    }

    return 1; // промах
}

//изменеие координат прицела бота в зависимости от направления.
// ложь если менять напрвление поиска не надо,истина если менять направление поиска надо
bool changeCoord(int map[N][N], int dir, int& x, int& y)
{
    //как тут меняется дир...
    bool changeDir = 0;

    int temp_x = x;
    int temp_y = y;

    switch (dir)
    {
        case 0:
            x++;
            break;
        case 1:
             y++;
             break;
        case 2:
             x--;
             break;
        case 3:
              y--;
              break;
    }

    //если попали на границу, то возвращаемся в прежнюю ячейку
    if (map[x][y] == -100)
    {
        x = temp_x;
        y = temp_y;
        changeDir = 1;
    }

    return changeDir;
}

// инициализация вектора направлений стрельбы для бота
// в вектор помещаются возможные варианты направлений (влево, вправо, вверх, вниз от места попадания),
// по которым бот будет искать оставшиеся палубы корабля

void initDirsVec(vector <int>& dirs)
{

    dirs.clear();
    dirs.push_back(3); //добавляет в конец вектора значение
    dirs.push_back(2);
    dirs.push_back(1);
    dirs.push_back(0);
    //3 2 1 0
}

// получение из вектора очередного направления в котором бот будет искать оставшиеся палубы корабля
int getDirFromVec(vector <int>& dirs)
{
    int dir = 0;
    if (!dirs.empty())
    {
        dir = dirs[dirs.size() - 1];
        dirs.pop_back();//удалили то, что взяли
    }
    return dir;
}

//проверяем, чтобы бот не стрелял вокруг уже убитых кораблей
int checkMask(int mask[N][N], int x, int y) {
    if ((mask[x][y + 1] == -3) || (mask[x][y - 1] == -3) || (mask[x + 1][y] == -3) || (mask[x - 1][y] == -3) || (mask[x - 1][y - 1] == -3) || (mask[x + 1][y - 1] == -3) || (mask[x - 1][y + 1] == -3) || (mask[x + 1][y + 1] == -3)) {
        return 0;
    }
    return 1;
}

int botTurn(int map[N][N], int ships[NUM_SHIPS], int mask[N][N])
{
    // координаты палубы по которой попали первой
    static int firstHitX = 0;  //почему статик?? потому что при следующем заходе в данную функцию пременная сохраняется такой, какой и была?
    static int firstHitY = 0;

    // режим работы бота
    static int mode = 0;

    // текущие координаты для стрельбы
    static int x = 0;
    static int y = 0;

    // текущее направление стрельбы
    static int dir = 0;

    //вектор возможных направлений
    static vector <int> dirs;

    int resultShot = 0;
    bool win = 0;

    do
    {
        //случайный режим стрельбы
        if (mode == 0)
        {
            do // стрелять до тех пор, пока результат условный промах (пока resultShot 1 или 2)
            {
                x = rand() % (N - 2) + 1;
                y = rand() % (N - 2) + 1;

                //проверка, чтобы бот не стрелял вокруг уже убитых кораблей
                if (checkMask(map, x, y) == 1) {

                    resultShot = shot(map, x, y, ships, mask);

                }
                else {
                    resultShot = 2; //если выпали на вокруг уже убитого корабля, то условный промах(там стрелять нельзя)
                }

            } while (resultShot == 2);

            if (resultShot == 1)
            {

                HANDLE hConsole1 = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole1, (WORD) ((Black << 4) | White));

                std::cout << "Промах. ";
            }
            else if (resultShot == 3)// ранен
                {
                mode = 1; // переход в режим поиска остальных палуб

                initDirsVec(dirs);//что значит передаём указатель??

                firstHitX = x;
                firstHitY = y;

                dir = getDirFromVec(dirs);

                HANDLE hConsole2 = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole2, (WORD) ((Black << 4) | White));

                std::cout << "Ранен. ";
                }
            else if (resultShot == 4)
            {
                HANDLE hConsole3 = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole3, (WORD) ((Black << 4) | White));

                std::cout << "Убит. ";
                if (playerIsWin(ships))
                {
                    return 2;
                }
            }
        }
        //режим стрельбы после первого попадания в корабль
        else if (mode == 1)
        {

            bool changeDir = changeCoord(map, dir, x, y);

            //изменить направление поиска палуб
            if (changeDir)
            {
                dir = getDirFromVec(dirs);

                // восстанавливаем координаты первого попадания по кораблю и от них ищем в новом направлении
                x = firstHitX;
                y = firstHitY;

                // смена направления происходит при столкновении с границей
                continue;

            }

            // стрельба
            resultShot = shot(map, x, y, ships, mask);

            if (resultShot == 2) {
                dir = getDirFromVec(dirs);

                x = firstHitX;
                y = firstHitY;

                continue;
            }

            if (resultShot == 1) // промах
                {
                dir = getDirFromVec(dirs);

                x = firstHitX;
                y = firstHitY;

                HANDLE hConsole8 = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole8, (WORD) ((Black << 4) | White));

                std::cout << "Промах. ";

                return 1;
                }
            else if (resultShot == 3) // ранен
                {
                HANDLE hConsole6 = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole6, (WORD) ((Black << 4) | White));

                std::cout << "Ранен. ";
                }
            else if (resultShot == 4) // убит
                {
                mode = 0;//возвращаемся в случайный режим нахождения палуб

                initDirsVec(dirs);

                HANDLE hConsole00 = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole00, (WORD) ((Black << 4) | White));

                std::cout << "Убит. ";
                if (playerIsWin(ships))
                {
                    return 2;
                }
                }
        }
        Sleep(1000);
    } while (resultShot != 1);

    return 1;
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);//для кодировки
    setlocale(LC_ALL, "ru_RU.UTF-8");
    // setlocale(LC_ALL, "rus");
    srand(time(0));// позволяет запускать программу несколько раз с различными последовательностями случайных чисел.

    while (true)
    {
        //Игровое поле размером 10 на 10 представляется двумерным массивом 12 на 12,
        //так как крайние столбцы и строки будут содержать значения обозначающие границы.
        int map1[N][N] = { 0 }; // игровое поле человека
        int map2[N][N] = { 0 }; // игровое поле компьютера

        mapInit(map1);//инициализация карты человека (границы)
        mapInit(map2);//инициализация карты игрока

        //Массивы содержат информацию о количестве кораблей и количестве палуб каждого корабля.
        //Размер массивов определяется константой NUM_SHIPS.
        //Индекс массива определяет номер корабля, а значение элемента его размер.
        //Поэтому корабль с номером 0 – 4-х палубный, а с номером 1 – 3-палубный
        int ships1[NUM_SHIPS] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 }; //корабли человека
        int ships2[NUM_SHIPS] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 }; //id

        int mask1[N][N] = { 0 };//?
        int mask2[N][N] = { 0 };

        std::cout << "Расставить корабли автоматически(1) или вручную(0)? ";

        int randOrHandMode = 2;

        std::cin >> randOrHandMode;

        system("cls");

        if (randOrHandMode == 1)
        {
            setRandShips(map1, ships1);
        }
        else
        {
            handSetShips(map1, ships1);
        }

        setRandShips(map2, ships2);

        int x = 0, y = 0;


        int winHuman = 1;
        int winBot = 1;

        bool turn = rand() % 2;

        // цикл раунда игры
        while (true)
        {
            mapShow(map1, 13, 0, mask1, 0);//показываем маску наша карта
            mapShow(map2, 0, 0, mask2, 1);// карта противника

            if (turn == 1)
            {
                winHuman = humanTurn(map2, ships2, mask2);
            }
            else
            {
                gotoxy(0,13);

                HANDLE hConsole1 = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole1, (WORD) ((Black << 4) | White));

                std::cout << endl << "Ход компьютера. ";
                Sleep(1000);

                winBot = botTurn(map1, ships1, mask1);
            }

            if (winHuman == 2)
            {
                mapShow(map1, 13, 0, mask1, 0);//показваем маску
                mapShow(map2, 0, 0, mask2, 1);
                gotoxy(0,13);

                HANDLE hConsole2 = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole2, (WORD) ((Black << 4) | White));

                std::cout << "Вы победили!!!" << endl;
                system("pause");
                break;
            }
            else if (winBot == 2)
            {
                mapShow(map1, 13, 0, mask1, 0);//показваем маску
                mapShow(map2, 0, 0, mask2, 1);
                gotoxy(0,13);

                HANDLE hConsole4 = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole4, (WORD) ((Black << 4) | White));

                std::cout << "Вы проиграли." << endl;
                system("pause");
                break;
            }

            Sleep(1000);
            system("cls");

            turn = !turn;
        }

        _getch();
        system("cls");
    }

    system("pause");
    return 0;
}