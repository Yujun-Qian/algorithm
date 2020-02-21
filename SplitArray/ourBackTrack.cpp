#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class Solution {
private:
    vector<int> res;

    // find out if there are n items in the vector whose sum equals 'sum'
    bool dfs(int start, int n, int sum, const vector<int>& A) {
        //cout << "n =" << n << endl;
        numbers++;

        if (A[start] * n > sum)
            return false;

        if (n == 0) {
            if (sum == 0)
                return true;
            return false;
        }

        for (int i = start; i <= A.size() - n; i++ ) {
            res.push_back(A[i]);

            if (dfs(i + 1, n - 1, sum - A[i], A)) {
                return true;
            }
            res.pop_back();
        }
        
        return false;
    }
    
public:
    static int numbers;
    vector<int> splitArraySameAverage(vector<int>& A) {        
        sort(A.begin(), A.end());
        
        int length = A.size();
        length ^= 1;
        
        int sum = 0;
        for (auto item : A) {
            sum += item;
        }

        //cout << length << endl;
        for (int i = 1; i <= length / 2; i++) {
            if (sum * i % A.size())
                continue;
            
            if (dfs(0, i, sum * i / A.size(), A)) {
                return res;
            }
        }
        
        return res;
    }
};
int Solution::numbers = 0;

int main() {
    vector<int> result;
    //vector<int> A = {72,56,81,54,15,96,80,90,8};
    //vector<int> A = {5,3,11,19,2};
    //vector<int> A = {5};
    //vector<int> A = {6,8,18,3,1};
    //vector<int> A = {4, 4, 4, 4, 4, 4, 5, 4, 4, 4, 4, 4, 4, 5};
    //vector<int> A = {4, 4, 4, 4, 4, 4, 4, 5, 4, 4, 4, 4, 4, 4, 5};
    //vector<int> A = {10,29,13,53,33,48,76,70,5,5};
    //vector<int> A = {17,3,7,12,1};
    vector<int> A = {53,6,3,34,91,82,47,9,70,1};
    //vector<int> A = {2,0,5,6,16,12,15,12,4};
    result = Solution().splitArraySameAverage(A);
    for (auto i : result) {
        cout << i << " ";
    }
    cout << endl;
    cout << Solution::numbers << endl;
    return 0;
}

