#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <string>

using namespace std;

void loadCSV(const string& filename, vector<double>& xData, vector<double>& yData) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string xValue, yValue;
        getline(ss, xValue, ',');
        getline(ss, yValue, ',');

        xData.push_back(stod(xValue));
        yData.push_back(stod(yValue));
    }
    file.close();
}

void computeLinearRegression(const vector<double>& xData, const vector<double>& yData, double& gradient, double& constant) {
    int count = xData.size();
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;

    for (int i = 0; i < count; i++) {
        sumX += xData[i];
        sumY += yData[i];
        sumXY += xData[i] * yData[i];
        sumX2 += xData[i] * xData[i];
    }

    gradient = (count * sumXY - sumX * sumY) / (count * sumX2 - sumX * sumX);
    constant = (sumY - gradient * sumX) / count;
}

void saveCSV(const string& filename, double gradient, double constant, double inputX, double outputY) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file for writing: " << filename << endl;
        return;
    }

    file << "Linear Regression Equation: y = " << gradient << " * x + " << constant << "\n";
    file << "Input x: " << inputX << "\n";
    file << "Predicted y: " << outputY << "\n";
    file.close();
}

int main() {
    vector<double> xData, yData;
    string inputFilename = "input.csv";

    loadCSV(inputFilename, xData, yData);

    if (xData.empty() || yData.empty()) {
        cerr << "No data available or error in file." << endl;
        return 1;
    }

    double gradient, constant;

    computeLinearRegression(xData, yData, gradient, constant);

    cout << "Linear Regression Equation: y = " << gradient << " * x + " << constant << endl;

    double inputX;
    cout << "Enter a new value for x: ";
    cin >> inputX;

    double predictedY = gradient * inputX + constant;

    cout << "Predicted value of y for x = " << inputX << " is: " << predictedY << endl;

    string outputFilename = "output.csv";
    saveCSV(outputFilename, gradient, constant, inputX, predictedY);

    cout << "Results written to " << outputFilename << endl;

    return 0;
}
