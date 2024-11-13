#include <bits/stdc++.h>
using namespace std;

void readCSV(string fileName, vector<int> &values) {
    ifstream file(fileName);
    string line;
    while (getline(file, line)) {
        stringstream s(line);
        string num;
        while (getline(s, num, ',')) {
            values.push_back(stoi(num));
        }
    }
    file.close();
}

void writeCSV(string fileName, vector<vector<int>> &bins, string method) {
    ofstream file(fileName);
    file << "Binning Method: " << method << endl;
    for (auto &bin : bins) {
        for (size_t i = 0; i < bin.size(); ++i) {
            file << bin[i];
            if (i != bin.size() - 1) file << ",";
        }
        file << endl;
    }
    file.close();
}

void binByMean(vector<int> &values, int binSize, vector<vector<int>> &binnedValues) {
    int length = values.size();
    for (int i = 0; i < length; i += binSize) {
        vector<int> bin;
        int total = 0;
        for (int j = i; j < min(i + binSize, length); ++j) {
            bin.push_back(values[j]);
            total += values[j];
        }
        int mean = total / bin.size();
        for (int &val : bin) val = mean;
        binnedValues.push_back(bin);
    }
}

void binByBoundary(vector<int> &values, int binSize, vector<vector<int>> &binnedValues) {
    int length = values.size();
    for (int i = 0; i < length; i += binSize) {
        vector<int> bin;
        for (int j = i; j < min(i + binSize, length); ++j) bin.push_back(values[j]);
        int minVal = bin.front(), maxVal = bin.back();
        for (int &val : bin) val = (val - minVal < maxVal - val) ? minVal : maxVal;
        binnedValues.push_back(bin);
    }
}

int main() {
    vector<int> data;
    vector<vector<int>> meanBins, boundaryBins;
    int binSize;
    cout << "Enter bin size: ";
    cin >> binSize;
    readCSV("input.csv", data);
    sort(data.begin(), data.end());
    binByMean(data, binSize, meanBins);
    binByBoundary(data, binSize, boundaryBins);
    writeCSV("mean_bins.csv", meanBins, "Mean");
    writeCSV("boundary_bins.csv", boundaryBins, "Boundary");
    cout << "Binning completed and results written to files." << endl;
    return 0;
}
