#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <cmath>
#include <string>
#include <set>
#include <algorithm>

using namespace std;

double computeEntropy(double positive, double negative) {
    double total = positive + negative;
    if (total == 0) return 0;
    double prob_positive = positive / total;
    double prob_negative = negative / total;

    double entropyValue = 0;
    if (prob_positive > 0) entropyValue -= prob_positive * log2(prob_positive);
    if (prob_negative > 0) entropyValue -= prob_negative * log2(prob_negative);

    return entropyValue;
}

double computeGini(double positive, double negative) {
    double total = positive + negative;
    if (total == 0) return 0;
    double prob_positive = positive / total;
    double prob_negative = negative / total;

    double giniValue = 1 - (prob_positive * prob_positive + prob_negative * prob_negative);
    return giniValue;
}

void loadCSV(const string& filename, vector<vector<string>>& records) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string value;
        vector<string> row;
        while (getline(ss, value, ',')) {
            row.push_back(value);
        }
        records.push_back(row);
    }
    file.close();
}

void saveCSV(const string& filename, const vector<string>& logEntries) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file for writing: " << filename << endl;
        return;
    }

    for (const string& entry : logEntries) {
        file << entry << "\n";
    }

    file.close();
}

bool isStringNumeric(const string& str) {
    for (char const &c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}

void analyzeNumerical(vector<vector<string>>& records, int featureIndex, int targetIndex, const map<string, double>& targetDistribution, vector<string>& logEntries, double initialEntropy, int totalCount, double& optimalGini) {
    set<double> distinctValues;
    for (int i = 1; i < records.size(); i++) {
        distinctValues.insert(stod(records[i][featureIndex]));
    }

    double maxGain = -1;
    string optimalSplit;
    for (double splitValue : distinctValues) {
        double leftPositive = 0, leftNegative = 0, rightPositive = 0, rightNegative = 0;

        for (int i = 1; i < records.size(); i++) {
            double featureValue = stod(records[i][featureIndex]);
            string targetValue = records[i][targetIndex];

            if (featureValue <= splitValue) {
                if (targetValue == "Yes") leftPositive++;
                else leftNegative++;
            } else {
                if (targetValue == "Yes") rightPositive++;
                else rightNegative++;
            }
        }

        double leftTotal = leftPositive + leftNegative;
        double rightTotal = rightPositive + rightNegative;
        double weightedEntropy = ((leftTotal / totalCount) * computeEntropy(leftPositive, leftNegative)) +
                                 ((rightTotal / totalCount) * computeEntropy(rightPositive, rightNegative));
        double weightedGini = ((leftTotal / totalCount) * computeGini(leftPositive, leftNegative)) +
                              ((rightTotal / totalCount) * computeGini(rightPositive, rightNegative));

        double informationGain = initialEntropy - weightedEntropy;

        string logEntry = "Split Value: " + to_string(splitValue) + ", Weighted Entropy: " + to_string(weightedEntropy) +
                          ", Gini Index: " + to_string(weightedGini) + ", Information Gain: " + to_string(informationGain);
        logEntries.push_back(logEntry);

        if (informationGain > maxGain) {
            maxGain = informationGain;
            optimalSplit = to_string(splitValue);
            optimalGini = weightedGini;
        }
    }

    logEntries.push_back("Optimal Split Value: " + optimalSplit + " with Information Gain: " + to_string(maxGain));
}

void analyzeCategorical(vector<vector<string>>& records, int featureIndex, int targetIndex, const map<string, double>& targetDistribution, vector<string>& logEntries, double initialEntropy, int totalCount, double& optimalGini) {
    map<string, map<string, double>> featureDistribution;

    for (int i = 1; i < records.size(); i++) {
        string featureValue = records[i][featureIndex];
        string targetValue = records[i][targetIndex];

        featureDistribution[featureValue][targetValue]++;
    }

    double weightedEntropy = 0;
    double weightedGiniValue = 0;
    for (const auto& feature : featureDistribution) {
        double featureTotal = 0;
        double featureEntropy = 0;
        double featureGini = 0;

        for (const auto& target : feature.second) {
            featureTotal += target.second;
        }

        for (const auto& target : feature.second) {
            double p = target.second / featureTotal;
            if (p > 0) featureEntropy -= p * log2(p);
        }

        for (const auto& target : feature.second) {
            double p = target.second / featureTotal;
            featureGini += p * p;
        }
        featureGini = 1 - featureGini;

        weightedEntropy += (featureTotal / totalCount) * featureEntropy;
        weightedGiniValue += (featureTotal / totalCount) * featureGini;

        logEntries.push_back("Feature: " + feature.first + " | Weighted Entropy: " + to_string(featureEntropy) + " | Gini Index: " + to_string(featureGini));
    }

    double informationGain = initialEntropy - weightedEntropy;
    optimalGini = weightedGiniValue;
    logEntries.push_back("Information Gain for Selected Feature: " + to_string(informationGain));
}

int main() {
    vector<vector<string>> records;

    string inputFile = "input2.csv";  
    loadCSV(inputFile, records);

    int targetIndex, featureIndex;
    cout << "Enter the index of the target column (0-indexed): ";
    cin >> targetIndex;
    cout << "Enter the index of the feature column (0-indexed): ";
    cin >> featureIndex;

    if (records.size() < 2 || targetIndex >= records[0].size() || featureIndex >= records[0].size()) {
        cerr << "Invalid input or no data found in the CSV file." << endl;
        return 1;
    }

    map<string, double> targetDistribution;
    int totalCount = records.size() - 1;

    for (int i = 1; i < records.size(); i++) {
        targetDistribution[records[i][targetIndex]]++;
    }

    double initialEntropy = 0;
    for (const auto& entry : targetDistribution) {
        double count = entry.second;
        double p = count / totalCount;
        if (p > 0) initialEntropy -= p * log2(p);
    }

    vector<string> logEntries;
    logEntries.push_back("Initial Entropy: " + to_string(initialEntropy));

    double optimalGini = 0;

    if (isStringNumeric(records[1][featureIndex])) {
        logEntries.push_back("Numerical Feature Selected");
        analyzeNumerical(records, featureIndex, targetIndex, targetDistribution, logEntries, initialEntropy, totalCount, optimalGini);
    } else {
        logEntries.push_back("Categorical Feature Selected");
        analyzeCategorical(records, featureIndex, targetIndex, targetDistribution, logEntries, initialEntropy, totalCount, optimalGini);
    }

    logEntries.push_back("Final Gini Index after Optimal Split: " + to_string(optimalGini));

    string outputFile = "output.csv";
    saveCSV(outputFile, logEntries);

    cout << "Results written to " << outputFile << endl;

    return 0;
}
