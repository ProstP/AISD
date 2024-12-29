/*23. Имеется   телефонный   справочник   со   сведениями   о
владельцах  телефонов.Организовать  индексацию  по   номерам
телефонов с помощью построения Б - дерева.Хранить дерево в виде
самостоятельного  файла.Обеспечить  дополнение, удаление  и
корректировку  информации.Проиллюстрировать   этапы   поиска
информации по заданному номеру телефона(13). 
Кузнецов Павел ПС-23, Microsoft Visual Studio 2019
*/

#define _CRT_SECURE_NO_DEPRECATE 
#include <iostream>
#include <windows.h>
#include <vector>
#include <conio.h>
#include <fstream>
#include <stdio.h>

const int FIND_KEY_RETURN = 0;
const int NAME_MAX_LENGTH = 20;
const int NULL_INDEX = -1;
const int ORDER = 2;
const char VERT_LINE = '|';
const char HOR_LINE = '-';
const char SPACE = ' ';
const std::string IN_FILE_NAME = "input1.txt";

struct Record
{
	int key;
	char name[NAME_MAX_LENGTH];
	bool isDeleted = false;
 };

struct TreeNode
{
	Record keys[2 * ORDER];
	int children[2 * ORDER + 1];
};

Record createRecord(int key, std::string name)
{
	Record newRec;
	newRec.key = key;
	std::strcpy(newRec.name, name.c_str());
	return newRec;
}

TreeNode createNewNode(Record record)
{
	TreeNode node;
	node.keys[0] = record;
	for (int i = 1; i < 2 * ORDER; i++)
	{
		node.keys[i].key = NULL_INDEX;
	}
	for (int i = 0; i < 2 * ORDER + 1; i++)
	{
		node.children[i] = NULL_INDEX;
	}
	return node;
}

TreeNode initNode()
{
	TreeNode node;
	for (int i = 0; i < 2 * ORDER; i++)
	{
		node.keys[i].key = NULL_INDEX;
	}
	for (int i = 0; i < 2 * ORDER + 1; i++)
	{
		node.children[i] = NULL_INDEX;
	}
	return node;
}

void writeNewNodeToFile(std::fstream& binFile, TreeNode node, int& counter)
{
	counter++;
	binFile.seekg(counter * sizeof(TreeNode), std::ios::beg);
	binFile.write((char*)&node, sizeof(TreeNode));
}

void updateNodeInFile(std::fstream& binFile, TreeNode node, int position)
{
	binFile.seekg(position * sizeof(TreeNode), std::ios::beg);
	binFile.write((char*)&node, sizeof(TreeNode));
}

TreeNode readNodeFromFile(std::fstream& binFile, int position)
{
	binFile.seekg(position * sizeof(TreeNode), std::ios::beg);
	TreeNode node;
	binFile.read((char*)&node, sizeof(TreeNode));
	return node;
}

bool isNodeLeaf(TreeNode node)
{
	for (int i : node.children)
	{
		if (i != NULL_INDEX)
		{
			return false;
		}
	}

	return true;
}

int getKeysArraySize(TreeNode node)
{
	int count = 0;
	for (Record rec : node.keys)
	{
		if (rec.key != NULL_INDEX)
		{
			count++;
		}
	}

	return count;
}

TreeNode insertRecordToNode(TreeNode node, Record record)
{
	int pos = 0;
	for (pos; pos < 2 * ORDER; pos++)
	{
		if ((record.key < node.keys[pos].key) || (node.keys[pos].key == NULL_INDEX))
		{
			break;
		}
	}

	for (int i = ORDER * 2 - 2; i >= pos; i--)
	{
		node.keys[i + 1] = node.keys[i];
	}
	node.keys[pos] = record;
	return node;
}

std::vector<Record> getKeysAndNewRecordInVector(TreeNode node, Record record)
{
	std::vector<Record> vector;
	bool isNewWritten = false;
	for (Record currRec : node.keys)
	{
		if ((record.key < currRec.key) && (!isNewWritten))
		{
			vector.push_back(record);
			isNewWritten = true;
		}
		vector.push_back(currRec);
	}
	if (!isNewWritten)
	{
		vector.push_back(record);
	}
	return vector;
}

Record splitNode(std::fstream& binFile, TreeNode node, int nodePos, Record newRec, int child, int& counter)
{
	std::vector<Record> vectorOfKeys = getKeysAndNewRecordInVector(node, newRec);
	int middle = (vectorOfKeys.size() - 1) / 2;
	std::vector<int> vectorOfChildren;
	for (int i = 0; i <= ORDER * 2; i++)
	{
		if (node.children[i] == NULL_INDEX)
		{
			continue;
		}
		vectorOfChildren.push_back(node.children[i]);
		if (i == child)
		{
			vectorOfChildren.push_back(counter);
		}
	}
	node = initNode();
	TreeNode secondNode = initNode();
	for (int i = 0; i < middle; i++)
	{
		node.keys[i] = vectorOfKeys[i];
	}
	for (int i = middle + 1; i < vectorOfKeys.size(); i++)
	{
		secondNode.keys[i - middle - 1] = vectorOfKeys[i];
	}

	if (child != NULL_INDEX)
	{
		int arraySizeForNode = getKeysArraySize(node);
		for (int i = 0; i <= arraySizeForNode; i++)
		{
			node.children[i] = vectorOfChildren[i];
		}
		for (int i = vectorOfChildren.size() - getKeysArraySize(secondNode) - 1; i < vectorOfChildren.size(); i++)
		{
			secondNode.children[i - arraySizeForNode - 1] = vectorOfChildren[i];
		}
	}

	updateNodeInFile(binFile, node, nodePos);
	writeNewNodeToFile(binFile, secondNode, counter);
	return vectorOfKeys[middle];
}

Record insertRecordToTree(std::fstream& binFile, Record rec, int nodePos, int& counter)
{
	TreeNode node = readNodeFromFile(binFile, nodePos);
	Record returnValue;
	returnValue.key = NULL_INDEX;

	if (isNodeLeaf(node))
	{
		if (getKeysArraySize(node) == ORDER * 2)
		{
			returnValue = splitNode(binFile, node, nodePos, rec, NULL_INDEX, counter);
		}
		else
		{
			node = insertRecordToNode(node, rec);
			updateNodeInFile(binFile, node, nodePos);
		}
	}
	else
	{
		int child = 0;
		int arraySize = getKeysArraySize(node);
		for (child; child < arraySize; child++)
		{
			if (rec.key < node.keys[child].key)
			{
				break;
			}
		}
		Record value = insertRecordToTree(binFile, rec, node.children[child], counter);
		if (value.key != NULL_INDEX)
		{
			if (getKeysArraySize(node) < ORDER * 2)
			{
				node = insertRecordToNode(node, value);
				for (int i = ORDER * 2 - 1; i > child; i--)
				{
					node.children[i + 1] = node.children[i];
				}
				node.children[child + 1] = counter;
				updateNodeInFile(binFile, node, nodePos);
			}
			else
			{
				returnValue = splitNode(binFile, node, nodePos, value, child, counter);
			}
		}
	}
	return returnValue;
}

int addNewRecordToTree(std::fstream& binFile, Record rec, int rootPos, int& counter)
{
	if (rootPos == NULL_INDEX)
	{
		writeNewNodeToFile(binFile, createNewNode(rec), counter);
		return counter;
	}
	else
	{
		Record value = insertRecordToTree(binFile, rec, rootPos, counter);
		if (value.key != NULL_INDEX)
		{
			TreeNode newRoot = createNewNode(value);
			newRoot.children[0] = rootPos;
			newRoot.children[1] = counter;
			writeNewNodeToFile(binFile, newRoot, counter);
			return counter;
		}
	}

	return rootPos;
}

void print(std::fstream& binFile, int pos, std::string indent)
{
	TreeNode node = readNodeFromFile(binFile, pos);
	for (Record record : node.keys)
	{
		if (record.key == NULL_INDEX)
		{
			continue;
		}
		if (record.isDeleted)
		{
			std::cout << "!";
		}
		std::cout << record.key << " ";
	}

	std::cout << "\n";

	for (int i : node.children)
	{
		if (i == NULL_INDEX)
		{
			continue;
		}
		std::cout << indent + VERT_LINE << "\n" << indent + VERT_LINE + HOR_LINE;
		print(binFile, i, indent + VERT_LINE + SPACE);
	}
}

void printNode(TreeNode node)
{
	std::cout << '(';
	for (int i = 0; i < ORDER * 2; i++)
	{
		if (node.keys[i].key == NULL_INDEX)
		{
			continue;
		}
		if (i != 0)
		{
			std::cout << " ";
		}
		std::cout << node.keys[i].key;
	}
	std::cout << ")";
}

int findUser(std::fstream& binFile ,int pos, int key)
{
	TreeNode node = readNodeFromFile(binFile, pos);
	printNode(node);

	bool find = false;
	for (int i = 0; i < ORDER * 2; i++)
	{
		if (node.keys[i].key == key)
		{
			find = true;
			std::cout << "\n" << node.keys[i].key << " " << node.keys[i].name << "\n";
			return pos;
		}
	}

	if (isNodeLeaf(node))
	{
		return NULL_INDEX;
	}

	int child = 0;
	for (child; child < ORDER * 2; child++)
	{
		if ((key < node.keys[child].key) || (node.keys[child].key == NULL_INDEX))
		{
			break;
		}
	}
	std::cout << "-{" << child + 1 << "}->";
	return findUser(binFile, node.children[child], key);
}

void writeRecordsFromFileToVector(std::fstream& binFile, int pos, std::vector<Record>& vector)
{
	TreeNode node = readNodeFromFile(binFile, pos);
	for (int i = 0; i < ORDER * 2; i++)
	{
		if ((node.keys[i].key != NULL_INDEX) && (!node.keys[i].isDeleted))
		{
			vector.push_back(node.keys[i]);
		}
	}
	for (int i = 0; i < ORDER * 2 + 1; i++)
	{
		if (node.children[i] != NULL_INDEX)
		{
			writeRecordsFromFileToVector(binFile, node.children[i], vector);
		}
	}
}

int clearFile(std::fstream& binFile, int rootPos)
{
	std::vector<Record> vector;
	writeRecordsFromFileToVector(binFile, rootPos, vector);
	binFile.clear();
	int counter = 0;
	rootPos = NULL_INDEX;
	for (int i = 0; i < vector.size(); i++)
	{
		rootPos = addNewRecordToTree(binFile, vector[i], rootPos, counter);
	}

	return rootPos;
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	std::string inFileName;
	std::cout << "Введите название входного файла: ";
	std::cin >> inFileName;
	std::ifstream inFile(inFileName);
	//std::ifstream inFile(IN_FILE_NAME);
	if (!inFile.is_open())
	{
		std::cout << "Входной файл не найден!\n";
		return 0;
	}

	//FILE *binFile;
	//binFile = fopen("tree.bin", "r+");
	std::fstream binFile("tree.bin", std::ios::in|std::ios::out|std::ios::binary);
	int rootPos = NULL_INDEX;

	int key;
	int counter = -1;
	std::string name;

	while (inFile >> key >> name)
	{
		rootPos = addNewRecordToTree(binFile, createRecord(key, name), rootPos, counter);
	}

	bool isExit = false;
	while (!isExit)
	{
		system("cls");
		print(binFile, rootPos, "");
		std::cout << "Выберите что хотите сделать?\n"
			<< "[1]Добавить новую запись\n"
			<< "[2]Найти запись по номеру\n"
			<< "[3]Отметить записи, которые нужно удалить\n"
			<< "[4]Удалить отмеченные\n"
			<< "[0]Выход\n";
		char comm;
		std::cin >> comm;
		int key = 0;
		int pos;
		int currentRec = 0;
		switch (comm)
		{
		case '1':
			std::cout << "Введите новую запись: ";
			std::cin >> key >> name;
			pos = findUser(binFile, rootPos, key);
			if (pos != NULL_INDEX)
			{
				std::cout << "Пользователь уже есть. Вы можете изменить его данные, если хотите?\n[1]Да\n[2]Нет\n";
				/*Можно вывести в метод отдельный*/
				std::cin >> comm;
				if (comm == '1')
				{
					TreeNode node = readNodeFromFile(binFile, pos);
					for (currentRec = 0; currentRec < ORDER * 2; currentRec++)
					{
						if (node.keys[currentRec].key == key)
						{
							break;
						}
					}
					std::strcpy(node.keys[currentRec].name, name.c_str());
					updateNodeInFile(binFile, node, pos);
				}
			}
			else
			{
				rootPos = addNewRecordToTree(binFile, createRecord(key, name), rootPos, counter);
			}
			break;
		case '2':
			std::cout << "Введите номер: ";
			std::cin >> key;
			pos = findUser(binFile, rootPos, key);
			if (pos == NULL_INDEX)
			{
				std::cout << "\n" << "Пользователь не найден.\n";
				char ch = _getch();
			}
			else
			{
				TreeNode node = readNodeFromFile(binFile, pos);
				for (currentRec = 0; currentRec < ORDER * 2; currentRec++)
				{
					if (node.keys[currentRec].key == key)
					{
						break;
					}
				}
				if (node.keys[currentRec].isDeleted)
				{
					std::cout << "Пользователь отмечен как удалён, хотите убрать метку?\nДа[1]\nНет[2]\n";
					std::cin >> comm;
					if (comm == '1')
					{
						node.keys[currentRec].isDeleted = false;
						updateNodeInFile(binFile, node, pos);
					}
					break;
				}
				std::cout << "Изменить данные о пользователе?\n[1]Да\n[2]Нет\n";
				std::cin >> comm;
				if (comm == '1')
				{
					std::cout << "Введите новые данные: ";
					std::string name;
					std::cin >> name;
					std::strcpy(node.keys[currentRec].name, name.c_str());
					updateNodeInFile(binFile, node, pos);
				}
			}
			break;
		case '3':
			std::cout << "Отметьте какие записи должны быть удалены(введите 'z' чтобы прекратить): ";
			while (true)
			{
				std::string command;
				std::cin >> command;
				if (command == "z")
				{
					//isRemoving = false;
					break;
				}
				key = atoi(command.c_str());
				pos = findUser(binFile, rootPos, key);
				if (pos != NULL_INDEX)
				{
					TreeNode node = readNodeFromFile(binFile, pos);
					for (currentRec = 0; currentRec < ORDER * 2; currentRec++)
					{
						if (node.keys[currentRec].key == key)
						{
							break;
						}
					}
					node.keys[currentRec].isDeleted = true;
					updateNodeInFile(binFile, node, pos);
				}
			}
			break;
		case '4':
			rootPos = clearFile(binFile, rootPos);
			break;
		case '0':
			isExit = true;
			break;
		default:
			break;
		}
	}

	binFile.close();
	inFile.close();
}