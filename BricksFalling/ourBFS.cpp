//size_t operator()(const T& item) "const"
//bool operator==(const Coordinate& other) "const"

class Solution {
private:
    size_t _size;
    int* _array;
    int* _depth;
    int* _processed;
    int _row;
    int _column;
    int _count;
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

    template<class T>
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
            _array[index] = _array[_array[index]];
            index = _array[index];
        }

        return index;
    }

    bool connected(int x, int y) {
        int index = x * _column + y;

        if (root(index) == root(_size - 1)) {
            return true;
        }

        return false;
    }

    void connect(int index1, int index2) {
        int root1 = root(index1);
        int root2 = root(index2);

        if (root1 != root2) {
            if (_depth[root1] < _depth[root2]) {
                _array[root1] = root2;
                _depth[root2] += _depth[root1];
            } else {
                _array[root2] = root1;
                _depth[root1] += _depth[root2];
            }
        }
    }

    void preparePercolation(const vector<vector<int>>& grid) {
        _array[_size - 1] = _size - 1;
        _depth[_size - 1] = 1;

        for (int i = 0; i < _row; i++) {
            for (int j = 0; j < _column; j++) {
                int index = i * _column + j;
                _array[index] = index;
                _depth[index] = 1;

                if (grid[i][j] == 1) {
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
            cout << _depth[i] << "  ";
        }
        cout << endl;
        */
    }

    void processCoordinate(int x, int y, vector<int>& bricks, vector<vector<int>>& grid) {
        bricks.push_back(x * _column + y);
        grid[x][y] = 0;
    }

    int bfsBricks(vector<vector<int>>& grid, int i, int j) {
        vector<int> bricks;
        bricks.reserve(_size - 1);
        processCoordinate(i, j, bricks, grid);

        // bfs
        for (int i = 0; i < bricks.size(); i++) {
            int x = bricks[i] / _column;
            int y = bricks[i] % _column;

            // right brick
            if (y + 1 < grid[0].size() && grid[x][y + 1] == 1) {
                processCoordinate(x, y + 1, bricks, grid);
            }

            // left brick
            if (y - 1 >= 0 && grid[x][y - 1] == 1) {
                processCoordinate(x, y - 1, bricks, grid);
            }

            // bottom brick
            if (x + 1 < grid.size() && grid[x + 1][y] == 1) {
                processCoordinate(x + 1, y, bricks, grid);
            }

            // top brick
            if (x - 1 >= 0 && grid[x - 1][y] == 1) {
                processCoordinate(x - 1, y, bricks, grid);
            }
        }

        return bricks.size();
    }

    bool needUpdateArray(vector<vector<int>>& grid, int x, int y) {
        if (y - 1 >= 0 && y + 1 < _column) {
            if (grid[x][y - 1] == 1 && grid[x][y + 1] == 1) {
                return true;
            }
        }

        if (x - 1 >= 0 && x + 1 < _row) {
            if (grid[x - 1][y] == 1 && grid[x + 1][y] == 1) {
                return true;
            }
        }

        if (x == 0 && x + 1 < _row) {
            if (grid[x + 1][y] == 1) {
                return true;
            }
        }

        int surroundOnes = 0;
        if (x - 1 >= 0 && grid[x - 1][y] == 1)
            surroundOnes++; // the top brick;

        if (x + 1 < _row && grid[x + 1][y] == 1)
            surroundOnes++; // the bottom brick;

        if (y - 1 >= 0) {
            if (grid[x][y - 1] == 1)
                surroundOnes++; // the left brick;

            if (x - 1 >= 0 && grid[x - 1][y - 1] == 1)
                surroundOnes++; // the top-left brick;

            if (x + 1 < _row && grid[x + 1][y - 1] == 1)
                surroundOnes++; // the bottom-left brick;
        }

        if (y + 1 < _column) {
            if (grid[x][y + 1] == 1)
                surroundOnes++; // the right brick;

            if (x - 1 >= 0 && grid[x - 1][y + 1] == 1)
                surroundOnes++; // the top-right brick;

            if (x + 1 < _row && grid[x + 1][y + 1] == 1)
                surroundOnes++; // the bottom-right brick;
        }

        if (surroundOnes <= 3)
            return true;

        return false;
    }

    int update(int x, int y, vector<vector<int>>& grid, int* processed, int hitNumber) {
        bool fall = true;
        vector<int> bricks;
        bricks.reserve(_size);
        bricks.push_back(x * _column + y);
        processed[x * _column + y] == hitNumber;

        for (int i = 0; i < bricks.size(); i++) {
            int x = bricks[i] / _column;
            int y = bricks[i] % _column;

            if (x == 0)
                fall = false;

            // right brick
            if (y + 1 < grid[0].size() && grid[x][y + 1] == 1 && _processed[x * _column + y + 1] != hitNumber) {
                bricks.push_back(x * _column + y + 1);
                processed[x * _column + y + 1] == hitNumber;
            }

            // left brick
            if (y - 1 >= 0 && grid[x][y - 1] == 1 && _processed[x * _column + y - 1] != hitNumber) {
                bricks.push_back(x * _column + y - 1);
                processed[x * _column + y - 1] == hitNumber;
            }

            // bottom brick
            if (x + 1 < grid.size() && grid[x + 1][y] == 1 && _processed[(x + 1) * _column + y] != hitNumber) {
                bricks.push_back((x + 1) * _column + y);
                processed[(x + 1) * _column + y] == hitNumber;
            }

            // top brick
            if (x - 1 >= 0 && grid[x - 1][y] == 1 && _processed[(x - 1) * _column + y] != hitNumber) {
                bricks.push_back((x - 1) * _column + y);
                processed[(x - 1) * _column + y] == hitNumber;
            }
        }

        if (!fall) {
            return 0;
        } else {
            for (auto brick : bricks) {
                int x = brick / _column;
                int y = brick % _column;
                int index = brick;
                _array[index] = index;
                _depth[index] = 1;
                grid[x][y] = 0;
            }

            return bricks.size();
        }
    }

    int updatePercolation(vector<vector<int>>& grid, int x, int y, int hitNumber) {
        int ret = 0;

        // right brick
        if (y + 1 < grid[0].size() && grid[x][y + 1] == 1 && _processed[x * _column + y + 1] != hitNumber) {
            ret += update(x, y + 1, grid, _processed, hitNumber);
        }

        // left brick
        if (y - 1 >= 0 && grid[x][y - 1] == 1 && _processed[x * _column + y - 1] != hitNumber) {
            ret += update(x, y - 1, grid, _processed, hitNumber);
        }

        // bottom brick
        if (x + 1 < grid.size() && grid[x + 1][y] == 1 && _processed[(x + 1) * _column + y] != hitNumber) {
            ret += update(x + 1, y, grid, _processed, hitNumber);
        }

        // top brick
        if (x - 1 >= 0 && grid[x - 1][y] == 1 && _processed[(x - 1) * _column + y] != hitNumber) {
            ret += update(x - 1, y, grid, _processed, hitNumber);
        }

        return ret;
    }

    int getNumberOfBricks(vector<vector<int>>& grid, vector<int>& hit, int hitNumber) {
        int ret = 0;

        if (grid[hit[0]][hit[1]] == 0) {
            return 0;
        }

        int x = hit[0];
        int y = hit[1];

        grid[x][y] = 0;
        _count--;

        bool recalculate = true;
        // have to calculate _array for the 1st hit
        if (hitNumber > 0) {
            recalculate = needUpdateArray(grid, x, y);
        }
        if (recalculate) {
            if (hitNumber == 0)
                preparePercolation(grid);
            else {
                double ratio = (double)_count / (double)_size;
                if (_size > 0.6) {
                    preparePercolation(grid);
                } else {
                    int ret = updatePercolation(grid, x, y, hitNumber);
                    _count -= ret;
                    return ret;
                }
            }
        }

        // the right brick
        if (y + 1 < grid[0].size() && grid[x][y + 1] == 1 && !connected(x, y + 1)) {
            ret += bfsBricks(grid, x, y + 1);
        }

        // the left brick
        if (y - 1 >= 0 && grid[x][y - 1] == 1 && !connected(x, y - 1)) {
            ret += bfsBricks(grid, x, y - 1);
        }

        // the bottom brick
        if (x + 1 < grid.size() && grid[x + 1][y] == 1 && !connected(x + 1, y)) {
            ret += bfsBricks(grid, x + 1, y);
        }

        // the top brick
        if (x - 1 >= 0 && grid[x - 1][y] == 1 && !connected(x - 1, y)) {
            ret += bfsBricks(grid, x - 1, y);
        }

        _count -= ret;
        return ret;
    }

public:
    vector<int> hitBricks(vector<vector<int>>& grid, vector<vector<int>>& hits) {
        vector<int> ret;

        _row = grid.size();
        _column = grid[0].size();

        // plus 1 for the virtual top point
        _size = grid.size() * grid[0].size() + 1;
        _array = new int[_size]();
        _depth = new int[_size]();
        _processed = new int[_size]();
        _count = 0;

        for (int i = 0; i < _row; i++) {
            for (int j = 0; j < _column; j++) {
                if (grid[i][j] == 1)
                    _count++;
            }
        }

        for (int i = 0; i < _size; ++i) {
            _processed[i] = -1;
        }

        int hitNumber = 0;
        for (auto hit : hits) {
            ret.push_back(getNumberOfBricks(grid, hit, hitNumber));
            hitNumber++;
        }

        if (_array != nullptr) {
            delete[] _array;
            delete[] _depth;
        }

        if (_processed != nullptr)
            delete[] _processed;

        return ret;
    }
};