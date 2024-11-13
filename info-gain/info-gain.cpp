#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <cmath>
#include <string>

using namespace std;

double calculateEntropy(double posCount, double negCount) {
    double totalCount = posCount + negCount;
    if (totalCount == 0) return 0;
    double pPositive = posCount / totalCount;
    double pNegative = negCount / totalCount;
    double entropy = 0;
    if (pPositive > 0) entropy -= pPositive * log2(pPositive);
    if (pNegative > 0) entropy -= pNegative * log2(pNegative);
    return entropy;
}

void readCSV(const string& fileName, vector<vector<string>>& rows) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << fileName << endl;
        return;
    }
    string row;
    while (getline(file, row)) {
        stringstream ss(row);
        string cell;
        vector<string> currentRow;
        while (getline(ss, cell, ',')) {
            currentRow.push_back(cell);
        }
        rows.push_back(currentRow);
    }
    file.close();
}

void writeCSV(const string& fileName, double parentEntropy, double weightedEntropy, double infoGain) {
    ofstream file(fileName);
    if (!file.is_open()) {
        cerr << "Failed to open file for writing: " << fileName << endl;
        return;
    }
    file << "Parent Entropy," << parentEntropy << "\n";
    file << "Weighted Entropy (Feature)," << weightedEntropy << "\n";
    file << "Information Gain (Feature)," << infoGain << "\n";
    file.close();
}

int main() {
    vector<vector<string>> data;
    string inputFileName = "input.csv";
    readCSV(inputFileName, data);

    int targetColIndex, featureColIndex;
    cout << "Enter target column index (0-indexed): ";
    cin >> targetColIndex;
    cout << "Enter feature column index (0-indexed): ";
    cin >> featureColIndex;

    if (data.size() < 2 || targetColIndex >= data[0].size() || featureColIndex >= data[0].size()) {
        cerr << "Invalid input or no data found." << endl;
        return 1;
    }

    map<string, double> targetCounts;
    int sampleSize = data.size() - 1;
    for (int i = 1; i < data.size(); i++) {
        targetCounts[data[i][targetColIndex]]++;
    }

    double parentEntropy = 0;
    for (const auto& count : targetCounts) {
        double prob = count.second / sampleSize;
        if (prob > 0) parentEntropy -= prob * log2(prob);
    }

    map<string, map<string, double>> featureCounts;
    for (int i = 1; i < data.size(); i++) {
        string featureValue = data[i][featureColIndex];
        string targetValue = data[i][targetColIndex];
        featureCounts[featureValue][targetValue]++;
    }

    double weightedEntropy = 0;
    for (const auto& feature : featureCounts) {
        double featureTotal = 0, featureEntropy = 0;
        for (const auto& target : feature.second) featureTotal += target.second;
        for (const auto& target : feature.second) {
            double prob = target.second / featureTotal;
            if (prob > 0) featureEntropy -= prob * log2(prob);
        }
        weightedEntropy += (featureTotal / sampleSize) * featureEntropy;
    }

    double infoGain = parentEntropy - weightedEntropy;

    string outputFileName = "output.csv";
    writeCSV(outputFileName, parentEntropy, weightedEntropy, infoGain);

    cout << "Parent Entropy: " << parentEntropy << endl;
    cout << "Weighted Entropy: " << weightedEntropy << endl;
    cout << "Information Gain: " << infoGain << endl;
    cout << "Results written to " << outputFileName << endl;

    return 0;
}
