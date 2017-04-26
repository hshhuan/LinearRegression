#include<iostream>
#include <fstream>
#include<stdlib.h>
#include<string>
#include<sstream>
#include<iomanip>
#include<vector>
using namespace std;

#define MAX_FEATURES 385
#define THRESHOLD 0.0001


int m;
double alpha = 0.01;
double theta[MAX_FEATURES] = { 0 };
double temp[MAX_FEATURES] = { 0 };

vector<vector<double> > train;
vector<vector<double> > test;
vector<double> prediction;
vector<double> different;

void getTrainingData() {
	ifstream in("train.csv");
	if (!in.is_open()) {
		cout << "error" << endl;
	}

	cout << "reading train file..." << endl;

	string line;
	// read title from the first line
	getline(in, line);

	// read data
	while (getline(in, line)) {
		istringstream iss(line);
		string number;
		//read id
		getline(iss, number, ',');

		vector<double> item;
		item.push_back(1);
		while (getline(iss, number, ',')) {
			istringstream is(number);
			double value;
			is >> value;
			item.push_back(value);
		}
		train.push_back(item);

		/*
		for (size_t j = 0; j < 384; j++)
		{
		cout << value[j] << ' ';
		}
		cout << setprecision(8) << reference  << setprecision(6) << endl;

		system("pause");

		*/

	}
	in.close();
	m = train.size();
	cout << "train data size is " << m << endl;
	cout << "read train file done!" << endl;
}

double getDifferent(size_t i) {
	double h = 0;
	for (size_t j = 0; j < MAX_FEATURES; j++)
	{
		h += theta[j] * train[i][j];
	}
	double diff = h - train[i].back();

	return diff;
}

double calculateCost() {
	double cost = 0;
	different.clear();
	for (int i = 0; i < m; i++)
	{
		double diff = getDifferent(i);
		different.push_back(diff);
		double variance = diff*diff;
		cost += variance;
	}
	cost = cost / (2 * m);
	cout << "cost = " << cost << endl;
	return cost;

}


double getPartialDerivative(size_t j) {
	double pd = 0;
	for (int i = 0; i < m; i++)
	{
		pd += different[i]* train[i][j];
	}
	return pd / m;
}

void Training() {

	getTrainingData();

	double cost1 = calculateCost();
	double cost2;
	bool flag = true;
	while (flag) {
		for (size_t i = 0; i < MAX_FEATURES; i++)
		{
			cout << "processing theta " << i << endl;
			temp[i] = theta[i] - alpha * getPartialDerivative(i);
			//cout << "alpha * getPartialDerivative(i) = " << alpha * getPartialDerivative(i) << endl;
			//system("pause");
		}
		for (size_t i = 0; i < MAX_FEATURES; i++)
		{
			theta[i] = temp[i];
		}

		cost2 = calculateCost();
		if (cost1 - cost2 < THRESHOLD) {
			flag = false;
		}
		else {
			cost1 = cost2;
		}
		//system("pause");

	}
	cout << endl << endl;
	cout << "Training Completed!" << endl;
}

void getTestingData() {
	ifstream in("test.csv");
	if (!in.is_open()) {
		cout << "error" << endl;
	}

	cout << "reading test file..." << endl;

	string line;
	// read title from the first line
	getline(in, line);

	// read data
	while (getline(in, line)) {
		istringstream iss(line);
		string number;
		//read id
		getline(iss, number, ',');

		vector<double> item;
		item.push_back(1);
		while (getline(iss, number, ',')) {
			istringstream is(number);
			double value;
			is >> value;
			item.push_back(value);
		}
		test.push_back(item);
	}

	in.close();
	cout << "test data size is " << test.size() << endl;
	cout << "read test file done!" << endl;

}
void Testing() {
	getTestingData();
	for (size_t i = 0; i < test.size(); i++)
	{
		double h = 0;
		for (size_t j = 0; j < MAX_FEATURES; j++)
		{
			h += theta[j] * test[i][j];
		}
		prediction.push_back(h);
	}
	cout << endl << endl;
	cout << "Testing Completed!" << endl;
}

void WritePrediction() {
	ofstream out("submission.csv");
	out << "id,reference" << endl;
	for (size_t i = 0; i < prediction.size(); i++)
	{
		out << i << "," << prediction[i] << endl;
	}

	out.close();
	cout << endl << endl;
	cout << "write prediction done!" << endl;
}

int main() {


	Training();
	Testing();
	WritePrediction();

	system("pause");
	return 0;
}
