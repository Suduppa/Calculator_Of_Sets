#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <stack>
#include <sstream>

using namespace std;

class MySet {
private:
    int _size;
    string _setName;
    int _capacity;
    int* _elements;
    int _minUniversal;
    int _maxUniversal;

public:
    MySet() : _elements(nullptr), _size(0), _capacity(0), _setName(""), _minUniversal(0), _maxUniversal(0) {}

    ~MySet() {
        delete[] _elements;
        _elements = nullptr;
    }

    void Insert(const int& element) {
        if (!Contains(element)) {
            if (_size >= _capacity) {
                _capacity = (_capacity == 0) ? 1 : _capacity * 2;
                int* newElements = new int[_capacity];
                for (int i = 0; i < _size; i++) {
                    newElements[i] = _elements[i];
                }
                delete[] _elements;
                _elements = newElements;
            }
            _elements[_size++] = element;
        }
    }

    bool Contains(const int& element) const {
        for (int i = 0; i < _size; ++i) {
            if (_elements[i] == element) {
                return true;
            }
        }
        return false;
    }

    void Remove(const int& element) {
        int index = -1;
        for (int i = 0; i < _size; ++i) {
            if (_elements[i] == element) {
                index = i;
                break;
            }
        }
        if (index != -1) {
            for (int i = index; i < _size - 1; ++i) {
                _elements[i] = _elements[i + 1];
            }
            --_size;
        }
    }

    void SetName(string name) {
        _setName = name;
    }

    string GetName() {
        return _setName;
    }

    int GetSize() const {
        return _size;
    }

    void Clear() {
        _size = 0;
        _capacity = 0;
        delete[] _elements;
        _elements = nullptr;
    }

    void Display() const {
        for (int i = 0; i < _size; i++) {
            cout << _elements[i] << " ";
        }
    }

    MySet operator*(const MySet& other) const {
        MySet intersection;
        for (int i = 0; i < _size; i++) {
            if (other.Contains(_elements[i])) {
                intersection.Insert(_elements[i]);
            }
        }
        return intersection;
    }

    MySet operator+(const MySet& other) const {
        MySet unionSet;
        for (int i = 0; i < _size; i++) {
            unionSet.Insert(_elements[i]);
        }
        for (int i = 0; i < other._size; i++) {
            unionSet.Insert(other._elements[i]);
        }
        return unionSet;
    }

    MySet operator-(const MySet& other) const {
        MySet difference;
        for (int i = 0; i < _size; i++) {
            if (!other.Contains(_elements[i])) {
                difference.Insert(_elements[i]);
            }
        }
        return difference;
    }

    MySet operator!() const {
        MySet complement;
        for (int i = _minUniversal; i <= _maxUniversal; i++) {
            if (!Contains(i)) {
                complement.Insert(i);
            }
        }
        return complement;
    }

    MySet& operator=(const MySet& other) {
        if (this == &other) {
            return *this; // Проверка на самоприсваивание
        }

        // Освобождаем текущие ресурсы
        Clear();

        // Копируем данные из другого объекта
        _size = other._size;
        _setName = other._setName;
        _capacity = other._capacity;
        _minUniversal = other._minUniversal;
        _maxUniversal = other._maxUniversal;

        // Выделяем память и копируем элементы
        if (_size > 0) {
            _elements = new int[_capacity];
            for (int i = 0; i < _size; i++) {
                _elements[i] = other._elements[i];
            }
        }
        else {
            _elements = nullptr;
        }

        return *this;
    }

    friend ostream& operator<<(ostream& os, const MySet& set) {
        for (int i = 0; i < set._size; i++) {
            os << set._elements[i] << " ";
        }
        return os;
    }

    void SetMinUniversal(int minVal) {
        _minUniversal = minVal;
    }

    void SetMaxUniversal(int maxVal) {
        _maxUniversal = maxVal;
    }
};


void ClearAll(vector<MySet*>& v) {
    for (int i = 0; i < v.size(); i++) {
        delete v[i];
    }
    v.clear();
}

bool IsNumber(const string& str) {
    for (char c : str) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

void ShowAllSets(const vector<MySet*>& v) {
    for (int i = 0; i < v.size(); i++) {
        cout << i << ". " << (*v[i]).GetName() << " = {" << (*v[i]) << "}\n";
    }
}

template<typename T>
void Remove(vector<T>& v, size_t index) {
    v.erase(v.begin() + index);
}

bool Calculate(vector<MySet*>& sets, string expression) {
    auto IsOperator = [](const string token) {
        return token == "+" || token == "*" || token == "-" || token == "!";
        };

    auto IsOperand = [&IsOperator](const string token) {
        return !IsOperator(token);
        };

    auto Priority = [](const string op) {
        if (op == "+" || op == "-") {
            return 1;
        }
        if (op == "*") {
            return 2;
        }
        if (op == "!") {
            return 3;
        }
        return 0;
        };

    istringstream iss(expression);
    string token;
    stack<string> operatorStack;
    stack<MySet*> operandStack;
    stack<string> parenthesesStack;

    while (iss >> token) {
        if (token == "(") {
            operatorStack.push(token);
            parenthesesStack.push(token);
        }
        else if (token == ")") {
            while (!operatorStack.empty() && operatorStack.top() != "(") {
                string topOperator = operatorStack.top();
                operatorStack.pop();

                MySet* secondOperand = operandStack.top();
                operandStack.pop();
                MySet* firstOperand = operandStack.top();
                operandStack.pop();

                if (topOperator == "+") {
                    operandStack.push(new MySet(*firstOperand + *secondOperand));
                }
                else if (topOperator == "*") {
                    operandStack.push(new MySet(*firstOperand * *secondOperand));
                }
                else if (topOperator == "-") {
                    operandStack.push(new MySet(*firstOperand - *secondOperand));
                }
                else if (topOperator == "!") {
                    operandStack.push(new MySet(!*firstOperand));
                }
            }
            if (!operatorStack.empty() && operatorStack.top() == "(") {
                operatorStack.pop();
                parenthesesStack.pop();
            }
            else {
                cout << "Ошибка: несогласованные скобки." << endl;
                return false;
            }
        }
        else if (IsOperator(token)) {
            while (!operatorStack.empty() && operatorStack.top() != "(" && Priority(token) <= Priority(operatorStack.top())) {
                string topOperator = operatorStack.top();
                operatorStack.pop();

                if (topOperator == "!") {
                    if (operandStack.empty()) {
                        cout << "Ошибка: отсутствует операнд для оператора '!" << endl;
                        return false;
                    }
                    MySet* firstOperand = operandStack.top();
                    operandStack.pop();
                    operandStack.push(new MySet(!*firstOperand));
                }
                else {
                    if (operandStack.size() < 2) {
                        cout << "Ошибка: недостаточно операндов для оператора '" << topOperator << "'" << endl;
                        return false;
                    }
                    MySet* secondOperand = operandStack.top();
                    operandStack.pop();
                    MySet* firstOperand = operandStack.top();
                    operandStack.pop();

                    if (topOperator == "+") {
                        operandStack.push(new MySet(*firstOperand + *secondOperand));
                    }
                    else if (topOperator == "*") {
                        operandStack.push(new MySet(*firstOperand * *secondOperand));
                    }
                    else if (topOperator == "-") {
                        operandStack.push(new MySet(*firstOperand - *secondOperand));
                    }
                }
            }
            operatorStack.push(token);
        }
        else if (IsOperand(token)) {
            bool found = false;
            for (MySet* set : sets) {
                if (set->GetName() == token) {
                    operandStack.push(set);
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << "Ошибка: множество '" << token << "' не найдено." << endl;
                return false;
            }
        }
    }

    while (!operatorStack.empty()) {
        string topOperator = operatorStack.top();
        operatorStack.pop();

        if (topOperator == "!") {
            MySet* operand = operandStack.top();
            operandStack.pop();
            operandStack.push(new MySet(!*operand));
        }
        else {
            MySet* secondOperand = operandStack.top();
            operandStack.pop();
            MySet* firstOperand = operandStack.top();
            operandStack.pop();

            if (topOperator == "+") {
                operandStack.push(new MySet(*firstOperand + *secondOperand));
            }
            else if (topOperator == "*") {
                operandStack.push(new MySet(*firstOperand * *secondOperand));
            }
            else if (topOperator == "-") {
                operandStack.push(new MySet(*firstOperand - *secondOperand));
            }
        }
    }

    if (!parenthesesStack.empty()) {
        cout << "Ошибка: несогласованные скобки." << endl;
        return false;
    }

    if (operandStack.size() != 1) {
        cout << "Ошибка: неверное количество операндов и операторов." << endl;
        return false;
    }

    cout << "Результат: " << *operandStack.top() << endl;

    while (!operandStack.empty()) {
        if (find(sets.begin(), sets.end(), operandStack.top()) == sets.end()) {
            delete operandStack.top();
        }
        operandStack.pop();
    }

    return true;
}


int main() {
    setlocale(0, "");
    srand(time(NULL));

    vector<MySet*> setVector; // Вектор множеств
    vector<MySet*> tempSetVector;
    bool programIsRunning = true;
    string setName;
    int minSetRange, maxSetRange;
    cout << "Ввод границ универсума\n";
    cout << "Введите минимальное значение универсума: ";
    cin >> minSetRange;
    cout << "Введите максимальное значение универсума: ";
    cin >> maxSetRange;

    while (programIsRunning) {
        system("cls");
        cout << "===========МЕНЮ:================\n";
        cout << "[1] Новое множество\n";
        cout << "[2] Удалить множество\n";
        cout << "[3] Изменить элементы множества\n";
        cout << "[4] Вычислить выражение\n";
        cout << "[5] Очистить все множества\n";
        cout << "[6] Выйти из программы\n\n";
        cout << "================================";
        ShowAllSets(setVector);
        bool flag = true;
        int n;
        char menuChoice;
        cin >> menuChoice;
        switch (menuChoice) {
        case '1': // Создать множество
        {
            system("cls");
            cout << "Каким создать множество?\n";
            cout << "1 - Пустое.\n";
            cout << "2 - С параметрами.\n";
            int indexOfName = 0;
            cin >> menuChoice;
            system("cls");
            switch (menuChoice) {
            case '1':

                cout << "Введите имя для нового множества: ";
                cin >> setName;

                while (indexOfName < setName.length())
                    if (!(setName[indexOfName] >= '0' and setName[indexOfName] <= '9') and
                        !(setName[indexOfName] >= 'A' and setName[indexOfName] <= 'Z') and
                        !(setName[indexOfName] >= 'a' and setName[indexOfName] <= 'z')) {
                        cout << "Введите название без кириллицы или пробелов: ";
                        cin >> setName;
                        indexOfName = 0;
                    }
                    else {
                        indexOfName++;
                    }
                setVector.push_back(new MySet());
                setVector.back()->SetName(setName);
                setVector.back()->SetMinUniversal(minSetRange);
                setVector.back()->SetMaxUniversal(maxSetRange);
                cout << "Множество " << setName << " успешно создано." << endl;
                break;
            case '2':
                flag = true;
                while (flag) {
                    system("cls");
                    cout << "Выберите параметры создаваемого множества:\n";
                    cout << "1 - Делтся на n\n2 - Четное\n3 - Нечетное\n4 - >n\n5 - <n\n6 - Закончить создание множества.\n";
                    for (int i = 0; i < tempSetVector.size(); i++) {
                        cout << (*tempSetVector[i]).GetName() << endl;
                    }
                    cin >> menuChoice;
                    switch (menuChoice) {
                    case '1':
                        cout << "Введите n: ";
                        cin >> n;
                        tempSetVector.push_back(new MySet());
                        (*tempSetVector[tempSetVector.size() - 1]).SetName("Кратное " + to_string(n) + ";");
                        for (int i = minSetRange; i <= abs(maxSetRange); i++) {
                            if (i % n == 0) {
                                (*tempSetVector[tempSetVector.size() - 1]).Insert(i);
                            }
                        }
                        system("cls");
                        break;
                    case '2':
                        tempSetVector.push_back(new MySet());
                        (*tempSetVector[tempSetVector.size() - 1]).SetName("Четное;");
                        for (int i = minSetRange; i <= abs(maxSetRange); i++) {
                            if (i % 2 == 0) {
                                (*tempSetVector[tempSetVector.size() - 1]).Insert(i);
                            }
                        }
                        break;
                    case '3':
                        tempSetVector.push_back(new MySet());
                        (*tempSetVector[tempSetVector.size() - 1]).SetName("Нечетное;");
                        for (int i = minSetRange; i <= abs(maxSetRange); i++) {
                            if (i % 2 != 0) {
                                (*tempSetVector[tempSetVector.size() - 1]).Insert(i);
                            }
                        }
                        break;
                    case '4':
                        cout << "Введите n: ";
                        cin >> n;
                        tempSetVector.push_back(new MySet());
                        (*tempSetVector[tempSetVector.size() - 1]).SetName("Больше " + to_string(n) + ";");
                        for (int i = minSetRange; i <= abs(maxSetRange); i++) {
                            if (i > n) {
                                (*tempSetVector[tempSetVector.size() - 1]).Insert(i);
                            }
                        }
                        break;
                    case '5':
                        cout << "Введите n: ";
                        cin >> n;
                        tempSetVector.push_back(new MySet());
                        (*tempSetVector[tempSetVector.size() - 1]).SetName("Меньше " + to_string(n) + ";");
                        for (int i = minSetRange; i <= abs(maxSetRange); i++) {
                            if (i < n) {
                                (*tempSetVector[tempSetVector.size() - 1]).Insert(i);
                            }
                        }
                        break;
                    case '6':
                        if (tempSetVector.size() != 0) {
                            cout << "Задайте название множества: ";
                            cin >> setName;

                            for (int i = tempSetVector.size() - 1; i >= 0; i--) {
                                *tempSetVector[0] = *tempSetVector[0] * (*tempSetVector[i]);
                            }
                            setVector.push_back(new MySet());
                            *setVector[setVector.size() - 1] = *tempSetVector[0];
                            setVector[setVector.size() - 1]->SetName(setName);
                            (*setVector[setVector.size() - 1]).SetMaxUniversal(maxSetRange);
                            (*setVector[setVector.size() - 1]).SetMinUniversal(minSetRange);
                            tempSetVector[0] = NULL;
                            ClearAll(tempSetVector);
                        }
                        flag = false;
                        break;
                    }
                }
                break;
            }
            break;
        }
        case '2': // Удалить множество
        {
            system("cls");
            if (setVector.empty()) {
                cout << "Нет созданных множеств для удаления." << endl;
                break;
            }

            cout << "Выберите множество для удаления:" << endl;
            for (size_t i = 0; i < setVector.size(); i++) {
                cout << i << ". " << setVector[i]->GetName() << endl;
            }

            int deleteChoice;
            cin >> deleteChoice;

            if (deleteChoice >= 0 && deleteChoice < static_cast<int>(setVector.size())) {
                delete setVector[deleteChoice];
                setVector.erase(setVector.begin() + deleteChoice);
                cout << "Множество успешно удалено." << endl;
            }
            else {
                cout << "Недопустимый выбор." << endl;
            }
        }
        break;
        case '3': // Добавить элементы в множество
        {
            system("cls");
            string indexOfDeleting;
            int elementOfSet;
            cout << "Как вы хотте зменить множество?\n";
            cout << "1 - Добавить элементы\n";
            cout << "2 - Удалить элементы\n";
            cin >> menuChoice;
            switch (menuChoice)
            {
            case '1':
                if (setVector.empty()) {
                    cout << "Нет созданных множеств для добавления элементов." << endl;
                    break;
                }

                cout << "Выберите множество, в которое хотите добавить элементы:" << endl;
                ShowAllSets(setVector);
                int addToSet;
                cin >> addToSet;

                if (addToSet >= 0 && addToSet < static_cast<int>(setVector.size())) {
                    cout << "Как вы хотите заполнить множество?" << endl;
                    cout << "1. Случайными числами" << endl;
                    cout << "2. Вручную" << endl;

                    int fillChoice;
                    cin >> fillChoice;

                    MySet* targetSet = setVector[addToSet];

                    if (fillChoice == 1) {
                        int count;
                        cout << "Введите количество элементов для добавления: ";
                        cin >> count;

                        for (int i = 0; i < count; i++) {
                            int element = rand() % (maxSetRange - minSetRange + 1) + minSetRange;
                            targetSet->Insert(element);
                        }

                        cout << "Элементы успешно добавлены в множество." << endl;
                    }
                    else if (fillChoice == 2) {
                        int element;
                        cout << "Введите элементы для добавления (целые числа), чтобы закончить, введите 444:" << endl;

                        while (true) {
                            cin >> element;
                            if (element == 444) {
                                break;
                            }
                            targetSet->Insert(element);
                        }

                        cout << "Элементы успешно добавлены в множество." << endl;
                    }
                    else {
                        cout << "Недопустимый выбор." << endl;
                    }
                }
                else {
                    cout << "Недопустимый выбор." << endl;
                }
                break;
            case '2':
                cout << "Введите номер множества в котором хотите удалить элементы:\n";
                ShowAllSets(setVector);
                cin >> indexOfDeleting;
                if (IsNumber(indexOfDeleting)) {
                    if (setVector.size() > stoi(indexOfDeleting) and setVector.size() != 0
                        and stoi(indexOfDeleting) >= 0) {
                        while (menuChoice != '1') {
                            cout << "Введите значение элемента, которое хотите удалить: ";
                            cin >> elementOfSet;
                            (*setVector[stoi(indexOfDeleting)]).Remove(elementOfSet);
                            cout << "Закончить?\n1 - Да\n2 - Нет\n";
                            cin >> menuChoice;
                        }
                    }
                }
                break;
            default:
                break;
            }
        }
            
        break;

        case '4': // Вычислить выражение
        {
            system("cls");
            if (setVector.empty()) {
                cout << "Нет созданных множеств для вычисления выражения." << endl;
                break;
            }

            string primer;
            cin.ignore(); // Clear the newline character from the previous input
            cout << "Введите выражение, используя имена множеств (например, A + B * ! C):" << endl;
            getline(cin, primer);
            if (Calculate(setVector, primer)) {
                cout << "Результат успешно вычислен." << endl;
            }
            else {
                cout << "Ошибка при вычислении выражения." << endl;
            }
            cin >> primer;
        }
        break;

        case '5': // Очистить все множества
        {

            for (MySet* set : setVector) {
                delete set;
            }
            setVector.clear();
            cout << "Все множества успешно очищены." << endl;
        }
        break;

        case '6': // Выйти из программы
        {
            for (MySet* set : setVector) {
                delete set;
            }
            setVector.clear();
            programIsRunning = false;
        }
        break;

        default:
            cout << "Недопустимый выбор." << endl;
            break;
        }
    }

    return 0;
}
