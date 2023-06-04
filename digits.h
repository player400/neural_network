#pragma once

class Digits
{
public:
	vector<DataSet> data;

	template<typename T>
	int interpret(vector<T>& result)
	{
		vector<int>digits = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		int final = digits[0];
		int value = digits[0];
		for (int i = 1; i < 10; i++)
		{
			if (value < result[i])
			{
				value = result[i];
				final = digits[i];
			}
		}
		return final;
	}

	DataSet generator()
	{
		int data_number = rand() % data.size();
		return data[data_number];
	}

	Digits()
	{
		ifstream odczyt("digit_dataset.csv");
		while (data.size() < 42000)
		{
			DataSet data_piece;
			int label;
			odczyt >> label;
			data_piece.correct_result = label;
			int pixels[28][28];
			for (int i = 0; i < 28; i++)
			{
				for (int j = 0; j < 28; j++)
				{
					int value;
					odczyt >> value;
					pixels[i][j] = value;
				}
			}

			for (int i = 0; i < 28; i += 2)
			{
				for (int j = 0; j < 28; j += 2)
				{
					data_piece.data.push_back(pixels[i][j]);
				}
			}
		
			data.push_back(data_piece);
			if (!(data.size() % 1000))
			{
				cout << data.size() << "/" << "42000" << endl;
			}
		}
	}
};