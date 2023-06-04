#include <iostream>
#include <vector>
#include <time.h>
#include <fstream>
#include <random>
#include <fstream>
#include <algorithm>
#include <string>
#include <thread>
#include <windows.h>

#define NETWORK_FILENAME ".network"
#define INITIAL_NETWORK_LINKS_VALUE 1
#define INITIAL_NETWORK_BIAS_VALUE 0

//This works in reverse! 15 = 1/15 chance etc
#define LINK_EVOLUTION_CHANCE 10
#define BIAS_EVOLUTION_CHANCE 2

//Determines how much a descandant network differs from it's parents
#define GENERAL_EVOLUTION_SPEED_FACTOR 5

#define POPULATION_SIZE 100
#define PARENTS_POPULATION_SIZE 10

//This number determines how many test examples a network is evaluated on during training
#define POPULATION_SAMPLING_VALUE 1000

//Thread number of your machine goes here!
#define THREAD_NUMBER 4

//This actually has nothing to do with gradient descent. It is called ,,gradient descent" for a joke.
const int gradient_descent[PARENTS_POPULATION_SIZE] = { 1, 2, 3, 4, 5, 7, 15, 30, 60, 100 };

using namespace std;

template<typename T>
using Activator = T(*)(T);

#include "network.h"

int value(int x)
{
	return x >= 0 ? x : x * (-1);
}

struct DataSet
{
	vector<double>data;
	int correct_result = 0;
};

#include "colors.h"
#include "digits.h"

#include "trainer.h"

double activator(double value) {
	return max((double)0.0, value);
}

int main()
{

	srand(time(NULL));
	//Put network layer sizes here, starting from the input layer:
	vector<int>sizes = {196, 25, 10};
	Network<double, (double) -5, (double)5, activator> network(sizes);
	Trainer<double, (double) -5, (double) 5, activator, Digits> trainer(&network);

	while (true)
	{
		string command;
		cin >> command;
		if (command == "save" || command=="load")
		{
			string name;
			cin >> name;
			if (command == "save")
			{
				trainer.return_network()->save(name);
			}
			else
			{
				trainer.return_network()->load(name);
			}
		}
		if (command == "train")
		{
			int how_many;
			cin >> how_many;
			trainer.train(how_many);
		}
		if (command == "try")
		{
			vector<double>data;
			int size;
			cin >> size;
			for (int i = 0; i < size; i++)
			{
				int d;
				cin >> d;
				data.push_back(d);
			}
			vector<double>& result = trainer.calculate(data);
			cout << result[0] << endl;
		}
		if (command == "test")
		{
			int skip;
			cin >> skip;
			trainer.test(skip);
			
		}
		if (command == "display")
		{
			trainer.return_network()->display();
		}
		if (command == "try_random")
		{
			trainer.try_random();
		}
		if (command == "kill_parents")
		{
			trainer.kill_parents();
		}
	}

	return 0;
}

