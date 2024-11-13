#include <bits/stdc++.h>
using namespace std;

float calculateCorrelation(vector<int> xValues, vector<int> yValues, int count) {
    int totalX = 0, totalY = 0, totalXY = 0;
    int totalX2 = 0, totalY2 = 0;

    for (int i = 0; i < count; i++) {
        totalX += xValues[i];
        totalY += yValues[i];
        totalXY += xValues[i] * yValues[i];
        totalX2 += xValues[i] * xValues[i];
        totalY2 += yValues[i] * yValues[i];
    }
    float correlation = (float)(count * totalXY - totalX * totalY) 
                       / sqrt((count * totalX2 - totalX * totalX) * (count * totalY2 - totalY * totalY));
    return correlation;
}

void readCSVandTranspose(string filePath, vector<vector<int>> &dataMatrix, vector<string> &headers) {
    ifstream inputFile(filePath);
    string line;
    vector<vector<int>> tempMatrix;

    if (getline(inputFile, line)) {
        stringstream stream(line);
        string cell;
        while (getline(stream, cell, ',')) {
            headers.push_back(cell);
        }
    }

    while (getline(inputFile, line)) {
        stringstream stream(line);
        string cell;
        vector<int> row;
        while (getline(stream, cell, ',')) {
            if (cell == "Y") {
                row.push_back(1);
            } else if (cell == "N") {
                row.push_back(0);
            } else {
                row.push_back(stoi(cell));
            }
        }
        tempMatrix.push_back(row);
    }
    inputFile.close();

    int rowCount = tempMatrix.size();
    int colCount = tempMatrix[0].size();
    dataMatrix.resize(colCount - 1);

    for (int i = 1; i < colCount; i++) {
        vector<int> columnData;
        for (int j = 0; j < rowCount; j++) {
            columnData.push_back(tempMatrix[j][i]);
        }
        dataMatrix[i - 1] = columnData;
    }
}

void exportToCSV(string filePath, vector<tuple<string, string, float, string>> resultsList) {
    ofstream outputFile(filePath);
    outputFile << "Column 1,Column 2,Correlation Coefficient,Correlation Type" << endl;
    for (auto &result : resultsList) {
        string firstCol = get<0>(result);
        string secondCol = get<1>(result);
        float correlationValue = get<2>(result);
        string correlationKind = get<3>(result);
        outputFile << firstCol << "," << secondCol << "," << correlationValue << "," << correlationKind << endl;
    }
    outputFile.close();
}

int main() {
    vector<vector<int>> transposedData;
    vector<string> columnLabels;

    readCSVandTranspose("input.csv", transposedData, columnLabels);

    int totalColumns = transposedData.size();
    if (totalColumns == 0) {
        cout << "Insufficient data found in input.csv" << endl;
        return 1;
    }

    vector<tuple<string, string, float, string>> correlationResults;

    for (int i = 0; i < totalColumns; i++) {
        for (int j = i + 1; j < totalColumns; j++) {
            float correlation = calculateCorrelation(transposedData[i], transposedData[j], transposedData[i].size());
            string correlationType = (correlation > 0) ? "Positive correlation" : "Negative correlation";
            correlationResults.push_back(make_tuple(columnLabels[i + 1], columnLabels[j + 1], correlation, correlationType));
        }
    }

    exportToCSV("output.csv", correlationResults);

    cout << "Correlation results saved to output.csv" << endl;

    return 0;
}
