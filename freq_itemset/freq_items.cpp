#include <bits/stdc++.h>
#include <map>
using namespace std;

ifstream fin;
ofstream fw; 
double minfre;
vector<set<string>> datatable;
set<string> products;
map<string, int> freq;

vector<string> wordsof(string str) {
    vector<string> tmpset;
    string tmp = "";
    int i = 0;
    while (str[i]) {
        if (isalnum(str[i])) {
            tmp += str[i];
        } else {
            if (tmp.size() > 0) {
                tmpset.push_back(tmp);
                tmp = "";
            }
        }
        i++;
    }
    if (tmp.size() > 0) {
        tmpset.push_back(tmp);
    }
    return tmpset;
}

string combine(vector<string> &arr, int miss) {
    string str;
    for (int i = 0; i < arr.size(); i++) {
        if (i != miss)
            str += arr[i] + " ";
    }
    if (!str.empty()) {
        str = str.substr(0, str.size() - 1);
    }
    return str;
}

set<string> cloneit(set<string> &arr) {
    set<string> dup(arr.begin(), arr.end());
    return dup;
}

set<string> apriori_gen(set<string> &sets, int k) {
    set<string> set2;
    for (auto it1 = sets.begin(); it1 != sets.end(); ++it1) {
        auto it2 = it1;
        ++it2;
        for (; it2 != sets.end(); ++it2) {
            vector<string> v1 = wordsof(*it1);
            vector<string> v2 = wordsof(*it2);

            bool alleq = true;
            for (int i = 0; i < k - 1 && alleq; i++) {
                if (v1[i] != v2[i]) {
                    alleq = false;
                }
            }

            if (alleq) {
                v1.push_back(v2[k - 1]);
                if (v1[v1.size() - 1] < v1[v1.size() - 2])
                    swap(v1[v1.size() - 1], v1[v1.size() - 2]);

                bool valid = true;
                for (int i = 0; i < v1.size() && valid; i++) {
                    string tmp = combine(v1, i);
                    if (sets.find(tmp) == sets.end()) {
                        valid = false;
                    }
                }

                if (valid)
                    set2.insert(combine(v1, -1));
            }
        }
    }
    return set2;
}

int main() {
    fin.open("freq_input.csv", ios::in);
    if (!fin.is_open()) {
        perror("Error in opening file : ");
        return -1;
    }

    fw.open("freq_itemset_output.csv", ios::out); 

    cout << "Frequency % :";
    cin >> minfre;
    fw << "Frequency % :" << minfre << endl; 

    string str;
    while (getline(fin, str)) {
        vector<string> arr = wordsof(str);
        set<string> tmpset(arr.begin(), arr.end());
        datatable.push_back(tmpset);

        for (const auto &item : tmpset) {
            products.insert(item);
            freq[item]++;
        }
    }
    fin.close();

    cout << "No of transactions: " << datatable.size() << endl;
    fw << "No of transactions: " << datatable.size() << endl; 

    minfre = minfre * datatable.size() / 100;
    cout << "Min frequency: " << minfre << endl;
    fw << "Min frequency: " << minfre << endl; 

    queue<set<string>::iterator> q;
    for (auto it = products.begin(); it != products.end(); ++it) {
        if (freq[*it] < minfre) {
            q.push(it);
        }
    }
    while (!q.empty()) {
        products.erase(*q.front());
        q.pop();
    }

    int pass = 1;
    cout << "\nFrequent " << pass++ << "-item set: \n";
    fw << "\nFrequent " << pass - 1 << "-item set: \n"; 
    for (const auto &it : products) {
        cout << "{" << it << "} " << freq[it] << endl;
        fw << "{" << it << "} " << freq[it] << endl; 
    }

    int i = 2;
    set<string> prev = cloneit(products);
    while (true) {
        set<string> cur = apriori_gen(prev, i - 1);
        if (cur.size() < 1)
            break;

        for (auto it = cur.begin(); it != cur.end();) {
            vector<string> arr = wordsof(*it);
            int tot = 0;

            for (int j = 0; j < datatable.size(); j++) {
                bool pres = true;
                for (const auto &elem : arr) {
                    if (datatable[j].find(elem) == datatable[j].end()) {
                        pres = false;
                        break;
                    }
                }
                if (pres) {
                    tot++;
                }
            }

            if (tot >= minfre) {
                freq[*it] += tot;
                ++it;
            } else {
                q.push(it);
                ++it;
            }
        }

        while (!q.empty()) {
            cur.erase(*q.front());
            q.pop();
        }

        if (cur.empty())
            break;

        cout << "\nFrequent " << pass++ << "-item set: \n";
        fw << "\nFrequent " << pass - 1 << "-item set: \n"; 
        for (const auto &it : cur) {
            cout << "{" << it << "} " << freq[it] << endl;
            fw << "{" << it << "} " << freq[it] << endl; 
        }

        prev = cloneit(cur);
        i++;
    }

    // association rules
    // cout << "\nAssociation Rules:\n";
    // fw << "\nAssociation Rules:\n"; 
    // for (const auto &itemset : freq) {
    //     string item = itemset.first;
    //     vector<string> items = wordsof(item);
    //     int item_count = freq[item];
        
    //     for (int j = 0; j < items.size(); j++) {
    //         string lhs = combine(items, j); // Left-hand side
    //         string rhs = items[j]; // Right-hand side
    //         int lhs_count = freq[lhs]; // Support for LHS

    //         if (lhs_count > 0) {
    //             double support = static_cast<double>(item_count) / datatable.size() * 100; // Support percentage
    //             double confidence = static_cast<double>(item_count) / lhs_count * 100; // Confidence percentage
                
    //             cout << lhs << " => " << rhs << " (Support: " << support << "%, Confidence: " << confidence << "%)\n";
    //             fw << lhs << " => " << rhs << " (Support: " << support << "%, Confidence: " << confidence << "%)\n"; 
    //         }
    //     }
    // }

    fw.close(); 
    return 0;
}
