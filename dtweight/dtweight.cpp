#include <bits/stdc++.h>
#include <fstream>
using namespace std;

int main() {
    fstream inputFile("input.csv", ios::in);
    if (!inputFile.is_open()) {
        cout << "Error: Unable to open the file.";
        return 0;
    }

    string lineContent, feature, group, countString;
    int value;
    map<string, map<string, int>> matrix;
    map<string, int> totalByColumn, totalByRow;

    int rowIndex = 0;
    while (getline(inputFile, lineContent)) {
        stringstream rowStream(lineContent);
        if (rowIndex == 0) {
            rowIndex++;
            continue;
        }

        getline(rowStream, group, ',');
        getline(rowStream, feature, ',');
        getline(rowStream, countString, ',');
        value = stoi(countString);

        matrix[group][feature] = value;
        totalByColumn[feature] += value;
        totalByRow[group] += value;
    }
    inputFile.close();

    int columnSumTotal = 0, rowSumTotal = 0;
    for (auto columnEntry : totalByColumn) {
        columnSumTotal += columnEntry.second;
    }

    for (auto rowEntry : totalByRow) {
        rowSumTotal += rowEntry.second;
    }

    ofstream outputFile("output.csv", ios::out);
    outputFile << "Feature\\Group , ,Programmer ,,  ,Non programmer ,,  ,Total,,, " << endl;
    outputFile << " ,Count, Row %, Column %, Count, Row %, Column %, Count, Row %, Column % " << endl;

    for (auto rowEntry : totalByRow) {
        group = rowEntry.first;
        outputFile << group << ",";

        for (auto columnEntry : totalByColumn) {
            feature = columnEntry.first;
            outputFile << matrix[group][feature] << ",";
            outputFile << ((float)matrix[group][feature] / totalByRow[group]) * 100 << "%,";
            outputFile << ((float)matrix[group][feature] / totalByColumn[feature]) * 100 << "%,";
        }
        outputFile << totalByRow[group] << "," 
                   << ((float)totalByRow[group] / totalByRow[group]) * 100 << "%," 
                   << ((float)totalByRow[group] / columnSumTotal) * 100 << "%" << endl;
    }

    outputFile << "Total ,";
    for (auto columnEntry : totalByColumn) {
        feature = columnEntry.first;
        outputFile << totalByColumn[feature] << ",";
        outputFile << ((float)totalByColumn[feature] / columnSumTotal) * 100 << "%,";
        outputFile << "100%,";
    }
    outputFile << columnSumTotal << ",100%,100%" << endl;
    outputFile.close();
    cout << "Output saved in output.csv";
    return 0;
}
