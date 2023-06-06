///////////////////////////////////////////////
//
// Лабораторная работа № 1 по дисциплине "Логические основы интеллектуальных систем"
// Выполнена студентами группы 121701 БГУИР Галуга М.В. и Кулешевский Г.Д.
// Файл содержит набор функций, определяющий количество различных подформул формулы сокращенного языка логики высказываний
// Дата: 16.05.2023
// 
///////////////////////////////////////////////



#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <set>

using namespace std;

struct LogicalElement //структура для парсинга строки на логические элементы 
{
    bool isVariable = 0;
    bool isUnOperator = 0;
    bool isBinOperator = 0;
    bool isInBracket = 0;
    bool isOutBracket = 0;
    LogicalElement()
    {
        this->isVariable = 0;
        this->isUnOperator = 0;
        this->isBinOperator = 0;
        this->isInBracket = 0;
        this->isOutBracket = 0;
    }
};
bool validateBrackets(string formula) // проверка формулы на соответствие количества открытых и закрытых скобок
{
    if (formula[0] != '(' || formula[formula.size() - 1] != ')')
        return 0;
    int capOfBrakets = 0;
    for (int i = 0; i < formula.size(); i++)
    {
        if (formula[i] == '(')
            capOfBrakets++;
        if (formula[i] == ')')
            capOfBrakets--;
    }
    if (capOfBrakets == 0)
        return 1;
    else
        return 0;
} 
bool isAlphabetCharacter(string formula) //проверка формулы на символы принадлежащие алфавиту
{
    vector <char> symbols = { '!', '(', ')', '~', '/', '\\', '.', '-', '>' };
    for (int i = 0; i < formula.size(); i++)
    {
        bool out = 0;
        if (formula[i] >= 65 && formula[i] <= 90) //letters check
            out = 1;
        else if (formula[i] == '0' || formula[i] == '1')    //else if (formula[i] >= 48 && formula[i] <= 57)
            out = 1;
        else
        {
            for (int j = 0; j < symbols.size(); j++)
                if (formula[i] == symbols[j])
                    out = 1;
        }
        if (out == 0)
            return 0;
    }
    return 1;
} 
bool splitFormulaIntoComponents(string formula, vector <LogicalElement>& ParsedVector) // парсинг формулы на логические элементы
{
    for (int i = 0; i < formula.size(); i++)
    {
        LogicalElement newThing;
        if (formula[i] == '(') //открывающая скобка
        {
            newThing.isInBracket = 1;
            ParsedVector.push_back(newThing);
        }
        else if (formula[i] == ')') //закрывающая скобка
        {
            newThing.isOutBracket = 1;
            ParsedVector.push_back(newThing);
        }
        else if (formula[i] >= 65 && formula[i] <= 90) //переменные
        {
            newThing.isVariable = 1;
            ParsedVector.push_back(newThing);
        }
        else if (formula[i] == '0' || formula[i] == '1') //константы
        {
            newThing.isVariable = 1;
            ParsedVector.push_back(newThing);
        }
        else if ((formula[i] == '/' && formula[i + 1] == '\\') || (formula[i] == '\\' && formula[i + 1] == '/') || (formula[i] == '-' && formula[i + 1] == '>')) //бинарные операторы из двух символов
        {
            newThing.isBinOperator = 1;
            ParsedVector.push_back(newThing);
            i += 1;
            continue;
        }
        else if (formula[i] == '~') //бинарные операторы из одного символа
        {
            newThing.isBinOperator = 1;
            ParsedVector.push_back(newThing);
        }
        else if (formula[i] == '!') //унарные операторы из одного символа
        {
            newThing.isUnOperator = 1;
            ParsedVector.push_back(newThing);
        }
        else
            return 0;
    }
    return 1;
}
bool checkBracketBalance(vector <LogicalElement> ParsedVector, int& index) // проверка на наличие хотябы одного оператора внутри одной пары скобок
{
    int capacityOfOperators = 0;
    bool isFuncTrue = 1;
    for (index = index; index < ParsedVector.size() - 1; index++)
    {
        LogicalElement thing = ParsedVector[index];
        if (thing.isBinOperator == 1 || thing.isUnOperator == 1)
            capacityOfOperators++;
        if (thing.isInBracket == 1)
        {
            index++;
            isFuncTrue = checkBracketBalance(ParsedVector, index);
        }
        else if (thing.isOutBracket == 1)
            break;
    }
    if (isFuncTrue == 1) {
        if (capacityOfOperators > 1)
            return 0;
        else
            return 1;
    }
    else
        return 0;
}
bool validateDoubleBrackets(vector <LogicalElement> ParsedVector) // проверка на наличие только одной пары скобок вокруг одного оператора
{
    int capOfOperators = 0;
    int capOfBrackets = 0;
    for (int i = 0; i < ParsedVector.size(); i++)
    {
        LogicalElement thing = ParsedVector[i];
        if (thing.isBinOperator == 1 || thing.isUnOperator == 1)
        {
            capOfOperators++;
            continue;
        }
        if (thing.isInBracket == 1)
        {
            capOfBrackets++;
            continue;
        }
    }
    if (capOfBrackets != capOfOperators)
        return 0;
    return 1;
}
bool checkLogicalFormula(string formula) // проверка на расстановку элементов в формуле
{
    vector <LogicalElement> parsedFormula;
    bool parseErr = 0;
    parseErr = !splitFormulaIntoComponents(formula, parsedFormula);
    if (parseErr == 1)
        return 0;
    if (parsedFormula.size() < 3)
        return 0;
    int zero = 0;
    if (!checkBracketBalance(parsedFormula, zero)) // прверка на один оператор на пару скобок
        return 0;
    if (!validateDoubleBrackets(parsedFormula)) // проверка на дублирующиеся скобки
        return 0;
    for (int i = 0; i < parsedFormula.size(); i++)
    {
        if (i == parsedFormula.size() - 1 && parsedFormula[i].isOutBracket == 1) // проверка на скобку в начеле и конце
            return 1;
        if (parsedFormula[i].isInBracket == 1) //проверка последующих логических элементов после открывающей скобки
        {
            if (!((parsedFormula[i + 1].isInBracket == 1) || (parsedFormula[i + 1].isUnOperator == 1) || (parsedFormula[i + 1].isVariable == 1)))
                return 0;
        }
        else if (parsedFormula[i].isVariable == 1)//проверка последующих логических элементов после переменной
        {
            if (!((parsedFormula[i + 1].isOutBracket == 1) || (parsedFormula[i + 1].isBinOperator == 1)))
                return 0;
        }
        else if (parsedFormula[i].isBinOperator == 1)//проверка последующих логических элементов после бинарного оператора
        {
            if (!((parsedFormula[i + 1].isInBracket == 1) || (parsedFormula[i + 1].isVariable == 1)))
                return 0;
        }
        else if (parsedFormula[i].isUnOperator == 1)//проверка последующих логических элементов после унарного оператора
        {
            if (!((parsedFormula[i + 1].isInBracket == 1) || (parsedFormula[i + 1].isVariable == 1 && parsedFormula[i + 2].isOutBracket == 1)))
                return 0;
        }
        else if (parsedFormula[i].isOutBracket == 1)//проверка последующих логических элементов после закрывающей скобки
        {
            if (!((parsedFormula[i + 1].isOutBracket == 1) || (parsedFormula[i + 1].isBinOperator == 1)))
                return 0;
        }
    }
    return 1;
}
bool validateVariables(string formula, set <string>& variables) //проверка переменных на синтаксис
{
    for (int i = 0; i < formula.size(); i++) //перебирая строку находим букву
    { // заменяем букву и все дальнейшие символы соответствующие синтаксису на "_"
        string buffer = "";
        if ((formula[i] == '0' || formula[i] == '1') && i <= formula.size() - 1)
        {
            if (!((formula[i + 1] >= 65 && formula[i + 1] <= 90) || (formula[i + 1] == '0' || formula[i + 1] == '1')))
                formula[i] = '_';
            else
                continue;
        }
        if ((formula[i] >= 65 && formula[i] <= 90) && i <= formula.size() - 1)
        {
            if (!((formula[i + 1] >= 65 && formula[i + 1] <= 90) || (formula[i + 1] == '0' || formula[i + 1] == '1')))
            {
                buffer.push_back(formula[i]);
                variables.insert(buffer);
                formula[i] = '_';
            }
            else
                continue;
        }
    }
    bool isThereNoWrongSymbols = 1; //проверяем строку на наличие незамененных символов
    for (int i = 0; i < formula.size(); i++)
        if ((formula[i] >= 65 && formula[i] <= 90) || (formula[i] == '0' || formula[i] == '1'))
            isThereNoWrongSymbols = 0;
    return isThereNoWrongSymbols;
}

bool CheckSymbolsAroundOperator(string formula, int i)
{
    char previousSymbol = formula[i - 1];
    if (((previousSymbol >= 65 && previousSymbol <= 90) || (previousSymbol == '0' || previousSymbol == '1') || (previousSymbol == ')') || (previousSymbol == '('))) {
        char postNextSymbol = formula[i + 2];
        if (((postNextSymbol >= 65 && postNextSymbol <= 90) || (postNextSymbol == '0' || postNextSymbol == '1') || (postNextSymbol == ')') || (postNextSymbol == '(')))
            return 1;
    }
    return 0;
}

bool validateOperators(string formula)
{
    for (int i = 0; i < formula.size(); i++)
    {
        char currentChar = formula[i];
        char nextChar = formula[i + 1];
        if (currentChar == '/' && nextChar == '\\')
            if (!CheckSymbolsAroundOperator(formula, i))
            {
                return 0;
            }
            else
                continue;
        if ((currentChar == '\\' && nextChar == '/'))
            if (!CheckSymbolsAroundOperator(formula, i))
            {
                return 0;
            }
            else
                continue;
        if ((currentChar == '-' && nextChar == '>'))
            if (!CheckSymbolsAroundOperator(formula, i))
            {
                return 0;
            }
            else
                continue;
    }
    return 1;
}

// Функция проверки принадлежности языку
bool isValidFormula(string formula)
{
    if ((formula[0] == '1' || formula[0] == '0') && formula.size() == 1) //проверка формулы состоящей из одной константы
        return 1;
    if ((formula[0] >= 65 && formula[0] <= 90) && formula.size() == 1) //проверка формулы состоящей из одной константы
        return 1;
    if (!validateBrackets(formula))//проверка скобок
        return 0;
    if (!isAlphabetCharacter(formula))//проверка алфавита
        return 0;
    set <string> unicAtomicFormula;
    if (!validateVariables(formula, unicAtomicFormula))//проверка перпеменных на синтаксис
        return 0;
    if (!validateOperators(formula))
        return 0;
    if (!checkLogicalFormula(formula)) // проверка на расстановку элементов в формуле
        return 0;
    return 1;
}

void findClosingBracket(string formula, int firstBracket, int& output) //поиск индекса закрывающей скобки
{
    int nesting = 1;
    for (int i = firstBracket + 1; i < formula.size(); i++)
    {
        if (formula[i] == '(')
            nesting++;
        else if ((formula[i] == ')'))
            nesting--;
        if (nesting <= 0)
        {
            output = i;
            break;
        }
    }
} 
void addSubformula(string formula, int firstBracket, int secondBracket, set <string>& subFormuls) // добавить подформулу в множество подформул
{
    string substring = "";
    for (int i = firstBracket; i <= secondBracket; i++)
        substring.push_back(formula[i]);
    subFormuls.insert(substring);
} 

set <string> findSubformulas(string formula) // поиск подформул
{
    set <string> unicFormula;

    if ((formula[0] == '1' || formula[0] == '0') && formula.size() == 1) //проверка формулы состоящей из одной константы
    {
        unicFormula.insert(formula);
        return unicFormula;
    }

    if (formula[0] >= 65 && formula[0] <= 90)     //если формула начинается с буква
    {
        unicFormula.insert(formula);
        return unicFormula;
    }

    validateVariables(formula, unicFormula);// поиск атомарных формул


    int nextBracket = 0;
    for (int i = 0; i < formula.size(); i++) //поиск подформул в строке
    {
        if (formula[i] == '(')
        {
            findClosingBracket(formula, i, nextBracket);
            addSubformula(formula, i, nextBracket, unicFormula);
        }
    }

    return unicFormula;
}



int main() {
    while (1)
    {
        setlocale(LC_ALL, "ru");
        string formula;
        cout << "Введите формулу логики высказываний: ";
        getline(cin, formula);
        if (isValidFormula(formula)) {
            cout << "Формула принадлежит языку" << endl;
            set <string> Subformuls;
            Subformuls = findSubformulas(formula);
            cout << "Количество уникальных подформул в заданной формуле: ";
            cout << Subformuls.size() << endl;
            cout << "Уникальные подформулы заданной формулы:" << endl;
            for (std::set<string>::iterator i = Subformuls.begin(); i != Subformuls.end(); ++i)
            {
                cout << *i << endl;
            }
        }
        else {
            cout << "Формула не принадлежит языку" << endl;
        }
    }
    return 0;
}


