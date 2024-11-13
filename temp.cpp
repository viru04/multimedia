void solve(int input1, int input2, int input3, int input4[]) {
    vector<vector<int>> c(input1/ input2);
    for (int i = 0; i < input1; i += input2) {
        sort(input4 + i, input4 + i + input2);
        for (int j = i; j < i + input2; j++) {
            c[i / input2].push_back(input4[j]);
        }
        c[i / input2].push_back(i / input2 + 1);
    }
    sort(c.begin(), c.end());
    
    input3--;
    cout << c[input3 / input2].back() << '\n';
    cout << c[input3 / input2][input3 % input2] << '\n';
}