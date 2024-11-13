#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>
using namespace std;

double singleLinkage(const vector<vector<double>> &matrix, const vector<int> &c1, const vector<int> &c2);
double completeLinkage(const vector<vector<double>> &matrix, const vector<int> &c1, const vector<int> &c2);
double averageLinkage(const vector<vector<double>> &matrix, const vector<int> &c1, const vector<int> &c2);

int main()
{
    string file = "input2.csv";
    string outputLog = "testoutput.csv";
    vector<vector<double>> distanceMatrix;
    ofstream logFile(outputLog); 

    cout << "Choose Linkage Method:\n";
    cout << "1. Single Linkage\n";
    cout << "2. Complete Linkage\n";
    cout << "3. Average Linkage\n";

    int choice;
    cin >> choice;

    ifstream inputFile(file);
    if (!inputFile)
    {
        cerr << "Error opening file." << endl;
        return 1;
    }

    // Read the CSV header line and ignore it
    string line;
    getline(inputFile, line); 

    // Read distance matrix values
    while (getline(inputFile, line))
    {
        vector<double> row;
        stringstream ss(line);
        string field;
        
        while (getline(ss, field, ','))
        {
            row.push_back(stod(field));
        }
        
        distanceMatrix.push_back(row);
    }
    inputFile.close();

    // Initialize each point as its own cluster
    vector<vector<int>> clusters;
    for (size_t i = 0; i < distanceMatrix.size(); ++i)
    {
        clusters.push_back({static_cast<int>(i)});
    }

    logFile << "Initial clusters:\n";
    for (const auto &cluster : clusters)
    {
        logFile << "{ ";
        for (int idx : cluster)
        {
            logFile << idx << " ";
        }
        logFile << "}, ";
    }
    logFile << "\n\n";

    // Agglomerative clustering process
    while (clusters.size() > 1)
    {
        double minDist = numeric_limits<double>::max();
        int idx1 = -1, idx2 = -1;

        logFile << "Distance Matrix:\n";
        
        // Compute distances between clusters
        for (size_t i = 0; i < clusters.size(); ++i)
        {
            for (size_t j = i + 1; j < clusters.size(); ++j)
            {
                double dist = 0;
                switch (choice)
                {
                case 1:
                    dist = singleLinkage(distanceMatrix, clusters[i], clusters[j]);
                    break;
                case 2:
                    dist = completeLinkage(distanceMatrix, clusters[i], clusters[j]);
                    break;
                case 3:
                    dist = averageLinkage(distanceMatrix, clusters[i], clusters[j]);
                    break;
                default:
                    cout << "Invalid choice. Exiting." << endl;
                    return 1;
                }

                logFile << "Distance between cluster " << i << " and " << j << ": " << dist << "\n";

                // Track the minimum distance and clusters to merge
                if (dist < minDist)
                {
                    minDist = dist;
                    idx1 = i;
                    idx2 = j;
                }
            }
        }

        logFile << "Merging clusters with minimum distance: " << minDist << "\n";

        // Merge clusters at idx1 and idx2
        clusters[idx1].insert(clusters[idx1].end(), clusters[idx2].begin(), clusters[idx2].end());
        clusters.erase(clusters.begin() + idx2);

        // Log clusters after merge
        logFile << "Clusters after merge:\n";
        for (const auto &cluster : clusters)
        {
            logFile << "{ ";
            for (int idx : cluster)
            {
                logFile << idx << " ";
            }
            logFile << "}, ";
        }
        logFile << "\n\n";
    }

    // Log final cluster
    logFile << "Final Cluster:\n";
    for (int idx : clusters[0])
    {
        logFile << idx << " ";
    }
    logFile << endl;

    logFile.close(); 
    return 0;
}

// Single Linkage
double singleLinkage(const vector<vector<double>> &matrix, const vector<int> &c1, const vector<int> &c2)
{
    double minDist = numeric_limits<double>::max();
    for (int i : c1)
    {
        for (int j : c2)
        {
            double dist = matrix[i][j];
            if (dist < minDist)
            {
                minDist = dist;
            }
        }
    }
    return minDist;
}

// Complete Linkage
double completeLinkage(const vector<vector<double>> &matrix, const vector<int> &c1, const vector<int> &c2)
{
    double maxDist = numeric_limits<double>::min();
    for (int i : c1)
    {
        for (int j : c2)
        {
            double dist = matrix[i][j];
            if (dist > maxDist)
            {
                maxDist = dist;
            }
        }
    }
    return maxDist;
}

// Average Linkage
double averageLinkage(const vector<vector<double>> &matrix, const vector<int> &c1, const vector<int> &c2)
{
    double totalDist = 0.0;
    int count = 0;
    for (int i : c1)
    {
        for (int j : c2)
        {
            totalDist += matrix[i][j];
            count++;
        }
    }
    return totalDist / count;
}
