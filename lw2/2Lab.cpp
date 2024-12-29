/*
 * Кузнецов Павел ПС-23, Microsoft Visual Studio 2019
 *  19. Организовать  в  основной  памяти с помощью указателей
 * стек из очередей. Обеспечить   операции   ведения  очереди из
 * вершины   стека,   расширения   и  сокращения  стека,  выдачи
 * содержимого стека (9).
 */

#include <iostream>
#include <windows.h>
#include <conio.h>

struct node_of_queue
{
    std::string value;
    node_of_queue* next = nullptr;
};

struct node_of_stack
{
    node_of_queue* first = nullptr;
    node_of_queue* last = nullptr;
    node_of_stack* next = nullptr;
    int queue_len = 0;
};

void add_to_queue(node_of_stack*& node, std::string value)
{
    if (node->first == nullptr)
    {
        node->first = new node_of_queue;
        node->first->value = value;
        node->last = node->first;
        node->queue_len = 1;
    }
    else
    {
        node->last->next = new node_of_queue;
        node->last = node->last->next;
        node->last->value = value;
        node->queue_len++;
    }
}

std::string get_from_queue(node_of_queue*& node)
{
    if (node == nullptr)
    {
        return "";
    }

    std::string value = node->value;

    node_of_queue* temp = node;
    node = node->next;

    temp = nullptr;
    delete(temp);

    return value;
}

void write_queue(node_of_queue* node)
{
    if (node == nullptr)
    {
        return;
    }

    std::cout << node->value << std::endl;
    write_queue(node->next);
}

void push(node_of_stack*& head)
{
    node_of_stack* temp;
    temp = new(node_of_stack);

    temp->next = head;
    head = temp;
}

void pop(node_of_stack*& head)
{
    if (head == nullptr)
    {
        return;
    }

    std::string word = get_from_queue(head->first);
    while (word != "")
    {
        std::cout << word << std::endl;
        word = get_from_queue(head->first);
    }

    node_of_stack* temp;
    temp = head;
    head = head->next;

    temp = nullptr;
    delete(temp);
}

void write_all_stack(node_of_stack* node)
{
    if (node == nullptr)
    {
        return;
    }

    if (node->queue_len == 0)
    {
        std::cout << "Очередь пуста" << std::endl;
    }
    else
    {
        write_queue(node->first);
    }
    std::cout << "<-------->" << std::endl;

    write_all_stack(node->next);
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    node_of_stack* head;
    head = new(node_of_stack);

    bool exit = false;
    int stack_len = 1;

    while (!exit)
    {
        std::cout << std::endl;
        std::cout << "Длина стэка: " << stack_len << std::endl;
        if (head != nullptr)
        {
            std::cout << "Длина очереди: " << head->queue_len << std::endl;
        }
        std::cout << "Что вы хотите сделать?" << std::endl;
        std::cout << "1. Добавить элементы в очередь" << std::endl;
        std::cout << "2. Взять элемент из очереди" << std::endl;
        std::cout << "3. Распечатать очередь" << std::endl;
        std::cout << "4. Добавить элемент в стэк" << std::endl;
        std::cout << "5. Удалить элемент с стека" << std::endl;
        std::cout << "6. Распечать весь стэк" << std::endl;
        std::cout << "7. Выход из программы" << std::endl;

        int ans;

        std::cin >> ans;

        switch (ans)
        {
        case 1:
        {
            if (stack_len == 0)
            {
                std::cout << "Стэк пуст" << std::endl;
                std::cout << std::endl << "Нажмите любую клавишу для продолжения" << std::endl;
                char ch;
                ch = _getch();
            }
            else
            {
                std::cout << "Введите * если хватит" << std::endl;
                std::string word;
                std::cin >> word;
                while (word != "*")
                {
                    add_to_queue(head, word);
                    std::cin >> word;
                }
            }
        }
        break;
        case 2:
        {
            if (stack_len == 0)
            {
                std::cout << "Стэк пуст" << std::endl;
            }
            else if (head->queue_len == 0)
            {
                std::cout << "Нет элементов в очереди" << std::endl;
            }
            else
            {
                std::cout << std::endl;
                std::string word;
                word = get_from_queue(head->first);
                std::cout << word << std::endl;
                head->queue_len--;
            }
            std::cout << std::endl << "Нажмите любую клавишу для продолжения" << std::endl;
            char ch;
            ch = _getch();
        }
        break;
        case 3:
        {
            if (stack_len == 0)
            {
                std::cout << "Стэк пуст" << std::endl;
            }
            else if (head->queue_len == 0)
            {
                std::cout << "Очередь пуста" << std::endl;
            }
            else
            {
                std::cout << std::endl;
                write_queue(head->first);
            }
            std::cout << std::endl << "Нажмите любую клавишу для продолжения" << std::endl;
            char ch;
            ch = _getch();
        }
        break;
        case 4:
        {
            stack_len++;
            push(head);
        }
            break;
        case 5:
        {
            if (stack_len == 0)
            {
                std::cout << "Стэк пуст" << std::endl;
            }
            else
            {
                std::cout << std::endl;
                pop(head);
                stack_len--;
            }
            std::cout << std::endl << "Нажмите любую клавишу для продолжения" << std::endl;
            char ch;
            ch = _getch();
        }
            break;
        case 6:
        {
            std::cout << std::endl;
            write_all_stack(head);
            std::cout << std::endl << "Нажмите любую клавишу для продолжения" << std::endl;
            char ch;
            ch = _getch();
        }
        break;
        case 7:
        {
            exit = true;
        }
        break;
        default:
            break;
        }
    }
}
