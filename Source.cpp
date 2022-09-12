#include <iostream>
#include <string>
#include <fstream>
#include <locale.h>
#define COUNT 10

using namespace std;

class Human
{
private:
    int numbers[15] = { 9, 5, 13, 2, 7, 11, 15, 1, 3, 6, 8, 10, 12, 14, 16 }; // числа, отвечающие за позицию в дереве { 9, 5, 13, 2, 7, 11, 15, 1, 3, 6, 8, 10, 12, 14, 16 }
public:
    string name;
    string surname;
    string age;
    int number;

    long passport(int pos, int num)
    {
        /*
        * pos - позиция курсора в файле
        * num - номер члена семьи (начиная с нуля) - указывает на расположение в дереве
        */

        string temp;
        long seek = 0;
        ifstream file;

        file.open("file.txt");

        /*
        * получаем строки, а затем разбиваем их по пробелу
        * чтобы заполнить поля name, surname и age
        *
        * в переменной seek отслеживаем позицию (курсор) в файле
        */

        file.seekg(pos);

        getline(file, temp, ' ');
        name = temp;
        seek += temp.length();

        getline(file, temp, ' ');
        surname = temp;
        seek += temp.length();

        getline(file, temp, '\n');
        age = temp;
        seek += temp.length();

        number = numbers[num];

        file.close();

        return seek + 4; // два пробела и символ \n
    }
};

typedef struct Tree
{
    int numbers[15] = { 9, 5, 13, 2, 7, 11, 15, 1, 3, 6, 8, 10, 12, 14, 16 };
    Human human;
    Tree* left;
    Tree* right;
} Tree;

void addNode(Tree** root, Human* human, int num); // функция добавления узла в дерево
void printTree(Tree* root, int space); // функция вывода древа
void findHuman(Tree* root, Human* human, string name); //функция поиска человека в дереве

int main()
{
    system("chcp 1251");
    system("cls");
    long seek = 0; // seek следит за позицией в файле file.txt
    Human human[15]; // массив из трех экземпляров клсса Human

    for (int i = 0; i < 15; i++)
    {
        seek += human[i].passport(seek, i);
    }

    Tree* root = NULL;
    for (int j = 0; j < 15; j++)
    {
        addNode(&root, human, j);
    }

    int choice = 1;
    while (choice != 0)
    {
        cout << "1 - Conclusion of tree\n2 - Scan of a name\n0 - Exit\nEnter: ";
        cin >> choice;

        switch (choice)
        {
        case 0: cout << "\n-End of program-\n"; break;
        case 1: printTree(root, 0); break;
        case 2:
        {
            string name;
            cout << "\nEnter name of pepope, which you want to search: ";
            cin >> name;
            findHuman(root, human, name);
        } break;
        default: cout << "\n-Eror input-\n"; break;
        }
    }

    return 0;
}

void addNode(Tree** root, Human* human, int num)
{
    /*
    * Функция добавления нового узла в БД
    * Добавляется числовое значение (num)
    * и имя, которое добавляется из массива экземпляров класса Human
    * по индексу (который равен num)
    */
    if ((*root) == NULL) // если дерево пустое, то добавляем корень
    {
        (*root) = new Tree;
        (*root)->left = NULL;
        (*root)->right = NULL;
        (*root)->human = human[0];
        return;
    }

    Tree* newNode = new Tree;
    Tree* current = *root;
    Tree* temp = current;
    while (current != NULL)
    {
        temp = current;
        if (current->human.number > (*root)->numbers[num])
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }
    newNode->human = human[num];
    newNode->left = NULL;
    newNode->right = NULL;
    if (temp->human.number > newNode->human.number)
    {
        temp->left = newNode;
    }
    else
    {
        temp->right = newNode;
    }
}

void printTree(Tree* root, int space)
{
    if (root == NULL)
        return;

    space += COUNT;

    printTree(root->right, space);

    cout << "\n";

    for (int i = COUNT; i < space; i++)
        cout << (" ");
    cout << root->human.name << "\n";

    printTree(root->left, space);
    cout << "\n";
}

void findHuman(Tree* root, Human* human, string name)
{
    int num, flag = 0;
    for (int i = 0; i < 15; i++)
    {
        if (human[i].name == name)
        {
            num = human[i].number;
            flag = 1;
            break;
        }
    }

    if (flag == 0)
    {
        cout << "\nThere is no such person in the Family Tree!\n\n";
        return;
    }
    Tree* current = root;
    while (current->human.name != name)
    {
        if (current->human.number > num)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }

    cout << "\nName: " << current->human.name << "\n";
    cout << "Surname: " << current->human.surname << "\n";
    cout << "Age: " << current->human.age << "\n";

    printTree(current, 0);
}