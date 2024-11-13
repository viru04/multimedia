class Solution {
public:
void solve(int node, vector<vector<int>>& adj, vector<int>& parent, string& s, unordered_map<char, int>& m) {
    char currentChar = s[node];
    int originalParent = parent[node];
    
    
    if (m.find(currentChar) != m.end()) {
        parent[node] = m[currentChar];
    }
    
    
    int originalValue = (m.count(currentChar) > 0) ? m[currentChar] : -1;
    m[currentChar] = node;  
    
    
    for (int child : adj[node]) {
        solve(child, adj, parent, s, m);
    }
    
    
    if (originalValue == -1) {
        m.erase(currentChar);
    } else {
        m[currentChar] = originalValue;
    }
}


void sizes(int node, vector<vector<int>>& new_vec, vector<int>& ans) {
    ans[node] = 1;  
    for (int child : new_vec[node]) {
        sizes(child, new_vec, ans);
        ans[node] += ans[child];
    }
}

vector<int> findSubtreeSizes(vector<int>& parent, string& s) {
    int n = parent.size();
    vector<vector<int>> adj(n);       
    unordered_map<char, int> m;  
    
    
    for (int i = 1; i < n; ++i) {
        adj[parent[i]].push_back(i);
    }

    
    solve(0, adj, parent, s, m);

   
    vector<vector<int>> new_vec(n);
    for (int i = 1; i < n; ++i) {
        new_vec[parent[i]].push_back(i);
    }

    
    vector<int> ans(n, 0);
    sizes(0, new_vec, ans);
    
    return subtreeSize;
}
    
};