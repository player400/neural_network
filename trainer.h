#pragma once

template<typename T, T min, T max, Activator<T> activator, typename D>
class Trainer
{
private:
	D* dataset;

	class Iteration
	{
	public:
		Network < T, min, max, activator>* network;
		int result = 0;
		int samples = 0;

		void evolve()
		{
			network->evolve();
		}

		int power(int x, int y)
		{
			int result = 1;
			for (int i = 0; i < y; i++)
			{
				result = result * x;
			}
			return result;
		}

		void evaluate(D* dataset)
		{
			result = 0;
			samples = 0;
			vector<int>digits;
			for (int i = 0; i < POPULATION_SAMPLING_VALUE; i++)
			{
				

				DataSet example = dataset->generator();
				vector<T> & final = network->calculate(example.data);
				for (int i = 9; i > 0; i--)
				{
					if (dataset->interpret(final) == example.correct_result)
					{
						result+=power(i,3);
						bool is_in_vector = false;
						for (int& digit : digits)
						{
							if (digit == dataset->interpret(final))
							{
								is_in_vector = true;
							}
						}
						if (!is_in_vector)
						{
							digits.push_back(example.correct_result);
						}
						break;
					}
					else
					{
						final[dataset->interpret(final)] = -300;
					}
				}
				
				samples++;
			}
			//result = result * digits.size();
		}

		Iteration(Network < T, min, max, activator>* network)
		{
			this->network = network;


		}
	};

	Network<T, min, max, activator>* current_network = nullptr;
	vector<Network<T, min, max, activator>*> parents;
	vector<Iteration> children;
	int last_delta = -1;

public:
	vector<T>& calculate(vector<T>data)
	{
		return current_network->calculate(data);
	}

	Network<T, min, max, activator>* return_network()
	{
		return current_network;
	}

	void swap(int j)
	{
		Iteration temp = children[j];
		children[j] = children.at(j + 1);
		children.at(j + 1) = temp;
	}


	struct Example
	{
		int answer=0;
		int correct=0;
		int total=0;
	};

	void test(int skip)
	{

		vector<Example>breakdown;
		for (int i = 0; i < 10; i++)
		{
			Example example;
			example.answer = i;
			breakdown.push_back(example);
		}
		int correct = 0;
		//vector<double>data;
		int total = 0;
		int total_red = 0;
		for (int i = 0; i < dataset->data.size(); i += skip)
		{
			DataSet example = dataset->data[i];
			if (example.correct_result == dataset->interpret(calculate(example.data)))
			{
				correct++;
				breakdown[example.correct_result].correct++;
			}
			breakdown[example.correct_result].total++;
			total++;
			//cout << i << "/" << dataset->data.size() << endl;
		}
		cout << correct << " " << total << endl;
		double percent = correct * 100.0;
		percent = percent / total;
		cout << percent << endl;
		for (int i = 0; i < 10; i++)
		{
			double percent2 = breakdown[i].correct * 100.0;
			percent2 = percent2 / breakdown[i].total;
			cout << i << " "<<endl;
			cout << breakdown[i].correct << " " << breakdown[i].total << " " << percent2 << endl;
		}
	}

	void sort()
	{
		for (int i = 0; i < children.size(); i++)
		{
			for (int j = 0; j < children.size() - 1; j++)
			{
				if (children[j].result < children[j + 1].result)
				{
					swap(j);
				}
			}
		}
	}

	vector<thread>threads;

	void evaluate_children(int start, int quantity)
	{
		for (int i = start; i < start + quantity; i++)
		{
			children[i].evaluate(dataset);
		}
	}

	bool all_threads_done()
	{
		bool result = true;
		for (int i=0;i<POPULATION_SIZE;i++)
		{
			if (children[i].samples < POPULATION_SAMPLING_VALUE)
			{
				result = false;
			}
		}
		return result;
	}

	void attempt(int iteration)
	{
	
		for (int i = 0; i+1 < PARENTS_POPULATION_SIZE; i+=2)
		{
			Network<T, min, max, activator>* network = parents[i];
			for (int j = 0; j < (POPULATION_SIZE/PARENTS_POPULATION_SIZE)*2; j++)
			{
				if (children.size()<POPULATION_SIZE)
				{
					children.emplace_back(network->copy());
				}
				else
				{
					children[i * PARENTS_POPULATION_SIZE + j].network->paste(network->export_network(), 1);
				}
			}

			Network<T, min, max, activator>* network2 = parents[i+1];
			for (int j = 0; j < (POPULATION_SIZE / PARENTS_POPULATION_SIZE) * 2; j++)
			{
				children[i * PARENTS_POPULATION_SIZE + j].network->paste(network2->export_network(), 2);
			}
		}


		for (int i = 0; i < POPULATION_SIZE; i++)
		{
			children[i].evolve();
		}

		threads.clear();
		
		int children_per_thread = POPULATION_SIZE / THREAD_NUMBER;
		for (int i = 0; i < THREAD_NUMBER-1; i++)
		{	
			threads.push_back(thread([=]() { evaluate_children(i * children_per_thread,  children_per_thread); }));
		}
		
		evaluate_children((THREAD_NUMBER - 1) * children_per_thread, children_per_thread);

		while (!all_threads_done())
		{
			Sleep(10);
		}

		for (thread& thread : threads)
		{
			thread.join();
		}

		sort();

		for (int i=0;i<PARENTS_POPULATION_SIZE;i++)
		{
			parents[i]->paste(children[gradient_descent[i] - 1].network->export_network(), 1);
		}
	}

	void generate_parents()
	{
		for (int i = 0; i < PARENTS_POPULATION_SIZE; i++)
		{
			Network<T, min, max, activator>* network = current_network->copy();
			network->evolve();
			if (parents.size() < PARENTS_POPULATION_SIZE)
			{
				parents.push_back(network);
			}
			else
			{
				parents[i]->paste(network->export_network(), 1);
				delete network;
			}
		}
	}

	void train(int attempts)
	{
		if (parents.size() < PARENTS_POPULATION_SIZE)
		{
			generate_parents();
		}
	
		for (int i = 0; i < attempts; i++)
		{
			attempt(i);
			cout << i << "/" << attempts << endl;
		}
		current_network = parents[0]->copy();
	}

	void try_random()
	{
		DataSet example = dataset->generator();

		int k = 0;
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (example.data[k])
				{
					cout << "#";
				}
				else
				{
					cout << "O";
				}
				k++;
			}
			cout << endl;
		}
		cout << example.correct_result<<endl;

		
		vector<T> & final = current_network->calculate(example.data);
		cout << dataset->interpret(final) << endl;
		vector<T> data = current_network->unefficient_layer_screening(1, example.data);
		k = 0;
		for (int i = 0; i < data.size(); i++)
		{
			cout << data[i] << " ";
		}
		cout << endl;
		for (int i = 0; i < sqrt(data.size()); i++)
		{
			for (int j = 0; j < sqrt(data.size()); j++)
			{
				if (data[k])
				{
					cout << "#";
				}
				else
				{
					cout << "O";
				}
				k++;
			}
			cout << endl;
		}
	}

	Trainer(Network<T, min, max, activator>* network_to_train)
	{
		current_network = network_to_train;
		dataset = new D;
	}
	
	void kill_parents()
	{
		for (int i = 0; i < parents.size(); i++)
		{
			delete parents[i];
		}
		parents.clear();
		cout << "All parents have been killed. Killing the children is not necesarry - this process is automatized" << endl;
	}

	~Trainer()
	{
		for (int i = 0; i < parents.size(); i++)
		{
			delete parents[i];
		}
	}

};