#include <bits/stdc++.h>
using namespace std;


float distance(float x1, float y1, float x2, float y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}


vector<pair<float, float>> initialize_centroids(const vector<pair<int, int>> &points, int k) {
    vector<pair<float, float>> centroids;
    for (int i = 0; i < k; i++) {
        centroids.push_back({points[i].first, points[i].second}); 
    }
    return centroids;
}



vector<int> assign_clusters(const vector<pair<int, int>> &points, const vector<pair<float, float>> &centroids) {
    vector<int> clusters(points.size());

    for (int i = 0; i < points.size(); i++) {
        float min_dist = numeric_limits<float>::max();
        int cluster_idx = 0;

        for (int j = 0; j < centroids.size(); j++) {
            float dist = distance(points[i].first, points[i].second, centroids[j].first, centroids[j].second);
            if (dist < min_dist) {
                min_dist = dist;
                cluster_idx = j;
            }
        }
        clusters[i] = cluster_idx;
    }

    return clusters;
}


vector<pair<float, float>> recompute_centroids(const vector<pair<int, int>> &points, const vector<int> &clusters, int k) {
    vector<pair<float, float>> new_centroids(k, {0, 0});
    vector<int> count(k, 0);

    for (int i = 0; i < points.size(); i++) {
        int cluster_idx = clusters[i];
        new_centroids[cluster_idx].first += points[i].first;
        new_centroids[cluster_idx].second += points[i].second;
        count[cluster_idx]++;
    }

    for (int i = 0; i < k; i++) {
        if (count[i] > 0) {
            new_centroids[i].first /= count[i];
            new_centroids[i].second /= count[i];
        }
    }

    return new_centroids;
}


bool has_converged(const vector<pair<float, float>> &centroids, const vector<pair<float, float>> &new_centroids, float tolerance) {
    for (int i = 0; i < centroids.size(); i++) {
        if (distance(centroids[i].first, centroids[i].second, new_centroids[i].first, new_centroids[i].second) > tolerance) {
            return false;
        }
    }
    return true;
}

int main() {
    string line, point, x, y;
    vector<pair<int, int>> points;
    int k = 2;  
    cout<<"enter value of k";
    cin>>k;
    

  
    fstream in("2d_input.csv", ios::in);
    if (!in.is_open()) {
        cout << "Couldn't open file" << endl;
        return -1;
    }

    getline(in, line);  
    while (getline(in, line)) {
        stringstream str(line);
        getline(str, point, ',');
        getline(str, x, ',');
        getline(str, y, ',');
        points.push_back({stoi(x), stoi(y)});
    }

    
    vector<pair<float, float>> centroids = initialize_centroids(points, k);

    
    vector<int> clusters;
    vector<pair<float, float>> new_centroids;
    int iterations = 0;
    const float tolerance = 0.001;

    do {
        clusters = assign_clusters(points, centroids);  
        new_centroids = recompute_centroids(points, clusters, k); 

        if (has_converged(centroids, new_centroids, tolerance)) {
            break;
        }

        centroids = new_centroids;
        iterations++;

    } while (true);

   
    ofstream out("cluster_output_2d.csv");

    
    out << "Distance Matrix\n";
    out << " ,";
    for (int i = 0; i < points.size(); i++) {
        out << "p" << i + 1 << ",";
    }
    out << "\n";

    for (int i = 0; i < points.size(); i++) {
        out << "p" << i + 1 << ",";
        for (int j = 0; j < points.size(); j++) {
            if (i == j) {
                out << "0,";
            } else {
                float dist = distance(points[i].first, points[i].second, points[j].first, points[j].second);
                out << dist << ",";
            }
        }
        out << "\n";
    }

    
    out << "\nPoint,Cluster\n";
    for (int i = 0; i < points.size(); i++) {
        out << "p" << i + 1 << "," << clusters[i] + 1 << endl;  // Clusters indexed from 1 for clarity
    }

    
    out << "\nFinal Centroids\n";
    for (int i = 0; i < centroids.size(); i++) {
        out << "Cluster " << i + 1 << ": (" << centroids[i].first << ", " << centroids[i].second << ")" << endl;
    }

    cout << "K-means clustering completed in " << iterations << " iterations." << endl;

    return 0;
}
