/*
* Кузнецов Павел, ПС-23, Microsoft Visual Studio 2019
*28. На равнинной местности имеется сеть автомобильных дорог
с двусторонним движением. Для  каждой  пары  городов  известно 
расстояние между ними и длина дороги в том  случае,  если  она 
имеется.  Оказалось,  что  некоторые  пары  городов  не  имеют 
автомобильного  сообщения.  Принято  решение  построить  новые 
дороги,  связывающие  напрямую  некоторые  города, так,  чтобы
существовал путь между любыми  двумя  городами, а общая  длина
новых  дорог была минимальна. Найти такие  дороги и определить
их общую длину (12). 
*/

#include <iostream>
#include <vector>
#include "windows.h"
#include <algorithm>

struct ArcStruct
{
	int start;
	int end;
	int value;
};

std::vector<int> createVectorOfVertexes(int numOfVertexes)
{
	std::vector<int> vertexes;
	vertexes.push_back(0);
	for (int i = 1; i <= numOfVertexes; i++)
	{
		vertexes.push_back(i);
	}
	return vertexes;
}

bool checkGraph(std::vector<int> vertexes)
{
	int value = vertexes[1];

	for (int i = 2; i < vertexes.size(); i++)
	{
		if (vertexes[i] != value)
		{
			return false;
		}
	}

	return true;
}

ArcStruct createNodeDistance(int start, int end, int distance)
{
	ArcStruct node;
	node.start = start;
	node.end = end;
	node.value = distance;
	return node;
}

std::vector<int> addNewArc(std::vector<int> vertexes, int start, int end)
{
	int value = vertexes[end];
	for (int i = 1; i < vertexes.size(); i++)
	{
		if (vertexes[i] == value)
		{
			vertexes[i] = vertexes[start];
		}
	}
	return vertexes;
}

std::vector<int> readRoads(int numOfRoads, std::vector<int> vertexes)
{
	for (int i = 0; i < numOfRoads; i++)
	{
		int start, end;
		std::cin >> start >> end;
		vertexes = addNewArc(vertexes, start, end);
	}
	return vertexes;
}

std::vector<ArcStruct> readDistances(std::vector<int> vertexes)
{
	std::vector<ArcStruct> distances;
	int start, end, distance;
	while (std::cin >> start >> end >> distance)
	{
		/*
		if (vertexes[start] != vertexes[end])
		{
			ArcStruct newDistance;
			newDistance.start = start;
			newDistance.end = end;
			newDistance.value = distance;
			//newDistance = createNodeDistance(start, end, distance);
			distances.push_back(newDistance);
		}
		*/
	}

	return distances;
}

bool compareByDistance(ArcStruct first, ArcStruct second)
{
	return first.value < second.value;
}

/*
struct ArcStruct
{
	int end;
	int distance;
};

struct Vertex
{
	bool isMarked;
	int minDistance;
	int from;
};

std::vector<std::vector<ArcStruct>> createVectorOfArcs(int numOfV)
{
	std::vector<std::vector<ArcStruct>> vector;

	for (int i = 0; i < numOfV; i++)
	{
		std::vector<ArcStruct> Arcs;
		vector.push_back(Arcs);
	}

	return vector;
}

std::vector<std::vector<ArcStruct>> readRoads(std::vector<std::vector<ArcStruct>> vetrexArcs, int numOfR)
{
	for (int i = 0; i < numOfR; i++)
	{
		int start, end;
		std::cin >> start >> end;
		ArcStruct newR;
		newR.end = end;
		newR.distance = 0;
		vetrexArcs[start - 1].push_back(newR);
	}

	return vetrexArcs;
}

std::vector<std::vector<ArcStruct>> readDistance(std::vector<std::vector<ArcStruct>> vertexArcs)
{
	int start, end, distance;
	while (std::cin >> start >> end >> distance)
	{
		ArcStruct nD;
		nD.end = end;
		nD.distance = distance;
		vertexArcs[start - 1].push_back(nD);
	}

	return vertexArcs;
}
*/
int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	int numOfVertexes, numOfRoads;

	std::cin >> numOfVertexes >> numOfRoads;
	/*
	std::vector<std::vector<ArcStruct>> vertexArcs = createVectorOfArcs(numOfVertexes);
	vertexArcs = readRoads(vertexArcs, numOfRoads);
	vertexArcs = readDistance(vertexArcs);
	/*
	for (int i = 0; i < vertexArcs.size(); i++)
	{
		std::cout << i + 1 << " -> ";
		for (ArcStruct road : vertexArcs[i])
		{
			std::cout << road.end << "(" << road.distance << ") ";
		}
		std::cout << "\n";
	}
	*/


	std::vector<int> vertexes = createVectorOfVertexes(numOfVertexes);
	vertexes = readRoads(numOfRoads, vertexes);
	if (checkGraph(vertexes))
	{
		std::cout << "0\n";
		return 0;
	}

	std::vector<ArcStruct> distances;
	std::cout << "1\n";
	distances = readDistances(vertexes);
	std::cout << "2\n";
	std::sort(distances.begin(), distances.end(), compareByDistance);

	std::cout << "3\n";
	for (ArcStruct distance : distances)
	{
		if (vertexes[distance.start] != vertexes[distance.end])
		{
			std::cout << distance.start << "->" << distance.end << ": " << distance.value << "\n";
			vertexes[0] += distance.value;
			vertexes = addNewArc(vertexes, distance.start, distance.end);
			if (checkGraph(vertexes))
			{
				break;
			}
		}
	}
	std::cout << "4\n";

	std::cout << "\n" << "Общая длина путей = " << vertexes[0];
}