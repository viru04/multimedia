#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <cmath>
#include <string>
#include <algorithm>

using namespace std;

double computeMean(const vector<double>& values) {
    double total = 0;
    for (double value : values) {
        total += value;
    }
    return total / values.size();
}

double computeStdDev(const vector<double>& values, double mean) {
    double sum = 0;
    for (double value : values) {
        sum += pow(value - mean, 2);
    }
    return sqrt(sum / values.size());
}

double gaussianPDF(double x, double mean, double stdDev) {
    return (1 / (sqrt(2 * M_PI) * stdDev)) * exp(-pow(x - mean, 2) / (2 * pow(stdDev, 2)));
}

void loadCSV(const string& filename, vector<vector<string>>& content) {
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
        content.push_back(row);
    }
    file.close();
}

void saveCSV(const string& filename, const vector<string>& output) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file for writing: " << filename << endl;
        return;
    }

    for (const string& line : output) {
        file << line << "\n";
    }
    file.close();
}

void fitNaiveBayes(const vector<vector<string>>& data, int targetIndex,
                   map<string, map<int, map<string, double>>>& catCounts,
                   map<string, map<int, vector<double>>>& numStats,
                   map<string, int>& labelCounts) {

    for (int i = 1; i < data.size(); i++) {
        string targetValue = data[i][targetIndex];
        labelCounts[targetValue]++;

        for (int j = 0; j < data[0].size(); j++) {
            if (j == targetIndex) continue;

            if (isdigit(data[i][j][0])) {
                numStats[targetValue][j].push_back(stod(data[i][j]));
            } else {
                catCounts[targetValue][j][data[i][j]]++;
            }
        }
    }
}

string classifyInstance(const vector<vector<string>>& data, int targetIndex,
                        const map<string, map<int, map<string, double>>>& catCounts,
                        const map<string, map<int, vector<double>>>& numStats,
                        const map<string, int>& labelCounts,
                        const vector<string>& instance, vector<string>& log) {

    int totalCount = data.size() - 1;
    double maxProbability = -1;
    string bestLabel;

    log.push_back("Class,Prior Probability,Feature Probabilities,Total Probability");

    for (const auto& labelEntry : labelCounts) {
        string label = labelEntry.first;
        double priorProb = static_cast<double>(labelEntry.second) / totalCount;
        string logEntry = label + "," + to_string(priorProb);

        for (int j = 0; j < instance.size(); j++) {
            if (j == targetIndex) continue;

            if (isdigit(instance[j][0])) {
                double mean = computeMean(numStats.at(label).at(j));
                double stdDev = computeStdDev(numStats.at(label).at(j), mean);
                double featureProb = gaussianPDF(stod(instance[j]), mean, stdDev);
                priorProb *= featureProb;

                logEntry += "," + to_string(featureProb);
            } else {
                if (catCounts.at(label).at(j).count(instance[j]) > 0) {
                    double featureProb = catCounts.at(label).at(j).at(instance[j]) / labelEntry.second;
                    priorProb *= featureProb;

                    logEntry += "," + to_string(featureProb);
                } else {
                    double featureProb = 1.0 / (labelEntry.second + catCounts.at(label).at(j).size());
                    priorProb *= featureProb;

                    logEntry += "," + to_string(featureProb);
                }
            }
        }

        logEntry += "=" + to_string(priorProb);
        log.push_back(logEntry);

        if (priorProb > maxProbability) {
            maxProbability = priorProb;
            bestLabel = label;
        }
    }

    return bestLabel;
}

int main() {
    vector<vector<string>> data;

    string inputFilename = "input.csv";
    loadCSV(inputFilename, data);

    int targetColumn;
    cout << "Enter the index of the target column (0-indexed): ";
    cin >> targetColumn;

    map<string, map<int, map<string, double>>> categoricalCounts;
    map<string, map<int, vector<double>>> numericalStatistics;
    map<string, int> classCounts;

    fitNaiveBayes(data, targetColumn, categoricalCounts, numericalStatistics, classCounts);

    vector<string> newRecord(data[0].size());
    cout << "Enter the values for the new instance: " << endl;
    for (int i = 0; i < data[0].size(); i++) {
        if (i == targetColumn) continue;
        cout << data[0][i] << ": ";
        cin >> newRecord[i];
    }

    vector<string> logEntries;

    string predictedLabel = classifyInstance(data, targetColumn, categoricalCounts, numericalStatistics, classCounts, newRecord, logEntries);

    logEntries.push_back("Predicted Class: " + predictedLabel);

    string outputFilename = "output.csv";
    saveCSV(outputFilename, logEntries);

    cout << "Predicted Class: " << predictedLabel << endl;
    cout << "Results and intermediate calculations written to " << outputFilename << endl;

    return 0;
}
