#pragma once

template<typename T, T min, T max, Activator <T> activator>
class Network
{
private:

	int total_node_number = 0;

	class Node
	{
	private:
		int output_nodes;
		vector<T>output_scalars;
		T input_value = 0;
		vector<T> output_values;
		

	public:
		void paste(vector<T>& data, int chance)
		{
			for (int i = 0; i < output_nodes; i++)
			{
				if(!(rand()%chance))
				output_scalars[i] = data[i];
			}
		}

		vector<T> copy()
		{
			return output_scalars;
		}

		void evolve()
		{
			for (int i = 0; i < output_nodes;i++)
			{
				int chance = rand() % LINK_EVOLUTION_CHANCE;
				if (!chance)
				{
					T new_value;
					random_device rd;
					mt19937 gen(rd());
					uniform_real_distribution<> dist(min, max);
					new_value = (T)dist(gen);
					new_value += min;
					output_scalars[i] += new_value;
				}
			}
			int chance = rand() % BIAS_EVOLUTION_CHANCE;
			if (!chance)
			{
				T new_value;
				random_device rd;
				mt19937 gen(rd());
				uniform_real_distribution<> dist(min, max);
				new_value = (T)dist(gen);
				new_value += min;
				output_scalars[output_nodes] += new_value;
			}

		}

		void take_input(T input)
		{
			input_value += input;
		}

		vector<T>& propagate()
		{
			input_value = activator(input_value + output_scalars[output_nodes]);
			for (int i = 0; i < output_nodes; i++)
			{
				output_values[i] = input_value * output_scalars[i];
			}
			input_value = 0;
			return output_values;
		}

		T get_output()
		{
			T result = input_value;
			input_value = 0;
			return activator(result + output_scalars[output_nodes]);
		}

		void display()
		{
			for (int i = 0; i < output_nodes; i++)
			{
				cout << output_scalars[output_nodes] << " " << output_scalars.at(i) << endl;
			}
		}

		Node(int output_nodes)
		{
			this->output_nodes = output_nodes;
			output_scalars.reserve(output_nodes + 1);
			for (int i = 0; i < output_nodes; i++)
			{
				output_scalars.push_back(INITIAL_NETWORK_LINKS_VALUE);
				output_values.push_back(0);
			}
			output_scalars.push_back(INITIAL_NETWORK_BIAS_VALUE);
			//LAST SCALAR IS THE BIAS
		}
	};

	class Layer
	{
	private:
		int size;
		int output_layer_size;
		vector<Node>nodes;
		vector<T>output;

	public:
		void paste(vector<vector<T>>& data, int chance)
		{
			for (int i = 0; i < size; i++)
			{
				nodes.at(i).paste(data[i], chance);
			}
		}

		vector<vector<T>> copy()
		{
			vector<vector<T>> data_to_copy;
			data_to_copy.reserve(size);
			for (int i = 0; i < size; i++)
			{
				data_to_copy.push_back(nodes[i].copy());
			}
			return data_to_copy;
		}

		void evolve(int node)
		{
			nodes[node].evolve();
		}

		void take_input(const vector<T>& input)
		{
			for (int i = 0; i < size; i++)
			{
				nodes[i].take_input(input[i]);
			}

		}

		vector<T>& propagate()
		{

			for (int i = 0; i < output_layer_size; i++)
			{
				output[i] = 0;
			}
			for (int i = 0; i < size; i++)
			{
				vector<T>& output_of_this_node = nodes[i].propagate();
				for (int j = 0; j < output_layer_size; j++)
				{
					output[j] += output_of_this_node[j];
				}
			}
			return output;
		}

		vector<T>& get_input()
		{
			for (int i = 0; i < size; i++)
			{
				output[i] = nodes[i].get_output();
			}
			return output;
		}

		void display()
		{
			for (int i = 0; i < size; i++)
			{
				nodes[i].display();
			}
		}

		vector<T> unefficient_screening()
		{
			vector<T>data;
			for (int i = 0; i < size; i++)
			{
				data.push_back(nodes[i].get_output());
			}
			return data;
		}

		Layer(int size, int output_layer_size)
		{
			this->size = size;
			this->output_layer_size = output_layer_size;
			nodes.reserve(size);
			for (int i = 0; i < size; i++)
			{
				nodes.emplace_back(output_layer_size);
			}
			for (int i = 0; i < output_layer_size; i++)
			{
				output.push_back(0);
			}
		}
	};

	vector<Layer>layers;
	int deepness;
	vector<int>layer_sizes;
public:

	vector<vector<vector<T>>> export_network()
	{
		vector<vector<vector<T>>> data;
		data.reserve(deepness);
		for (int i = 0; i < deepness; i++)
		{
			data.push_back(layers[i].copy());
		}
		return data;
	}


	void evolve()
	{
		for (int j = 0; j < GENERAL_EVOLUTION_SPEED_FACTOR; j++)
		{
			
			int i = rand() % total_node_number;
			int k = -1;
			while (i >= 0)
			{
				k+=1;
				i -= layer_sizes[k];
			}
			i = i * (-1);
			i = i - 1;
			
			
			layers[k].evolve(i);
			
			
			/* for (int i = 0; i < deepness; i++)
			{
				int node = rand() % layer_sizes[i];
				layers[i].evolve(node);
			} */

		}
	}

	void paste(vector<vector<vector<T>>> data, int chance)
	{
	
			for (int i = 0; i < deepness; i++)
			{
				layers[i].paste(data[i], chance);
			}
	}

	vector<T> unefficient_layer_screening(int layer, vector<T>& data)
	{
		layers.at(0).take_input(data);
		for (int i = 0; i < layer; i++)
		{
			layers.at(i + 1).take_input(layers[i].propagate());
		}
		return layers[layer].unefficient_screening();
		layers.at(0).take_input(data);
		for (int i = layer; i < deepness - 1; i++)
		{
			layers.at(i + 1).take_input(layers[i].propagate());
		}
		layers[deepness - 1].propagate();
	}

	void save(string name)
	{
		name += NETWORK_FILENAME;
		ofstream file(name);
		vector<vector<vector<T>>> data = export_network();
		//PRINT INFO ABOUT THE NETWORK
		file << deepness << endl;
		for (int i = 0; i < layer_sizes.size(); i++)
		{
			file << layer_sizes[i] << endl;
		}
		file << endl;
		for (int i = 0; i < deepness; i++)
		{
			//PRINT LAYER NUMBER 
			file << i << endl;
			for (int j = 0; j < layer_sizes[i]; j++)
			{
				//PRINT NODE NUMBER 
				file << j << " ";
				file << data[i][j].size() << " " << endl;
				for (int k = 0; k < data[i][j].size(); k++)
				{
					file << data[i][j][k] << " ";
				}
				file << endl;
			}
			file << endl;
		}
		cout << "Save complete" << endl;
	}

	void load(string name)
	{
		name += NETWORK_FILENAME;
		ifstream file(name);
		vector<vector<vector<T>>> data = export_network();
		int deepness_from_file;
		vector<int> layer_sizes_from_file;
		file >> deepness_from_file;
		for (int i = 0; i < deepness_from_file; i++)
		{
			int size;
			file >> size;
			layer_sizes_from_file.push_back(size);
		}
		bool ok = true;
		if (deepness != deepness)
		{
			ok = false;
		}
		else
		{
			for (int i = 0; i < deepness; i++)
			{
				if (layer_sizes[i] != layer_sizes_from_file[i])
				{
					ok = false;
				}
			}
		}
		if (!ok)
		{
			cout << "Load failed! Networks incompatible. Check deepness and width of the networks" << endl;
			return;
		}

		for (int i = 0; i < deepness; i++)
		{
			int layer_number;
			file >> layer_number;
			vector<vector<T>> layer_data;
			for (int j = 0; j < layer_sizes[layer_number]; j++)
			{
				int node_number;
				file >> node_number;
				int node_data_size;
				file >> node_data_size;
				vector<T> node_data;
				for (int k = 0; k < node_data_size; k++)
				{
					T data_piece;
					file >> data_piece;
					node_data.push_back(data_piece);
				}
				layer_data.push_back(node_data);
			}
			data[layer_number] = layer_data;
		}
		paste(data, 1);
		cout << "Load completed" << endl;
	}

	Network* copy()
	{
		vector<vector<vector<T>>> data_to_copy = export_network();
		Network<T, min, max, activator>* target = new Network<T, min, max, activator>(layer_sizes);
		target->paste(data_to_copy, 1);
		return target;
	}

	vector<T>& calculate(vector<T>& data)
	{
		layers.at(0).take_input(data);
		for (int i = 0; i < deepness - 1; i++)
		{
			layers.at(i + 1).take_input(layers[i].propagate());
		}
		return layers[deepness - 1].propagate();
	}

	void display()
	{
		for (int i = 0; i < deepness; i++)
		{
			layers[i].display();
		}
	}

	Network(vector<int>& layer_sizes)
	{
		deepness = layer_sizes.size();
		layers.reserve(deepness);
		for (int i = 0; i < deepness; i++)
		{
			int next_layer_size = 0;
			if (i < deepness - 1)
			{
				next_layer_size = layer_sizes[i + 1];
			}
			else
			{
				next_layer_size = layer_sizes[i];
			}
			layers.emplace_back(layer_sizes[i], next_layer_size);
			next_layer_size = layer_sizes[i];
		}
		for (int i = 0; i < deepness; i++)
		{
			this->layer_sizes.push_back(layer_sizes[i]);
			total_node_number += layer_sizes[i];
		}
	}
};