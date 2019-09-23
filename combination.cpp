#include <iostream>
#include <vector>
#include <sys/time.h>

using namespace std;

int debug = 0;

// n - numbers of all items (0, 1, 2, ..., n - 1)
// k - numbers of selected items
bool first_combination(vector<int>& a, int n, int k) {
    if (k < 0 || k > n)
        return false;

    a.clear();
    a.resize(k);
    for (int i = 0; i < k; i++) {
        a[i] = i;
    }

    return true;
}

bool next_combination(vector<int>& a, int n, int k) {
    int r = k - 1;

    // find the first element that can be incremented
    while(r >= 0 && a[r] == n - k + r) {
        r--;
    }

    // if no element can be incremented, we are done
    if (r < 0) {
        return false;
    }

    // increment the first element that can be incremented
    a[r]++;
    // reset elements after it to lowest combination possible
    for (int j = r + 1; j < k; j++) {
        a[j] = a[r] + j - r;
    }

    return true;
}

void print(const vector<int>& a) {
    if (debug) {
        return;
    }

    for (int i = 0; i < a.size(); i++) {
        cout << a[i];
    }

    cout << endl;
}

void printCombination(const vector<vector<int>>& a) {
    if (debug) {
        return;
    }

    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < a[i].size(); j++) {
            cout << a[i][j];
        }
        cout << endl;
    }

    cout << endl;
}

// m - start index
// n - end index
// k - numbers of selected items
vector<vector<int>> combination(int m, int n, int k) {
    vector<vector<int>> ret;

    if (k > n - m + 1 || k < 0) {
        return std::move(ret);
    }

    if (k == 1) {
        for (int i = m; i <= n; i++) {
            ret.push_back(vector<int>{i});
        }

        return std::move(ret);
    }

    for (int i = m; i <= n - k + 1; i++) {
        vector<vector<int>> b;
        b = combination(i + 1, n, k - 1);
        for (int j = 0; j < b.size(); j++) {
            b[j].push_back(i);
        }
        ret.insert(ret.end(), b.begin(), b.end());
    }

    return std::move(ret);
}

int main(int argc, char** argv) {
    struct timeval start;
    gettimeofday(&start, NULL);

    int n = atoi(argv[1]);
    debug = atoi(argv[2]);
    vector<int> a;
    for (int k = 0; k <= n; k++) {
        if (first_combination(a, n, k)) {
            print(a);

            while (next_combination(a, n, k)) {
                print(a);
            }
        }
    }

    struct timeval end;
    gettimeofday(&end, NULL);
    cout << "time cost: " << (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000 << endl;


    for (int k = 0; k <= n; k++) {
        vector<vector<int>> a = combination(0, n - 1, k);
        printCombination(a);
    }


    struct timeval end1;
    gettimeofday(&end1, NULL);
    cout << "time cost: " << (end1.tv_sec - end.tv_sec) * 1000 + (end1.tv_usec - end.tv_usec) / 1000 << endl;

    return 0;
}