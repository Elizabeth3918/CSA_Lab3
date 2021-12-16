#include <iostream>
#include <stack>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

typedef union
{
    float f;
    struct
    {
        int mantissa : 7;
        int exponent : 12;
        int sign : 1;
    } raw;
} binary_float;

stack<binary_float> operations;
vector<binary_float> R;
string IR;
int TC = 0, PC = 0, counter = 0;
char PS = '+';
float x = 1.6, y = 1.9;
binary_float nullValue;

void printBinary(int n, int i)
{
    int k;
    for(k = i - 1; k >= 0; k--)
    {
        if((n >> k) & 1)
            cout << "1";
        else
            cout << "0";
    }
}

void printIEEE754(binary_float value)
{
    cout << ' ' << fabs(value.raw.sign) << " | ";
    printBinary(value.raw.exponent, 12);
    cout << " | ";
    printBinary(value.raw.mantissa, 7);
    cout << endl;
}

void DisplayInfo()
{
    TC = TC % 2 + 1;
    PC += TC % 2;
    cout << "\nIR: " << IR << endl;
    for(int i = 0; i < 4; ++i)
    {
        cout << "R" << i + 1 << ':';
        printIEEE754(R[i]);
    }
    cout << "PS: " << PS << "\n";
    cout << "PC: " << PC << "\n";
    cout << "TC: " << TC << "\n";
}

void Move(const float& value)
{
    binary_float temp;
    temp.f = value;
    R[counter] = temp;
    operations.push(temp);
    ++counter;
}

void Subtract()
{
    binary_float first = operations.top();
    operations.pop();
    binary_float second = operations.top();
    operations.pop();
    binary_float newValue;
    newValue.f = second.f - first.f;
    operations.push(newValue);
    --counter;
    R[counter - 1] = newValue;
    R[counter] = nullValue;
}

void Add()
{
    binary_float first = operations.top();
    operations.pop();
    binary_float second = operations.top();
    operations.pop();
    binary_float newValue;
    newValue.f = first.f + second.f;
    operations.push(newValue);
    --counter;
    R[counter - 1] = newValue;
    R[counter] = nullValue;
}

void Divide()
{
    binary_float first = operations.top();
    operations.pop();
    binary_float second = operations.top();
    operations.pop();
    binary_float newValue;
    newValue.f =  second.f / first.f;
    operations.push(newValue);
    --counter;
    R[counter - 1] = newValue;
    R[counter] = nullValue;
}

void Mult()
{
    binary_float first = operations.top();
    operations.pop();
    binary_float second = operations.top();
    operations.pop();
    binary_float newValue;
    newValue.f =  second.f * first.f;
    operations.push(newValue);
    --counter;
    R[counter - 1] = newValue;
    R[counter] = nullValue;
}

void ProcessCommand(const string& command)
{
    if(command[0] == 'm')
    {
        if(command[5] == 'x') Move(x);
        else if(command[5] == 'y') Move(y);
        else
        {

            string decimalValue = command.substr(5, command.size() - 1);
            float value = stof(decimalValue);
            Move(value);
        }
    }
    else if(command[0] == 'a')
    {
        Add();
    }
    else if(command[0] == 'd')
    {
        Divide();
    }
    else if (command[0] == 'M')
    {
        Mult();
    }
    else
    {
        Subtract();
    }
    if(operations.top().raw.sign != 0) PS = '-';
    else PS = '+';
    DisplayInfo();
}

void ReadDoc()
{
    fstream stream;
    stream.open("execute.txt", ios::in);
    if(stream.is_open())
    {
        string command;
        while(getline(stream, command))
        {
            IR = command;
            DisplayInfo();
            ProcessCommand(command);
        }
        stream.close();
    }
}

void PrintHead()
{
    cout << " s | hhhhhhhhhhhh | mmmmmmm - number in IEEE 754 Format\n";
    cout << " 0 | 111111111110 | 1111111  - maximum positive value\n";
    cout << " 1 | 111111111110 | 0000000  - minimum negative value\n";
    cout << " 0 | 111111111111 | 0000000 - positive infinity value\n";
    cout << " 1 | 111111111111 | 0000000 - negative infinity value\n";
    cout << " 1 | 111111111111 | 0000110 - NaN value\n";

    cout << "\nThe program is coded to calculate the given formula\n"
            "(2*x - y + 11)/(x + 3*y) at the user-specified values x and y.\n"
            "It will be executed one step at a tact and will show the user\n"
            "in-between calculations\n";
}

void FillR()
{
    nullValue.f = 0;
    for(int i = 0; i < 3; ++i)
    {
        R.push_back(nullValue);
    }
}

int main()
{
    PrintHead();
    FillR();
    ReadDoc();

    cout << "\nRESULT: " << operations.top().f << "\n";
    return 0;
}