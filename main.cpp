#include "finiteautomaton.h"

int main()
{
    FiniteAutomaton finiteAutomaton;
    // Читаем входной файл
    if(finiteAutomaton.readInputFile("../SynthesisOfFiniteAutomata/regular_expression.txt"))
    {
        // Вычисляем алфавит для регулярного выражения
        finiteAutomaton.calulateAlphabet();
        // Вычисляем состояния, то есть разбиваем регулярное выражение
        finiteAutomaton.calculateSegments();
        // Строим автоматную таблицу по этим состояниям
        finiteAutomaton.buildTable();
        // Записываем автоматную таблицу в выходной файл
        finiteAutomaton.writeOutputFile("../SynthesisOfFiniteAutomata/result.txt");
    }
}
