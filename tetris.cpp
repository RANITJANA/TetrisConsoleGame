#include <bits/stdc++.h>
#include <windows.h>
#include <chrono>
#include <conio.h>
using namespace std;

int color[16] = {31, 91, 32, 33, 92, 35, 93, 36, 94, 37, 95, 96, 34, 97};

// height and width of our playground
#define height 27
#define width 18

// falling point
int posX = 1, posY = 7;

// four subBoxes
pair<int, int> posBoxes[4] = {{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}};
pair<int, int> nextBoxes[4] = {{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}};
multiset<pair<int, int>> s;

unordered_map<int, int> boxCollection[height];

char Shapes[7] = {'O', 'I', 'S', 'Z', 'L', 'J', 'T'};
int current_piece_index = -1;

// score count;
int score = 0;

// fast forward
bool able = false;

// previous falling object time
time_t pastUpdateTime;

// function to display our map
void display(char ch)
{

    cout << "\n";

    for (int i = 0; i < height; i++)
    {
        cout << "\t";
        for (int j = 0; j < width; j++)
        {
            if (i == 0 && j == 0)
            {
                cout << char(201);
            }
            else if (i == height - 1 && j == 0)
            {
                cout << char(200);
            }
            else if (i == 0 && j == width - 1)
            {
                cout << char(187);
            }
            else if (i == height - 1 && j == width - 1)
            {
                cout << char(188);
            }
            else if (i == 2 && j == 0)
            {
                cout << char(204);
            }
            else if (i == 2 && j == width - 1)
            {
                cout << char(185);
            }
            else if (i == 0 || i == height - 1 || i == 2)
            {
                cout << char(205);
            }
            else if (j == 0 || j == width - 1)
            {
                cout << char(186);
            }

            else if (i == 1 && j == 5)
            {
                cout << "\033[1m\033[94m"
                     << " TETRIS "
                     << "\033[0m";
                j += 7;
            }
            else if (i == 1)
            {
                cout << " ";
            }
            else if (i == 2)
            {
                cout << char(178);
            }
            else
            {
                auto it = boxCollection[i].find(j);
                if (it != boxCollection[i].end())
                {
                    if (ch != ' ')
                    {
                        cout << "\033[" << it->second << "m" << ch << "\033[0m";
                    }
                    else
                    {
                        cout << "\033[" << it->second << "m" << char(219) << "\033[0m";
                    }
                }
                else
                    cout << " ";
            }
        }

        // right side instructions.
        char top_left = char(201), top_bottom = char(205), top_right = char(187), left_right = char(186), left_bottom = char(200), right_bottom = char(188);
        if (i == 3 || i == 11)
        {
            cout << "    " << top_left;
            for (int i = 0; i < 17; i++)
            {
                cout << top_bottom;
            }
            cout << top_right;
        }
        else if (i == 4)
        {
            cout << "    " << left_right;
            cout << "   Next Piece";
            cout << "    " << left_right;
        }
        else if (i == 5)
        {
            cout << "    " << char(204);
            for (int j = 0; j < 17; j++)
                cout << top_bottom;
            cout << char(185);
        }
        else if (i == 6 || i == 7 || i == 8 || i == 9)
        {
            cout << "    " << left_right << "       ";

            for (int j = 0; j < 4; j++)
            {
                if (s.find({i - 7, j - 1}) != s.end())
                    cout << char(219);
                else
                    cout << " ";
            }
            cout << "      " << left_right;
        }
        else if (i == 10)
        {
            cout << "    " << left_bottom;
            for (int j = 0; j < 17; j++)
                cout << top_bottom;
            cout << right_bottom;
        }
        else if (i == 12)
        {
            cout << "    " << left_right << "  GAME CONTROLS. " << left_right;
        }
        else if (i == 13)
        {
            cout << "    " << char(204);
            for (int i = 0; i < 17; i++)
            {
                cout << top_bottom;
            }
            cout << char(185);
        }
        else if (i == 14)
        {
            cout << "    " << left_right << "   Left :   " << char(17) << "    " << left_right;
        }
        else if (i == 15)
        {
            cout << "    " << left_right << "  Right :   " << char(16) << "    " << left_right;
        }
        else if (i == 16)
        {
            cout << "    " << left_right << "   Fast : Space. " << left_right;
        }
        else if (i == 17)
        {
            cout << "    " << left_right << " Rotate : ENTER. " << left_right;
        }
        else if (i == 18)
        {
            cout << "    " << left_bottom;
            for (int i = 0; i < 17; i++)
            {
                cout << top_bottom;
            }
            cout << right_bottom;
        }
        else if (i == 21)
        {
            cout << "   \t  \033[41m\033[1m\033[37m Score : " << score << " \033[0m\033[0m\033[0m";
        }
        cout << endl;
    }
}

// generate random shapes
void genRandomShapes()
{
    srand(time(NULL));
    int getIndex = rand() % 7;

    char ch = Shapes[getIndex];
    current_piece_index = getIndex;

    switch (ch)
    {
    case 'O':

        nextBoxes[0] = {posX, posY + 1};
        nextBoxes[1] = {posX, posY};
        nextBoxes[2] = {posX + 1, posY};
        nextBoxes[3] = {posX + 1, posY + 1};
        break;

    case 'I':

        nextBoxes[0] = {posX - 1, posY};
        nextBoxes[1] = {posX, posY};
        nextBoxes[2] = {posX + 1, posY};
        nextBoxes[3] = {posX + 2, posY};
        break;

    case 'S':

        nextBoxes[0] = {posX, posY + 1};
        nextBoxes[1] = {posX, posY};
        nextBoxes[2] = {posX + 1, posY};
        nextBoxes[3] = {posX + 1, posY - 1};
        break;

    case 'Z':

        nextBoxes[0] = {posX, posY - 1};
        nextBoxes[1] = {posX, posY};
        nextBoxes[2] = {posX + 1, posY};
        nextBoxes[3] = {posX + 1, posY + 1};
        break;

    case 'L':

        nextBoxes[0] = {posX - 1, posY};
        nextBoxes[1] = {posX, posY};
        nextBoxes[2] = {posX + 1, posY};
        nextBoxes[3] = {posX + 1, posY + 1};
        break;

    case 'J':
        nextBoxes[0] = {posX - 1, posY};
        nextBoxes[1] = {posX, posY};
        nextBoxes[2] = {posX + 1, posY};
        nextBoxes[3] = {posX + 1, posY - 1};
        break;

    case 'T':

        nextBoxes[0] = {posX, posY - 1};
        nextBoxes[1] = {posX, posY};
        nextBoxes[2] = {posX, posY + 1};
        nextBoxes[3] = {posX + 1, posY};
        break;
    }
}

// add those position into our display
void fixShapes()
{
    for (int i = 0; i < 4; i++)
    {
        int X = posBoxes[i].first;
        int Y = posBoxes[i].second;
        srand(time(NULL));
        boxCollection[X].insert({Y, color[rand() % 14]});
    }
}

// logic to move our current piece to left
void left_movement()
{

    for (int i = 0; i < 4; i++)
    {
        int X = posBoxes[i].first;
        int Y = posBoxes[i].second;
        if (Y - 1 == 0)
        {
            return;
        }
        if (boxCollection[X].count(Y - 1))
        {
            bool flag = true;
            for (int j = 0; j < 4; j++)
            {
                if (posBoxes[j].second == Y - 1 && posBoxes[j].first == X)
                {
                    flag = false;
                    break;
                }
            }
            if (flag)
                return;
        }
    }
    int colorValue = 0;
    // for eraseing previous valus
    for (int i = 0; i < 4; i++)
    {
        auto it = posBoxes[i];
        int X = it.first, Y = it.second;
        colorValue = boxCollection[X][Y];
        posBoxes[i].second--;
        boxCollection[X].erase(Y);
    }

    // for new values
    for (int i = 0; i < 4; i++)
    {
        auto it = posBoxes[i];
        int X = it.first, Y = it.second;
        boxCollection[X].insert({Y, colorValue});
    }
}

// logic to move our current piece to right
void right_movemenet()
{
    for (int i = 0; i < 4; i++)
    {
        int X = posBoxes[i].first;
        int Y = posBoxes[i].second;
        if (Y + 1 == width - 1)
        {
            return;
        }
        if (boxCollection[X].count(Y + 1))
        {
            bool flag = true;
            for (int j = 0; j < 4; j++)
            {
                if (posBoxes[j].second == Y + 1 && posBoxes[j].first == X)
                {
                    flag = false;
                    break;
                }
            }
            if (flag)
                return;
        }
    }
    int colorValue = 0;
    // for eraseing previous valus
    for (int i = 0; i < 4; i++)
    {
        auto it = posBoxes[i];
        int X = it.first, Y = it.second;
        colorValue = boxCollection[X][Y];
        posBoxes[i].second++;
        boxCollection[X].erase(Y);
    }
    // for new values
    for (int i = 0; i < 4; i++)
    {
        auto it = posBoxes[i];
        int X = it.first, Y = it.second;
        boxCollection[X].insert({Y, colorValue});
    }
}

// rotation of pices
void rotateI();
void rotateS();
void rotateZ();
void rotateL();
void rotateJ();
void rotateT();

void rotate()
{
    char cp = Shapes[current_piece_index];
    switch (cp)
    {
    case 'I':
        rotateI();
        break;
    case 'S':
        rotateS();
        break;
    case 'Z':
        rotateZ();
        break;
    case 'L':
        rotateL();
        break;
    case 'J':
        rotateJ();
        break;
    case 'T':
        rotateT();
        break;
    }

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
    display(' ');
}

// latest values will be assigned here after rotation
inline void updateAfterRotaion(pair<int, int> tempPos[], int colorValue)
{
    for (int i = 0; i < 4; i++)
    {
        boxCollection[posBoxes[i].first].erase(posBoxes[i].second);
    }
    for (int i = 0; i < 4; i++)
    {
        posBoxes[i] = tempPos[i];
        boxCollection[tempPos[i].first].insert({tempPos[i].second, colorValue});
    }
}

// rotation of I shaped object
void rotateI()
{
    pair<int, int> tempPos[4];

    auto secondBox = posBoxes[1];

    int colorValue = boxCollection[secondBox.first][secondBox.second];

    if (posBoxes[0].first != posBoxes[3].first)
    {

        tempPos[0] = {secondBox.first, secondBox.second + 1};
        tempPos[1] = {secondBox.first, secondBox.second};
        tempPos[2] = {secondBox.first, secondBox.second - 1};
        tempPos[3] = {secondBox.first, secondBox.second - 2};

        if (tempPos[0].second > width - 2 || boxCollection[tempPos[0].first].find(tempPos[0].second) != boxCollection[tempPos[0].first].end())
        {
            for (int j = 0; j < 4; j++)
            {
                tempPos[j].second--;
            }
        }
        for (int i = 2; i <= 3; i++)
        {
            pair<int, int> position = tempPos[i];
            if (position.second < 1 || boxCollection[position.first].find(position.second) != boxCollection[position.first].end())
            {
                for (int j = 0; j < 4; j++)
                {
                    tempPos[j].second++;
                }
                if (boxCollection[tempPos[0].first].find(tempPos[0].second) != boxCollection[tempPos[0].first].end())
                    return;
            }
        }

        if (tempPos[0].second > width - 2)
        {
            return;
        }
    }
    else
    {

        tempPos[0] = {secondBox.first - 1, secondBox.second};
        tempPos[1] = {secondBox.first, secondBox.second};
        tempPos[2] = {secondBox.first + 1, secondBox.second};
        tempPos[3] = {secondBox.first + 2, secondBox.second};

        if (boxCollection[tempPos[0].first].find(tempPos[0].second) != boxCollection[tempPos[0].first].end())
        {
            for (int j = 0; j < 4; j++)
            {
                tempPos[j].first++;
            }
        }
        for (int i = 2; i <= 3; i++)
        {
            pair<int, int> position = tempPos[i];
            if (position.first > height - 2 || boxCollection[position.first].find(position.second) != boxCollection[position.first].end())
            {
                for (int j = 0; j < 4; j++)
                {
                    tempPos[j].first--;
                }
                if (boxCollection[tempPos[0].first].find(tempPos[0].second) != boxCollection[tempPos[0].first].end())
                    return;
            }
        }

        if (tempPos[0].first <= 3)
        {
            return;
        }
    }
    // update those values
    updateAfterRotaion(tempPos, colorValue);
}

// Rotation of S shaped object
void rotateS()
{
    pair<int, int> tempPos[4];

    auto secondBox = posBoxes[1];
    int colorValue = boxCollection[secondBox.first][secondBox.second];

    if (posBoxes[1].first < posBoxes[2].first)
    {
        tempPos[0] = {secondBox.first + 1, secondBox.second + 1};
        tempPos[1] = {secondBox.first, secondBox.second + 1};
        tempPos[2] = {secondBox.first, secondBox.second};
        tempPos[3] = {secondBox.first - 1, secondBox.second};

        if (tempPos[0].first > height - 2 || boxCollection[tempPos[0].first].find(tempPos[0].second) != boxCollection[tempPos[0].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[0].first--;
            }
        }
        if (boxCollection[tempPos[3].first].find(tempPos[3].second) != boxCollection[tempPos[3].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[0].first++;
            }
        }

        if (tempPos[0].first > height - 2 || boxCollection[tempPos[0].first].find(tempPos[0].second) != boxCollection[tempPos[0].first].end())
            return;
    }
    else
    {
        tempPos[0] = secondBox;
        tempPos[1] = {secondBox.first, secondBox.second - 1};
        tempPos[2] = {secondBox.first + 1, secondBox.second - 1};
        tempPos[3] = {secondBox.first + 1, secondBox.second - 2};

        if (boxCollection[tempPos[2].first].find(tempPos[2].second) != boxCollection[tempPos[2].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].first--;
            }
        }
        if (tempPos[3].second < 1 || boxCollection[tempPos[3].first].find(tempPos[3].second) != boxCollection[tempPos[3].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].second++;
            }
        }
    }
    // update those values
    updateAfterRotaion(tempPos, colorValue);
}

// rotaion of Z shaped object
void rotateZ()
{

    pair<int, int> tempPos[4];

    auto secondBox = posBoxes[1];
    int colorValue = boxCollection[secondBox.first][secondBox.second];

    if (posBoxes[1].first < posBoxes[2].first)
    {
        tempPos[0] = {secondBox.first - 1, secondBox.second + 1};
        tempPos[1] = {secondBox.first, secondBox.second + 1};
        tempPos[2] = {secondBox.first, secondBox.second};
        tempPos[3] = {secondBox.first + 1, secondBox.second};

        if (tempPos[1].second > width - 2 || boxCollection[tempPos[1].first].find(tempPos[1].second) != boxCollection[tempPos[1].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].second--;
            }
        }
        if (boxCollection[tempPos[0].first].find(tempPos[0].second) != boxCollection[tempPos[0].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].first++;
            }
        }

        if (tempPos[3].first > height - 2)
            return;
    }
    else
    {
        tempPos[0] = {secondBox.first, secondBox.second - 2};
        tempPos[1] = {secondBox.first, secondBox.second - 1};
        tempPos[2] = {secondBox.first + 1, secondBox.second - 1};
        tempPos[3] = {secondBox.first + 1, secondBox.second};

        if (tempPos[0].second < 1 || boxCollection[tempPos[0].first].find(tempPos[0].second) != boxCollection[tempPos[0].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].second++;
            }
        }
        if (tempPos[3].second > width - 2 || boxCollection[tempPos[3].first].find(tempPos[3].second) != boxCollection[tempPos[3].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].second--;
            }
        }
        if (tempPos[1].second < 1)
            return;
    }
    // update those values
    updateAfterRotaion(tempPos, colorValue);
}

// rotation of L shaped piece
void rotateL()
{
    pair<int, int> tempPos[4];

    auto secondBox = posBoxes[1];
    int colorValue = boxCollection[secondBox.first][secondBox.second];

    if (posBoxes[1].first < posBoxes[2].first)
    {
        tempPos[0] = {secondBox.first, secondBox.second + 1};
        tempPos[1] = {secondBox.first, secondBox.second};
        tempPos[2] = {secondBox.first, secondBox.second - 1};
        tempPos[3] = {secondBox.first + 1, secondBox.second - 1};

        // right side collision
        if (boxCollection[tempPos[0].first].find(tempPos[0].second) != boxCollection[tempPos[0].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].second--;
            }
        }

        // left side collision
        if (tempPos[2].second < 1 || tempPos[3].second < 1)
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].second++;
            }
        }
        else if (boxCollection[tempPos[2].first].find(tempPos[2].second) != boxCollection[tempPos[2].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].second++;
            }
        }
        else if (boxCollection[tempPos[3].first].find(tempPos[3].second) != boxCollection[tempPos[3].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].second++;
            }
        }

        // check again in case still invalid
        if (boxCollection[tempPos[0].first].find(tempPos[0].second) != boxCollection[tempPos[0].first].end())
        {
            return;
        }
    }
    else if (posBoxes[1].first > posBoxes[2].first)
    {
        tempPos[0] = {secondBox.first, secondBox.second - 1};
        tempPos[1] = secondBox;
        tempPos[2] = {secondBox.first, secondBox.second + 1};
        tempPos[3] = {secondBox.first - 1, secondBox.second + 1};

        // left collision
        if (boxCollection[tempPos[0].first].find(tempPos[0].second) != boxCollection[tempPos[0].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].second++;
            }
        }

        // right side collision
        if (tempPos[2].second > width - 2)
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].second--;
            }
        }
        else if (boxCollection[tempPos[2].first].find(tempPos[2].second) != boxCollection[tempPos[2].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].second--;
            }
        }
        else if (boxCollection[tempPos[3].first].find(tempPos[3].second) != boxCollection[tempPos[3].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].second--;
            }
        }

        // check again in case still invalid
        if (boxCollection[tempPos[0].first].find(tempPos[0].second) != boxCollection[tempPos[0].first].end())
        {
            return;
        }
    }
    else if (posBoxes[1].second < posBoxes[2].second)
    {
        tempPos[0] = {secondBox.first - 1, secondBox.second};
        tempPos[1] = secondBox;
        tempPos[2] = {secondBox.first + 1, secondBox.second};
        tempPos[3] = {secondBox.first + 1, secondBox.second + 1};

        // top collision
        if (boxCollection[tempPos[0].first].find(tempPos[0].second) != boxCollection[tempPos[0].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].first++;
            }
        }

        // bottom collision
        if (boxCollection[tempPos[2].first].find(tempPos[2].second) != boxCollection[tempPos[2].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].first--;
            }
        }
        else if (boxCollection[tempPos[3].first].find(tempPos[3].second) != boxCollection[tempPos[3].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].first--;
            }
        }

        // final check
        if (boxCollection[tempPos[0].first].find(tempPos[0].second) != boxCollection[tempPos[0].first].end())
        {
            return;
        }
    }
    else
    {
        tempPos[0] = {secondBox.first + 1, secondBox.second};
        tempPos[1] = secondBox;
        tempPos[2] = {secondBox.first - 1, secondBox.second};
        tempPos[3] = {secondBox.first - 1, secondBox.second - 1};

        // bottom collision
        if (boxCollection[tempPos[0].first].find(tempPos[0].second) != boxCollection[tempPos[0].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].first--;
            }
        }

        // top collision
        if (boxCollection[tempPos[2].first].find(tempPos[2].second) != boxCollection[tempPos[2].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].first++;
            }
        }
        else if (boxCollection[tempPos[3].first].find(tempPos[3].second) != boxCollection[tempPos[3].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].first++;
            }
        }

        // final check
        if (boxCollection[tempPos[0].first].find(tempPos[0].second) != boxCollection[tempPos[0].first].end())
        {
            return;
        }
    }
    updateAfterRotaion(tempPos, colorValue);
}

// rotation of J shaped piece
void rotateJ()
{
    pair<int, int> tempPos[4];

    auto secondBox = posBoxes[1];
    int colorValue = boxCollection[secondBox.first][secondBox.second];

    if (posBoxes[1].first < posBoxes[2].first)
    {
        tempPos[0] = {secondBox.first, secondBox.second + 1};
        tempPos[1] = {secondBox.first, secondBox.second};
        tempPos[2] = {secondBox.first, secondBox.second - 1};
        tempPos[3] = {secondBox.first - 1, secondBox.second - 1};

        // left side collision
        if (tempPos[2].second < 1)
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].second++;
            }
        }
        else if (boxCollection[tempPos[2].first].find(tempPos[2].second) != boxCollection[tempPos[2].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].second++;
            }
        }
        else if (boxCollection[tempPos[3].first].find(tempPos[3].second) != boxCollection[tempPos[3].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].second++;
            }
        }
        // right side collision
        if (tempPos[0].second > width - 2 || boxCollection[tempPos[0].first].find(tempPos[0].second) != boxCollection[tempPos[0].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].second--;
            }
        }

        // check again in case still invalid
        if (boxCollection[tempPos[2].first].find(tempPos[2].second) != boxCollection[tempPos[2].first].end())
        {
            return;
        }
        if (boxCollection[tempPos[3].first].find(tempPos[3].second) != boxCollection[tempPos[3].first].end())
        {
            return;
        }
    }
    else if (posBoxes[1].first > posBoxes[2].first)
    {
        tempPos[0] = {secondBox.first, secondBox.second - 1};
        tempPos[1] = secondBox;
        tempPos[2] = {secondBox.first, secondBox.second + 1};
        tempPos[3] = {secondBox.first + 1, secondBox.second + 1};

        // left collision
        if (tempPos[0].second < 1 || boxCollection[tempPos[0].first].find(tempPos[0].second) != boxCollection[tempPos[0].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].second++;
            }
            // check again in case still invalid
            if (boxCollection[tempPos[2].first].find(tempPos[2].second) != boxCollection[tempPos[2].first].end())
            {
                return;
            }
            if (boxCollection[tempPos[3].first].find(tempPos[3].second) != boxCollection[tempPos[3].first].end())
            {
                return;
            }
        }
        else if (boxCollection[tempPos[3].first].find(tempPos[3].second) != boxCollection[tempPos[3].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].first--;
            }
            // check again in case still invalid
            if (tempPos[0].second < 1 || boxCollection[tempPos[0].first].find(tempPos[0].second) != boxCollection[tempPos[0].first].end())
            {
                return;
            }
        }
        if (boxCollection[tempPos[2].first].find(tempPos[2].second) != boxCollection[tempPos[2].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].second--;
            }
            // check again in case still invalid
            if (tempPos[0].second < 1 || boxCollection[tempPos[0].first].find(tempPos[0].second) != boxCollection[tempPos[0].first].end())
            {
                return;
            }
        }

        // check again in case still invalid
    }
    else if (posBoxes[1].second < posBoxes[2].second)
    {
        tempPos[0] = {secondBox.first - 1, secondBox.second};
        tempPos[1] = secondBox;
        tempPos[2] = {secondBox.first + 1, secondBox.second};
        tempPos[3] = {secondBox.first + 1, secondBox.second - 1};

        // top collision
        if (boxCollection[tempPos[0].first].find(tempPos[0].second) != boxCollection[tempPos[0].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].first++;
            }
            // final check
            if (boxCollection[tempPos[2].first].find(tempPos[2].second) != boxCollection[tempPos[2].first].end())
            {
                return;
            }
            if (boxCollection[tempPos[3].first].find(tempPos[3].second) != boxCollection[tempPos[3].first].end())
            {
                return;
            }
        }

        // bottom collision
        if (boxCollection[tempPos[2].first].find(tempPos[2].second) != boxCollection[tempPos[2].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].first--;
            }
            // final check
            if (boxCollection[tempPos[0].first].find(tempPos[0].second) != boxCollection[tempPos[0].first].end())
            {
                return;
            }
        }
        else if (boxCollection[tempPos[3].first].find(tempPos[3].second) != boxCollection[tempPos[3].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].second++;
            }
            // final check
            if (boxCollection[tempPos[0].first].find(tempPos[0].second) != boxCollection[tempPos[0].first].end())
            {
                return;
            }
        }

        // final check
    }
    else
    {
        tempPos[0] = {secondBox.first + 1, secondBox.second};
        tempPos[1] = secondBox;
        tempPos[2] = {secondBox.first - 1, secondBox.second};
        tempPos[3] = {secondBox.first - 1, secondBox.second + 1};

        // top collision
        if (boxCollection[tempPos[2].first].find(tempPos[2].second) != boxCollection[tempPos[2].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].first++;
            }
        }
        else if (boxCollection[tempPos[3].first].find(tempPos[3].second) != boxCollection[tempPos[3].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].second--;
            }
        }

        // bottom collision
        if (tempPos[0].first > height - 2 || boxCollection[tempPos[0].first].find(tempPos[0].second) != boxCollection[tempPos[0].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].first--;
            }
        }

        // final check

        if (boxCollection[tempPos[3].first].find(tempPos[3].second) != boxCollection[tempPos[3].first].end())
        {
            return;
        }
    }
    updateAfterRotaion(tempPos, colorValue);
}

// rotation of T shaped piece
void rotateT()
{
    pair<int, int> tempPos[4];

    auto secondBox = posBoxes[1];
    int colorValue = boxCollection[secondBox.first][secondBox.second];

    if (posBoxes[1].second < posBoxes[2].second)
    {
        tempPos[0] = {secondBox.first - 1, secondBox.second};
        tempPos[1] = secondBox;
        tempPos[2] = {secondBox.first + 1, secondBox.second};
        tempPos[3] = {secondBox.first, secondBox.second - 1};

        if (boxCollection[tempPos[0].first].find(tempPos[0].second) != boxCollection[tempPos[0].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].first++;
            }
            if (tempPos[2].first > height - 2 || boxCollection[tempPos[2].first].find(tempPos[2].second) != boxCollection[tempPos[2].first].end())
            {
                return;
            }
            if (boxCollection[tempPos[3].first].find(tempPos[3].second) != boxCollection[tempPos[3].first].end())
            {
                return;
            }
        }
    }
    else if (posBoxes[1].first < posBoxes[2].first)
    {
        tempPos[0] = {secondBox.first, secondBox.second + 1};
        tempPos[1] = secondBox;
        tempPos[2] = {secondBox.first, secondBox.second - 1};
        tempPos[3] = {secondBox.first - 1, secondBox.second};

        if (tempPos[0].second > width - 2 || boxCollection[tempPos[0].first].find(tempPos[0].second) != boxCollection[tempPos[0].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].second--;
            }
            if (tempPos[2].second < 1 || boxCollection[tempPos[2].first].find(tempPos[2].second) != boxCollection[tempPos[2].first].end())
            {
                return;
            }
            if (boxCollection[tempPos[3].first].find(tempPos[3].second) != boxCollection[tempPos[3].first].end())
            {
                return;
            }
        }
    }
    else if (posBoxes[1].second > posBoxes[2].second)
    {
        tempPos[0] = {secondBox.first + 1, secondBox.second};
        tempPos[1] = secondBox;
        tempPos[2] = {secondBox.first - 1, secondBox.second};
        tempPos[3] = {secondBox.first, secondBox.second + 1};

        if (tempPos[0].first > height - 2 || boxCollection[tempPos[0].first].find(tempPos[0].second) != boxCollection[tempPos[0].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].first--;
            }
            if (tempPos[2].first < 1 || boxCollection[tempPos[2].first].find(tempPos[2].second) != boxCollection[tempPos[2].first].end())
            {
                return;
            }
            if (boxCollection[tempPos[3].first].find(tempPos[3].second) != boxCollection[tempPos[3].first].end())
            {
                return;
            }
        }
    }
    else
    {
        tempPos[0] = {secondBox.first, secondBox.second - 1};
        tempPos[1] = secondBox;
        tempPos[2] = {secondBox.first, secondBox.second + 1};
        tempPos[3] = {secondBox.first + 1, secondBox.second};

        if (tempPos[0].second < 1 || boxCollection[tempPos[0].first].find(tempPos[0].second) != boxCollection[tempPos[0].first].end())
        {
            for (int i = 0; i < 4; i++)
            {
                tempPos[i].second++;
            }
            if (tempPos[2].second > width - 2 || boxCollection[tempPos[2].first].find(tempPos[2].second) != boxCollection[tempPos[2].first].end())
            {
                return;
            }
            if (boxCollection[tempPos[3].first].find(tempPos[3].second) != boxCollection[tempPos[3].first].end())
            {
                return;
            }
        }
    }
    updateAfterRotaion(tempPos, colorValue);
}

// function to determine where to move our piece.
void movement(char ch)
{
    switch (ch)
    {
    case 75:
        left_movement();
        return;

    case 77:
        right_movemenet();
        return;

    case 13:
        rotate();
        return;
    }
}

// It will determine the time interval between previous object  faling time and current time
bool delayFall()
{
    if (able)
        return able;
    while (true)
    {
        time_t currentTime = time(NULL);
        if (_kbhit())
        {
            char ch = _getch();
            if (ch == ' ')
            {
                able = true;
                return able;
            }
            else
            {
                movement(ch);
            }
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
            display(' ');
        }
        if (currentTime - pastUpdateTime >= 1)
        {
            pastUpdateTime = currentTime;
            return true;
        }
    }
    return true;
}

// It'll make fall the object as well as check for user input
bool fallLogic()
{

    bool result = delayFall();
    if (able || result)
    {
        // check if below elemnt is already prenset or not if present check if it contains another box or not
        for (int i = 0; i < 4; i++)
        {
            int X = posBoxes[i].first;
            int Y = posBoxes[i].second;
            if (X + 1 == height - 1)
            {
                return false;
            }
            if (boxCollection[X + 1].count(Y))
            {
                bool flag = true;
                for (int j = 0; j < 4; j++)
                {
                    if (posBoxes[j].second == Y && posBoxes[j].first == X + 1)
                    {
                        flag = false;
                        break;
                    }
                }
                if (flag)
                    return false;
            }
        }
        int colorValue = 0;
        // for eraseing previous valus
        for (int i = 0; i < 4; i++)
        {
            auto it = posBoxes[i];
            int X = it.first, Y = it.second;
            posBoxes[i].first++;
            colorValue = boxCollection[X][Y];
            boxCollection[X].erase(Y);
        }
        // for new values
        for (int i = 0; i < 4; i++)
        {
            auto it = posBoxes[i];
            int X = it.first, Y = it.second;
            boxCollection[X].insert({Y, colorValue});
        }
    }

    return true;
}

// function to destroy matched line and restock remaining pices
void destroyMatched()
{
    for (int i = height - 2; i > 3;)
    {
        if (boxCollection[i].size() == width - 2)
        {

            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
            display(char(176));
            Sleep(200);

            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
            display(char(254));
            Sleep(200);

            boxCollection[i].clear();
            score += width - 2;
            for (int column = 1; column < width - 1; column++)
            {
                int row1 = i;

                while (row1 < height - 1 && boxCollection[row1].find(column) == boxCollection[row1].end())
                {
                    row1++;
                }
                row1--;
                int row2 = i;
                while (row2 > 3 && row1 > 3)
                {
                    while (row2 > 3 && boxCollection[row2].find(column) == boxCollection[row2].end())
                        row2--;
                    if (row2 == 3)
                        break;
                    int colorValue = boxCollection[row2][column];
                    boxCollection[row2--].erase(column);
                    boxCollection[row1--].insert({column, colorValue});
                }
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
                display(' ');
                Sleep(100);
            }
            i = height - 2;
        }
        else
            i--;
    }
}
// Determine if the game is over or not
void isGameOver()
{
    for (int i = 0; i <= 3; i++)
    {
        if (boxCollection[i].size() != 0)
        {
            cout << "\n\t  Game Over !!\n"
                 << endl;
            getch();
            exit(0);
        }
    }
}

int main()
{
    system("cls");
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    pastUpdateTime = time(NULL);
    queue<int> q;
    genRandomShapes();
    Sleep(400);
    q.push(current_piece_index);
    while (true)
    {
        for (int i = 0; i < 4; i++)
        {
            posBoxes[i] = nextBoxes[i];
        }
        fixShapes();
        Sleep(400);
        genRandomShapes();
        q.push(current_piece_index);
        current_piece_index = q.front();
        q.pop();
        for (int i = 0; i < 4; i++)
        {
            s.insert({nextBoxes[i].first - 1, nextBoxes[i].second - 7});
        }
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
        display(' ');
        able = false;
        while (fallLogic())
        {
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
            display(' ');
        }
        destroyMatched();
        s.clear();
        isGameOver();
    }
    return 0;
}