#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

using std::cout;
using std::cin;
using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;

class FiniteAutomaton
{
public:
    FiniteAutomaton();
    ~FiniteAutomaton();

    bool readInputFile(string path);
    void writeOutputFile(string path);
    void calulateAlphabet();
    void calculateSegments();
    void buildTable();
    void calculate(string state);

private:
    // регулярное выражение
    string expression;
    // алфавит регулярного выражения
    string alphabet;
    // сегменты, на которые разбили регулярное выражение
    vector<string> segments;
    /* хранит предыдущие состояния, в которые
     * пришли при работе с предыдущим сегментом */
    vector<int> recentStates;
    // текущая строка
    int indexRow;
    // текущая колонка
    int indexColumn;
    // количество состояний
    unsigned long long numberOfStates;
    // автоматная таблица
    vector<vector<string>> table;
};
