#pragma once

class Colors
{
public:



	vector<DataSet>data;

	bool does_contain(string stack, string needle)
	{
		return stack.find(needle) != string::npos;
	}

	DataSet generator()
	{
		int wanted_result = rand() % 2;
		DataSet result;
		while (true)
		{
			int index = rand() % data.size();
			result = data[index];
			if (result.correct_result == wanted_result)
			{
				break;
			}
		}
		return result;
	}

	template<typename T>
	int interpret(vector<T>& result)
	{
		return result[0] > 0;
	}

	Colors()
	{
		ifstream odczyt("colors.csv");
		string color;
		while (getline(odczyt, color) && data.size() < 1000000)
		{
			if (data.size() % 1000 == 0)
			{
				cout << data.size() << endl;
			}
			int spacer_count = 0;
			string r;
			string g;
			string b;
			string result;
			for (int i = 0; i < color.size(); i++)
			{
				if (color[i] == ',')
				{
					spacer_count++;
				}
				else
				{
					if (spacer_count == 1)
					{
						r.push_back(color[i]);
					}
					if (spacer_count == 2)
					{
						g.push_back(color[i]);
					}
					if (spacer_count == 3)
					{
						b.push_back(color[i]);
					}
					if (spacer_count == 4)
					{
						result.push_back(color[i]);
					}
				}
			}
			DataSet example;
			example.data.push_back((float)atoi(&r[0]));
			example.data.push_back((float)atoi(&g[0]));
			example.data.push_back((float)atoi(&b[0]));
			if (does_contain(result, "red"))
			{
				example.correct_result = 1;
			}
			data.push_back(example);
		}
	}
};
