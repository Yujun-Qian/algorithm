#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class Solution {
private:
    // find out if there are n items in the vector 
    //   whose smallest index >= 'start' and whose sum equals 'sum'
    bool dfs(int start, int n, int sum, const vector<int>& A) {
        //cout << "n =" << n << endl;
        numbers++;
        if (n == 0) {
            cout << "n == 0" << endl;
            return sum == 0;
        }

        if (A[start] * n > sum)
            return false;

        for (int i = start; i <= A.size() - n; i++) {
            if (dfs(i + 1, n - 1, sum - A[i], A)) {
                return true;
            }
        }
        
        return false;
    }
    
public:
    static int numbers;
    bool splitArraySameAverage(vector<int>& A) {        
        sort(A.begin(), A.end());
        
        int length = A.size();
        length ^= 1;
        
        vector<double> B;
        int sum = 0;
        for (auto item : A) {
            sum += item;
            B.push_back((double)item);
        }
        
        if (A.size() == 1)
            return false;

        if (sum % A.size() == 0 && binary_search(A.begin(), A.end(), sum / A.size()))
            return true;
        
        double avg = (double)sum / A.size();
        auto lowerBound = lower_bound(B.begin(), B.end(), avg);
        int index = lowerBound - B.begin();
        if (index == 1 || index == B.size() - 1)
            return false;

        //cout << length << endl;
        for (int i = 1; i <= length / 2; i++) {
            if (sum * i % A.size())
                continue;
            
            if (dfs(0, i, sum * i / A.size(), A))
                return true;
        }
        
        return false;
    }
};

int Solution::numbers = 0;

int main() {
    //vector<int> A = {72,56,81,54,15,96,80,90,8};
    //vector<int> A = {5,3,11,19,2};
    //vector<int> A = {6,8,18,3,1};
    //vector<int> A = {4, 4, 4, 4, 4, 4, 5, 4, 4, 4, 4, 4, 4, 5};
    //vector<int> A = {10,29,13,53,33,48,76,70,5,5};
    //vector<int> A = {17,3,7,12,1};
    vector<int> A = {53,6,3,34,91,82,47,9,70,1};
    cout << Solution().splitArraySameAverage(A) << endl;
    cout << Solution::numbers << endl;
    return 0;
}

