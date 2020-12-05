#include "finiteautomaton.h"

FiniteAutomaton::FiniteAutomaton()
{
    numberOfStates = 1;
}

FiniteAutomaton::~FiniteAutomaton() {

}
/* Считаем входной файл. Если файл не удалось найти,
 * то говорим об ошибке. */
bool FiniteAutomaton::readInputFile(string path)
{
    ifstream file(path);
    if (!file)
    {
        cout << "Error. File 'input' not found\n";
        return false;
    }
    while (!file.eof())
    {
        string str;
        getline(file, str);

        expression = str;
    }
    file.close();

    //cout << expression << "\n";
    return true;
}
/*Вычисляем алфавит и считаем количество состояний,
 * которое необходимо для построения таблицы. */
void FiniteAutomaton::calulateAlphabet() {
    for(unsigned long long i = 0; i < expression.length(); i++)
    {
        if(expression[i] != '<'  && expression[i] != '>' && expression[i] != '(')
            if(expression[i] != ')' && expression[i] != '|' && alphabet.find(expression[i]) == string::npos)
            {
                alphabet = alphabet + expression[i];
                numberOfStates++;
            }

    }
    if(!alphabet.empty()) std::sort(alphabet.begin(), alphabet.end());
}
/* Разбиваем на сегменты регулярное выражение. */
void FiniteAutomaton::calculateSegments() {
    unsigned long long i = 0;

    while(i < expression.length())
    {
        if(expression[i] == '(')
        {
            unsigned long long left = expression.find('(', i + 1), right = expression.find(')', i);

            while(left != string::npos && left < right)
            {
                left = expression.find('(', left + 1);
                right = expression.find(')', right + 1);
            }

            segments.push_back(expression.substr(i, right - i + 1));
            i = right;
        }
        else if(expression[i] == '<')
        {
            unsigned long long left = expression.find('<', i + 1), right = expression.find('>', i);

            while(left != string::npos && left < right)
            {
                left = expression.find('<', left + 1);
                right = expression.find('>', right + 1);
            }

            if(right + 1 < expression.length())
                if(expression[right + 1] != '(' && expression[right + 1] != '<')
                    right = right + 1;

            segments.push_back(expression.substr(i, right - i + 1));
            i = right;
        }
        else
        {
            segments.push_back(expression.substr(i, 1));
        }

        i++;
    }

    //for(unsigned long long i = 0; i < segments.size(); i++)
    //    cout << " " << segments[i] << " ";
}
/*Построение автоматной таблицы. */
void FiniteAutomaton::buildTable() {
    vector<string> row = {" "};
    indexRow = 1;
    indexColumn = 1;
    for(unsigned long long i = 0; i < numberOfStates; i++)
        row.push_back("q" + std::to_string(i + 1));
    table.push_back(row);

    for(unsigned long long i = 1; i <= numberOfStates; i++)
    {
        row = {"q" + std::to_string(i)};
        for(unsigned long long j = 1; j <= numberOfStates; j++)
            row.push_back("0");
        table.push_back(row);
    }

    for(unsigned long long i = 0; i < segments.size(); i++)
        calculate(segments[i]);

}
/* Вычисления для каждого сегмента. */
void FiniteAutomaton::calculate(string state) {
    vector<int> currentStates;
    if(state[0] == '(')
    {
        for(unsigned long long i = 0; i < state.find(')', state.size() - 2); i++)
        {
            if(state[i] != '<')
            {
                for(unsigned long long j = 0; j < alphabet.length(); j++)
                    if(alphabet[j] == state[i])
                    {
                        indexColumn++;
                        if(recentStates.empty())
                        {
                            table[indexRow][indexColumn] = state[i];
                            currentStates.push_back(indexColumn);
                        }
                        else
                        {
                            for(unsigned long long j_2 = 0; j_2 < recentStates.size(); j_2++)
                            {
                                table[recentStates[j_2]][indexColumn] = state[i];
                                currentStates.push_back(indexColumn);
                            }
                        }
                    }
            }
            else
            {
                for(unsigned long long i_2 = i; i_2 < state.find('>', i); i_2++)
                {
                    for(unsigned long long j = 0; j < alphabet.length(); j++)
                        if(alphabet[j] == state[i_2])
                        {
                            indexColumn++;
                            if(recentStates.empty())
                            {
                                table[indexRow][indexColumn] = state[i_2];
                                table[indexColumn][indexColumn] = state[i_2];
                                currentStates.push_back(indexColumn);
                            }
                            else
                            {
                                for(unsigned long long j_2 = 0; j_2 < recentStates.size(); j_2++)
                                {
                                    table[recentStates[j_2]][indexColumn] = state[i_2];
                                    table[indexColumn][indexColumn] = state[i_2];
                                    currentStates.push_back(indexColumn);
                                }
                            }
                        }
                    i = i_2;
                }
            }
        }
        // Для последнего элемента. Вдруг он входит в алфавит
        for(unsigned long long i = 0; i < alphabet.length(); i++)
            if(alphabet[i] == state[state.size()-1])
            {
                indexColumn++;
                if(recentStates.empty())
                {
                    table[indexRow][indexColumn] = state[state.size()-1];
                    currentStates.push_back(indexColumn);
                }
                else
                {
                    vector<int> tempStates;
                    for(unsigned long long j_2 = 0; j_2 < currentStates.size(); j_2++)
                    {
                        table[currentStates[j_2]][indexColumn] = state[state.size()-1];
                        tempStates.push_back(indexColumn);
                    }
                    currentStates = tempStates;
                }
            }
    }
    else if(state[0] == '<')
    {
        for(unsigned long long i = 0; i < state.find('>', state.size() - 2); i++)
        {

            for(unsigned long long j = 0; j < alphabet.length(); j++)
                if(alphabet[j] == state[i])
                {
                    indexColumn++;
                    if(recentStates.empty())
                    {
                        table[indexRow][indexColumn] = state[i];
                        table[indexColumn][indexColumn] = state[i];
                        currentStates.push_back(indexColumn);
                    }
                    else
                    {
                        for(unsigned long long j_2 = 0; j_2 < recentStates.size(); j_2++)
                        {
                            table[recentStates[j_2]][indexColumn] = state[i];
                            table[indexColumn][indexColumn] = state[i];
                            currentStates.push_back(indexColumn);
                        }
                    }
                }

        }
        // Для последнего элемента. Вдруг он входит в алфавит
        for(unsigned long long i = 0; i < alphabet.length(); i++)
            if(alphabet[i] == state[state.size()-1])
            {
                indexColumn++;
                if(currentStates.empty())
                {
                    table[indexRow][indexColumn] = state[state.size()-1];
                    currentStates.push_back(indexColumn);
                }
                else
                {
                    vector<int> tempStates;
                    for(unsigned long long j_2 = 0; j_2 < currentStates.size(); j_2++)
                    {
                        table[currentStates[j_2]][indexColumn] = state[state.size()-1];
                        tempStates.push_back(indexColumn);
                    }
                    currentStates = tempStates;
                }
            }
    }
    else
    {

        for(unsigned long long i = 0; i < alphabet.length(); i++)
            if(alphabet[i] == state[0])
            {
                indexColumn++;
                if(recentStates.empty())
                {
                    table[indexRow][indexColumn] = state[0];
                    currentStates.push_back(indexColumn);
                }
                else
                {
                    for(unsigned long long j_2 = 0; j_2 < recentStates.size(); j_2++)
                    {
                        table[recentStates[j_2]][indexColumn] = state[0];
                        currentStates.push_back(indexColumn);
                    }
                }
            }
    }
    recentStates = currentStates;
    indexRow = indexColumn;
}
/*Запись результата в файл. */
void FiniteAutomaton::writeOutputFile(string path) {
    ofstream file_out;
    file_out.open(path, std::ios::app);

    for(unsigned long long i = 0; i < table.size(); i++)
    {
        for(unsigned long long j = 0; j < table[i].size(); j++)
            if(i != 0 && j != 0) file_out << table[i][j] << "  ";
            else file_out << table[i][j] << " ";
        file_out << "\n";
    }
    file_out.close();
}







