#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n = 5, m = 5, q = 3;
    vector<vector<int>> a(n + 1, vector<int>(m + 1, 0));  // 原矩阵
    vector<vector<int>> d(n + 1, vector<int>(m + 1, 0));  // 差分矩阵

    d[1][1] += q;
    d[4][1] -= q;
    d[1][4] -= q;
    d[4][4] += q;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            a[i][j] = d[i][j] + a[i - 1][j] + a[i][j - 1] - a[i - 1][j - 1];
            cout << a[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
