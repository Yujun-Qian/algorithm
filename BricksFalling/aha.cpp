//size_t operator()(const T& item) "const"
//bool operator==(const Coordinate& other) "const"

class Solution {
private:
    size_t _size;
    int* _array;
    int* _number;
    int _row;
    int _column;
public:
    struct Coordinate {
        int x;
        int y;

        Coordinate(int x_, int y_) {
            x = x_;
            y = y_;
        }

        bool operator==(const Coordinate& other) const {
            if (x == other.x && y == other.y)
                return true;

            return false;
        }
    };

    template <class T>
    struct Hash {
        Hash() {}

        size_t operator()(const T& item) const {
            size_t h1 = std::hash<int>{}(item.x);
            size_t h2 = std::hash<int>{}(item.y);

            return h1 ^ (h2 << 1);
        }
    };

private:
    int root(int index) {
        while (_array[index] != index) {
            // path compression
            _array[index] = _array[_array[index]];
            index = _array[index];
        }

        return index;
    }

    /*
    bool connected(int x, int y) {
        int index = x * _column + y;

        if (root(index) == root(_size - 1)) {
            return true;
        }

        return false;
    }
    */

    void connect(int index1, int index2) {
        int root1 = root(index1);
        int root2 = root(index2);

        if (root1 != root2) {
            if (_number[root1] < _number[root2]) {
                _array[root1] = root2;
                _number[root2] += _number[root1];
            } else {
                _array[root2] = root1;
                _number[root1] += _number[root2];
            }
        }
    }

    void preparePercolation(const vector<vector<int>>& grid) {
        _array[_size - 1] = _size - 1;
        _number[_size - 1] = 1;

        for (int i = 0; i < _row; i++) {
            for (int j = 0; j < _column; j++) {
                if (grid[i][j] == 1) {
                    int index = i * _column + j;
                    _array[index] = index;
                    _number[index] = 1;

                    if (i == 0) {
                        // connect the first row to the virtual top
                        connect(index, _size - 1);
                    } else {
                        // connect to top brick
                        if (grid[i - 1][j] == 1)
                            connect(index, index - _column);
                        if (j > 0) {
                            // connect to left brick
                            if (grid[i][j - 1] == 1)
                                connect(index, index - 1);
                        }
                    }
                }
            }
        }

        outputArray();
    }

    void outputArray() {
        /*
        for (int i = 0; i < _size; i++) {
            cout << _array[i] << "  ";
        }
        cout << endl;

        for (int i = 0; i < _size; i++) {
            cout << _number[i] << "  ";
        }
        cout << endl;
        */
    }

    int getNumberOfBricks(vector<vector<int>>& grid, vector<int>& hit, int value) {
        int ret = 0;
        
        if (value == 0)
            return ret;

        int originalSize = _number[root(_size - 1)];
        int i = hit[0];
        int j = hit[1];
        int index = i * _column + j;
        grid[i][j] = 1;
        _array[index] = index;
        _number[index] = 1;

        if (i == 0) {
            // connect the first row to the virtual top
            connect(index, _size - 1);
        } else {
            // connect to top brick
            if (grid[i - 1][j] == 1)
                connect(index, index - _column);
        }
        
        // connect to bottom brick
        if (i + 1 < _row && grid[i + 1][j] == 1)
            connect(index, index + _column);

        // connect to left brick
        if (j - 1 >= 0 && grid[i][j - 1] == 1)
            connect(index, index - 1);

        // connect to right brick
        if (j + 1 < _column && grid[i][j + 1] == 1)
            connect(index, index + 1);
        
        ret = _number[root(_size - 1)] - originalSize;
        
        if (ret >= 1)
            --ret;

        return ret;
    }

public:
    vector<int> hitBricks(vector<vector<int>>& grid, vector<vector<int>>& hits) {
        vector<int> ret(hits.size());
        vector<int> value(hits.size());

        _row = grid.size();
        _column = grid[0].size();

        // plus 1 for the virtual top point
        _size = grid.size() * grid[0].size() + 1;
        _array = new int[_size]();
        _number = new int[_size]();

        // calculate the final remains and store the original value of the hit positions
        int i = 0;
        for (auto hit : hits) {
            value[i] = grid[hit[0]][hit[1]];
            grid[hit[0]][hit[1]] = 0;
            ++i;
        }
        
        preparePercolation(grid);

        for (i = hits.size() - 1; i >= 0; --i) {
            ret[i] = getNumberOfBricks(grid, hits[i], value[i]);
        }

        if (_array != nullptr) {
            delete[] _array;
            delete[] _number;
        }

        return ret;
    }
};