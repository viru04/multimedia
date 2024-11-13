#include <bits/stdc++.h>
#include <fstream>
using namespace std;

int main() {
    
    vector<double> data;
    double tmp;
    double mini, maxi, new_mini, new_maxi;
    double sum = 0, cnt = 0, square_sum = 0, mean, standard_deviation;
    ifstream infile("input.csv");
    if (!infile) {
        cout << "Error opening file, try again.";
        return 1;
    } 
    while (infile >> tmp) {
        data.push_back(tmp);
    }
    infile.close();

    int opt;
    cout << "\nEnter option: \n1.Min-Max Normalization \n2.Z-Score Normalization\nOption: ";
    cin >> opt;
    ofstream out1("output_MinMax.csv", ios::app);
    ofstream out2("output_Zscore.csv", ios::app);
    switch (opt) {
        case 1: 
            if (data.empty()) {
                cout << "No data available.";
                return 1;
            }
            mini = *min_element(data.begin(), data.end());
            maxi = *max_element(data.begin(), data.end());
            cout << "Enter new min: ";
            cin >> new_mini;
            cout << "\nEnter new max: ";
            cin >> new_maxi;

            out1 << "Data,Normalized Data\n";
            for (double value : data) {
                double normalized_value = (((value - mini) / (maxi - mini)) * (new_maxi - new_mini)) + new_mini;
                out1 << value << "," << normalized_value << "\n";
            }
            out1.close();
            break;

        case 2:
            if (data.empty()) {
                cout << "No data available.";
                return 1;
            }

            
            cnt = data.size();
            for (double value : data) {
                sum += value;
            }
            mean = sum / cnt; 

            
            for (double value : data) {
                square_sum += (value - mean) * (value - mean);
            }
            standard_deviation = sqrt(square_sum / cnt); 
            cout<<standard_deviation<<" "<<mean;
            out2 << "Data,Normalized Data\n";
            for (double value : data) {
                double normalized_value = (value - mean) / standard_deviation;
                out2 << value << "," << normalized_value << "\n";
            }
            out2.close();
            break;

        default:
            cout << "Wrong Option" << endl;
            break;
    }

    return 0;
}
