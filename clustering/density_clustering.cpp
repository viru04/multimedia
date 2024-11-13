#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>

struct Point {
    double value;
    int clusterID; 
    bool isCore;
    bool isBoundary;
    bool isNoise;
};

double distance(double p1, double p2) {
    return std::abs(p1 - p2);  
}

std::vector<Point> readCSV(const std::string& filename) {
    std::vector<Point> points;
    std::ifstream file(filename);
    std::string line;
    
    while (std::getline(file, line)) {
        Point p;
        p.value = std::stod(line);
        p.clusterID = 0;  
        p.isCore = false;
        p.isBoundary = false;
        p.isNoise = false;
        points.push_back(p);
    }
    
    file.close();
    return points;
}

void writeCSV(const std::string& filename, const std::vector<Point>& points, int numClusters) {
    std::ofstream file(filename);
    
    file << "Value,ClusterID,Type\n";
    for (const auto& point : points) {
        std::string type = point.isCore ? "Core" : (point.isBoundary ? "Boundary" : "Noise");
        file << point.value << "," << point.clusterID << "," << type << "\n";
    }
    file << "Total Clusters: " << numClusters << "\n";
    file.close();
}

std::vector<int> regionQuery(const std::vector<Point>& points, int pIdx, double eps) {
    std::vector<int> neighbors;
    for (int i = 0; i < points.size(); i++) {
        if (distance(points[pIdx].value, points[i].value) <= eps) {
            neighbors.push_back(i);
        }
    }
    return neighbors;
}

void expandCluster(std::vector<Point>& points, int pIdx, int clusterID, double eps, int minPts) {
    std::vector<int> seeds = regionQuery(points, pIdx, eps);
    
    if (seeds.size() < minPts) {
        points[pIdx].isNoise = true;  // Keep it noise for now
        return;
    }
    
    points[pIdx].clusterID = clusterID;  // Assign the cluster ID
    points[pIdx].isCore = true;  // Mark as core point
    
    for (int i = 0; i < seeds.size(); i++) {
        int idx = seeds[i];
        if (points[idx].clusterID == 0) {  // Check if the point is unvisited
            points[idx].clusterID = clusterID;  // Assign cluster ID
            std::vector<int> newNeighbors = regionQuery(points, idx, eps);
            
            if (newNeighbors.size() >= minPts) {
                points[idx].isCore = true;  // Mark as core point
                seeds.insert(seeds.end(), newNeighbors.begin(), newNeighbors.end());  // Add new neighbors
            } else if (!points[idx].isCore) {  // If not core, mark as boundary
                points[idx].isBoundary = true;
            }
        } else if (points[idx].isNoise) {  // Change noise to boundary if it joins a cluster
            points[idx].isBoundary = true;
            points[idx].isNoise = false;
            points[idx].clusterID = clusterID;
        }
    }
}

void dbscan(std::vector<Point>& points, double eps, int minPts) {
    int clusterID = 0;
    for (int i = 0; i < points.size(); i++) {
        if (points[i].clusterID == 0) {  // Only process unvisited points
            std::vector<int> neighbors = regionQuery(points, i, eps);
            if (neighbors.size() >= minPts) {
                clusterID++;  // New cluster found
                expandCluster(points, i, clusterID, eps, minPts);
            }
        }
    }
}


int main() {
    std::string inputFile = "cluster_input.csv";
    std::string outputFile = "density_output.csv";
    double eps = 5.0;  
    int minPts = 2;    

    
    std::vector<Point> points = readCSV(inputFile);

    
    dbscan(points, eps, minPts);

    
    int numClusters = 0;
    for (const auto& point : points) {
        if (point.isCore && point.clusterID > numClusters) {
            numClusters = point.clusterID;
        }
    }

    
    writeCSV(outputFile, points, numClusters);

    std::cout << "DBSCAN completed. Results saved in " << outputFile << std::endl;
    return 0;
}
