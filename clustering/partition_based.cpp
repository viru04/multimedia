#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <limits>
#include <string>
#include <algorithm>

using namespace std;

double computeDiff(int a, int b) {
    return abs(a - b);
}

double findCentroid(const vector<int>& cluster) {
    double total = 0;
    for (int point : cluster) {
        total += point;
    }
    return total / cluster.size();
}

void saveDiffMatrix(ofstream& outFile, const vector<int>& points, const vector<double>& centroids, int iteration) {
    outFile << "Iteration " << iteration << " - Diff Matrix\n";
    outFile << "Point";
    for (int i = 0; i < centroids.size(); ++i) {
        outFile << ",Centroid " << i + 1;
    }
    outFile << "\n";

    for (int point : points) {
        outFile << point;
        for (double centroid : centroids) {
            outFile << "," << computeDiff(point, static_cast<int>(centroid));
        }
        outFile << "\n";
    }
    outFile << "\n";
}

void saveClusters(ofstream& outFile, const vector<vector<int>>& groups, int iteration) {
    outFile << "Iteration " << iteration << " - Clusters\n";
    outFile << "Group,Points\n";
    for (size_t i = 0; i < groups.size(); i++) {
        for (int point : groups[i]) {
            outFile << "Group " << i + 1 << "," << point << "\n";
        }
    }
    outFile << "\n";
}

void clusterPoints(vector<int>& points, int numGroups, vector<vector<int>>& groups, ofstream& outFile) {
    
    vector<double> centroids(numGroups);
    int step = points.size() / numGroups;
    for (int i = 0; i < numGroups; ++i) {
        centroids[i] = points[i * step];  
    }

    bool isStable = false;
    int iteration = 1;

    while (!isStable) {
        
        for (auto& group : groups) {
            group.clear();
        }

        
        for (int point : points) {
            double minDiff = numeric_limits<double>::max();
            int nearest = 0;

            for (int i = 0; i < numGroups; i++) {
                double diff = computeDiff(point, centroids[i]);
                if (diff < minDiff) {
                    minDiff = diff;
                    nearest = i;
                }
            }

            groups[nearest].push_back(point);
        }

        saveDiffMatrix(outFile, points, centroids, iteration);
        saveClusters(outFile, groups, iteration);

        
        vector<double> updatedCentroids(numGroups);
        for (int i = 0; i < numGroups; i++) {
            if (!groups[i].empty()) {
                updatedCentroids[i] = findCentroid(groups[i]);  
            } else {
                updatedCentroids[i] = centroids[i];  
            }
        }

        
        isStable = true;
        for (int i = 0; i < numGroups; i++) {
            if (updatedCentroids[i] != centroids[i]) {
                isStable = false;
                break;
            }
        }

        centroids = updatedCentroids;
        iteration++;
    }
}

vector<int> loadDataFromCSV(const string& filename) {
    vector<int> points;
    ifstream file(filename);
    string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            stringstream ss(line);
            string value;
            while (getline(ss, value, ',')) {
                points.push_back(stoi(value));
            }
        }
        file.close();
    } else {
        cerr << "Cannot open file: " << filename << endl;
    }

    return points;
}

int main() {
    string inputFile = "cluster_input.csv";
    string outputFile = "clusters.csv";

    vector<int> points = loadDataFromCSV(inputFile);
    if (points.empty()) {
        cerr << "No points in input file." << endl;
        return 1;
    }

    int numGroups;
    cout << "Enter number of groups (k): ";
    cin >> numGroups;

    if (numGroups <= 0 || numGroups > points.size()) {
        cerr << "Invalid number of groups." << endl;
        return 1;
    }

    vector<vector<int>> groups(numGroups);

    ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        cerr << "Cannot open output file: " << outputFile << endl;
        return 1;
    }

    clusterPoints(points, numGroups, groups, outFile);

    outFile.close();
    cout << "Clustering results and distance matrix saved to " << outputFile << endl;

    return 0;
}
