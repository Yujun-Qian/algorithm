/* *****************************************************************************
 *  Name:
 *  Date:
 *  Description:
 **************************************************************************** */

import java.util.Arrays;
import java.util.Iterator;
import java.util.NoSuchElementException;

public class Board {
    private final int[][] _tiles;
    private final int[][] _twinTiles;
    private final int _hammingDistance;
    private final int _manhattanDistance;
    private final int _numOfRows;
    private final int _numOfColumns;
    private final int[][][] _neighbours;
    private final int _numOfNeighbours;

    private int[][] deepCopy(int[][] original) {
        int length = 0;
        if (original != null) {
            length = original.length;
        }

        final int[][] result = new int[length][];
        for (int i = 0; i < length; i++) {
            result[i] = Arrays.copyOf(original[i], original[i].length);
        }
        return result;
    }

    // create a board from an n-by-n array of tiles,
    // where tiles[row][col] = tile at (row, col)
    public Board(int[][] tiles) {
        if (tiles == null) {
            throw new IllegalArgumentException();
        }

        int hammingDistance = 0;
        int manhattanDistance = 0;
        _numOfRows = tiles.length;

        if (_numOfRows > 0) {
            _numOfColumns = tiles[0].length;
        }
        else {
            _numOfColumns = 0;
        }

        if (_numOfColumns > 0 && _numOfRows > 0) {
            _tiles = new int[_numOfRows][_numOfColumns];

            for (int i = 0; i < tiles.length; i++) {
                // System.arraycopy(tiles[i], 0, _tiles[i], 0, tiles[i].length);
                for (int j = 0; j < tiles[i].length; j++) {

                    _tiles[i][j] = tiles[i][j];
                }
            }
        }
        else {
            throw new IllegalArgumentException();
        }

        int rowOfHole = 0;
        int columnOfHole = 0;
        for (int i = 0; i < _tiles.length; i++) {
            for (int j = 0; j < _tiles[i].length; j++) {
                if (_tiles[i][j] == 0) {
                    rowOfHole = i;
                    columnOfHole = j;
                    continue;
                }

                if (_tiles[i][j] != i * _numOfColumns + j + 1) {
                    hammingDistance++;

                    int item = _tiles[i][j] - 1;
                    int expectedRow = item / _numOfColumns;
                    int expectedColumn = item % _numOfColumns;

                    manhattanDistance += Math.abs(expectedRow - i) + Math.abs(expectedColumn - j);
                }
            }
        }

        _hammingDistance = hammingDistance;
        _manhattanDistance = manhattanDistance;

        //calculate neighbours
        _neighbours = new int[4][][];
        int numOfNeighbours = 0;
        if (rowOfHole > 0) {
            int[][] tempTiles = deepCopy(tiles);
            exchange(tempTiles, rowOfHole - 1, columnOfHole, rowOfHole, columnOfHole);
            _neighbours[numOfNeighbours] = tempTiles;
            numOfNeighbours++;
        }

        if (rowOfHole < _numOfRows - 1) {
            int[][] tempTiles = deepCopy(tiles);
            exchange(tempTiles, rowOfHole + 1, columnOfHole, rowOfHole, columnOfHole);
            _neighbours[numOfNeighbours] = tempTiles;
            numOfNeighbours++;
        }

        if (columnOfHole > 0) {
            int[][] tempTiles = deepCopy(tiles);
            exchange(tempTiles, rowOfHole, columnOfHole - 1, rowOfHole, columnOfHole);
            _neighbours[numOfNeighbours] = tempTiles;
            numOfNeighbours++;
        }

        if (columnOfHole < _numOfColumns - 1) {
            int[][] tempTiles = deepCopy(tiles);
            exchange(tempTiles, rowOfHole, columnOfHole + 1, rowOfHole, columnOfHole);
            _neighbours[numOfNeighbours] = tempTiles;
            numOfNeighbours++;
        }

        _numOfNeighbours = numOfNeighbours;

        _twinTiles = deepCopy(_tiles);
        int a = 0;
        int b = 0;
        for (int j = 0; j < _tiles[0].length; j++) {
            if (_tiles[0][j] != 0) {
                a = j;
                break;
            }
        }

        for (int j = 0; j < _tiles[1].length; j++) {
            if (_tiles[1][j] != 0) {
                b = j;
                break;
            }
        }

        exchange(_twinTiles, 0, a, 1, b);
    }

    private void exchange(int[][] tiles, int row1, int column1, int row2, int column2) {
        int temp;

        temp = tiles[row1][column1];
        tiles[row1][column1] = tiles[row2][column2];
        tiles[row2][column2] = temp;
    }

    // string representation of this board
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append(_tiles.length).append("\n");

        for (int i = 0; i < _tiles.length; i++) {
            for (int j = 0; j < _tiles[i].length; j++) {
                sb.append(_tiles[i][j]);
                sb.append("\t");
            }
            sb.append("\n");
        }

        return sb.toString();
    }

    // board dimension n
    public int dimension() {
        return _tiles.length;
    }

    // number of tiles out of place
    public int hamming() {
        return _hammingDistance;
    }

    // sum of Manhattan distances between tiles and goal
    public int manhattan() {
        return _manhattanDistance;
    }

    // is this board the goal board?
    public boolean isGoal() {
        return _hammingDistance == 0;
    }

    // does this board equal y?
    public boolean equals(Object y) {
        if (this == y)
            return true;

        if (y == null || getClass() != y.getClass()) {
            return false;
        }

        Board other = (Board) y;

        if (_hammingDistance != other._hammingDistance
                || _manhattanDistance != other._manhattanDistance
                || _numOfRows != other._numOfRows
                || _numOfColumns != other._numOfColumns) {
            return false;
        }

        for (int i = 0; i < _numOfRows; i++) {
            for (int j = 0; j < _numOfColumns; j++) {
                if (_tiles[i][j] != other._tiles[i][j])
                    return false;
            }
        }

        return true;
    }


    private class NeighbourIterator implements Iterator<Board> {
        int _index = 0;

        NeighbourIterator() {
        }

        public boolean hasNext() {
            return (_index < _numOfNeighbours);
        }

        public void remove() {
            // not supported
            throw new UnsupportedOperationException();
        }

        public Board next() {
            if (!hasNext()) {
                throw new NoSuchElementException();
            }

            int[][] tiles = _neighbours[_index];
            _index++;
            return new Board(tiles);
        }
    }

    private class NeighbourIterable implements Iterable<Board> {
        public Iterator<Board> iterator() {
            return new NeighbourIterator();
        }
    }

    // all neighboring boards
    public Iterable<Board> neighbors() {
        return new NeighbourIterable();
    }

    // a board that is obtained by exchanging any pair of tiles
    public Board twin() {
        return new Board(_twinTiles);
    }

    // unit testing (not graded)
    public static void main(String[] args) {
        int[][] tiles = { { 0, 1 }, { 2, 3 } };
        Board a = new Board(tiles);
        System.out.println(a);

        System.out.println(a.twin());

        Iterator<Board> iter = a.neighbors().iterator();

        while (iter.hasNext()) {
            System.out.println(iter.next());
        }

        System.out.println(a.hamming());
        System.out.println(a.manhattan());
    }
}
