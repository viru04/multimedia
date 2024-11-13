#include <bits/stdc++.h>
using namespace std;

float median(vector<float> a)
{
    int size = a.size();
    if (size % 2 == 1)
        return a[size / 2]; 
    else
        return (a[(size / 2) - 1] + a[size / 2]) / 2.0;
}

float quartile1(vector<float> v)
{
    int n = v.size();
    vector<float> first;

    for (int i = 0; i < n / 2; i++)
    {
        first.push_back(v[i]);
    }
    return median(first);
}

float quartile3(vector<float> v)
{
    int n = v.size();
    vector<float> last;
    if (n % 2 == 0)
    {
        for (int i = n / 2; i < n; i++)
        {
            last.push_back(v[i]);
        }
    }
    else
    {
        for (int i = n / 2 + 1; i < n; i++)
        {
            last.push_back(v[i]);
        }
    }

    return median(last);
}

int main()
{
    ifstream in("sample.csv");
    if (!in.is_open())
    {
        cout << "Couldn't open file";
        exit(0);
    }

    ofstream out("boxplot_output.csv");

    int i = 0;
    string line, mark;
    vector<float> arr;
    while (getline(in, line))
    {
        if (i == 0)
        {
            i++;
            continue;
        }
        stringstream str(line);
        getline(str, mark, ',');

        
        try
        {
            if (!mark.empty()) 
            {
                float x = stof(mark); 
                arr.push_back(x); 
            }
        }
        catch (invalid_argument &e)
        {
            cerr << "Invalid number in the CSV: " << mark << endl;
        }
    }

    int n = arr.size();
    if (n == 0)
    {
        cerr << "No valid data found" << endl;
        return 0;
    }

    sort(arr.begin(), arr.end());

    out << "Minimum value: " << "," << arr[0] << "\n";
    out << "Quartile1 value: " << "," << quartile1(arr) << "\n";
    out << "Median value: " << "," << median(arr) << "\n";
    out << "Quartile3 value: " << "," << quartile3(arr) << "\n";
    out << "Maximum value: " << "," << arr[n - 1] << "\n";

    cout << "Minimum value is " << arr[0] << endl;
    cout << "Q1: " << quartile1(arr) << endl;
    cout << "Median: " << median(arr) << endl;
    cout << "Q3: " << quartile3(arr) << endl;
    cout << "Maximum value is " << arr[n - 1] << endl;

    return 0;
}
