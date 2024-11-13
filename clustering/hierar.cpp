#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <limits>
using namespace std;

double singleLinkage(const vector<double> &c1, const vector<double> &c2);
double completeLinkage(const vector<double> &c1, const vector<double> &c2);
double averageLinkage(const vector<double> &c1, const vector<double> &c2);

int main()
{
    string file = "testinput.csv";
    string outputLog = "testoutput.csv";
    vector<double> voltages;
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

    string line;
    getline(inputFile, line); 

    while (getline(inputFile, line))
    {
        stringstream ss(line);
        string field;
        if (getline(ss, field, ','))
        {
            voltages.push_back(stod(field));
        }
    }
    inputFile.close();

    
    vector<vector<double>> clusters;
    for (double v : voltages)
    {
        clusters.push_back({v});
    }

    logFile << "Initial clusters:\n";
    for (const auto &cluster : clusters)
    {
        logFile << "{ ";
        for (double v : cluster)
        {
            logFile << v << " ";
        }
        logFile << "}, ";
    }
    logFile << "\n\n";

    
    while (clusters.size() > 1)
    {
        double minDist = numeric_limits<double>::max();
        int idx1 = -1, idx2 = -1;

        logFile << "Distance Matrix:\n";
        
        for (size_t i = 0; i < clusters.size(); ++i)
        {
            for (size_t j = 0; j < clusters.size(); ++j)
            {
                if (i == j)
                {
                    logFile << "0 ";
                }
                else
                {
                    double dist = 0;
                    switch (choice)
                    {
                    case 1:
                        dist = singleLinkage(clusters[i], clusters[j]);
                        break;
                    case 2:
                        dist = completeLinkage(clusters[i], clusters[j]);
                        break;
                    case 3:
                        dist = averageLinkage(clusters[i], clusters[j]);
                        break;
                    default:
                        cout << "Invalid choice. Exiting." << endl;
                        return 1;
                    }
                    logFile << dist << " ";

                    if (dist < minDist)
                    {
                        minDist = dist;
                        idx1 = i;
                        idx2 = j;
                    }
                }
            }
            logFile << "\n";
        }
        logFile << "\n";

        logFile << "Merging clusters with minimum distance: " << minDist << "\n";

        
        clusters[idx1].insert(clusters[idx1].end(), clusters[idx2].begin(), clusters[idx2].end());
        clusters.erase(clusters.begin() + idx2);

       
        logFile << "Clusters after merge:\n";
        for (const auto &cluster : clusters)
        {
            logFile << "{ ";
            for (double v : cluster)
            {
                logFile << v << " ";
            }
            logFile << "}, ";
        }
        logFile << "\n\n";
    }

  
    logFile << "Final Cluster:\n";
    for (double v : clusters[0])
    {
        logFile << v << " ";
    }
    logFile << endl;

    logFile.close(); 
    return 0;
}

double singleLinkage(const vector<double> &c1, const vector<double> &c2)
{
    double minDist = numeric_limits<double>::max();
    for (double v1 : c1)
    {
        for (double v2 : c2)
        {
            double dist = abs(v1 - v2);
            if (dist < minDist)
            {
                minDist = dist;
            }
        }
    }
    return minDist;
}

double completeLinkage(const vector<double> &c1, const vector<double> &c2)
{
    double maxDist = numeric_limits<double>::min();
    for (double v1 : c1)
    {
        for (double v2 : c2)
        {
            double dist = abs(v1 - v2);
            if (dist > maxDist)
            {
                maxDist = dist;
            }
        }
    }
    return maxDist;
}

double averageLinkage(const vector<double> &c1, const vector<double> &c2)
{
    double totalDist = 0.0;
    int count = 0;
    for (double v1 : c1)
    {
        for (double v2 : c2)
        {
            totalDist += abs(v1 - v2);
            count++;
        }
    }
    return totalDist / count;
}
