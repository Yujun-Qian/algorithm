/* *****************************************************************************
 *  Name:
 *  Date:
 *  Description:
 **************************************************************************** */

import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.MinPQ;
import edu.princeton.cs.algs4.StdOut;

import java.util.Comparator;
import java.util.Iterator;
import java.util.NoSuchElementException;

public class Solver {
    final private int _moves;
    final private boolean _solvable;
    final private Node _node;
    final private Board[] _solution;

    private class Node {
        private final Board _board;
        private final Node _prev;
        private final int _moves;
        private final int _priority;

        public Node(Board board, Node prev) {
            _board = board;
            _prev = prev;
            if (prev != null) {
                _moves = _prev._moves + 1;
            }
            else {
                _moves = 0;
            }
            _priority = _moves + _board.manhattan();
        }
    }

    private class NodeComparator implements Comparator<Node> {
        public int compare(Node node1, Node node2) {
            if (node1._priority < node2._priority) {
                return -1;
            }
            else if (node1._priority == node2._priority) {
                return 0;
            }
            else {
                return 1;
            }
        }
    }

    // find a solution to the initial board (using the A* algorithm)
    public Solver(Board initial) {
        if (initial == null) {
            throw new IllegalArgumentException();
        }

        boolean solvable = false;
        int moves = -1;

        MinPQ<Node> queue = new MinPQ<Node>(new NodeComparator());
        queue.insert(new Node(initial, null));

        MinPQ<Node> queueTwin = new MinPQ<Node>(new NodeComparator());
        queueTwin.insert(new Node(initial.twin(), null));

        while (true) {
            Node node = queue.delMin();
            if (node._board.isGoal()) {
                moves = node._moves;
                solvable = true;
                _node = node;
                break;
            }

            Node nodeTwin = queueTwin.delMin();
            if (nodeTwin._board.isGoal()) {
                _node = null;
                break;
            }

            insert(node, queue);
            insert(nodeTwin, queueTwin);
        }

        _moves = moves;
        _solvable = solvable;
        if (_solvable) {
            _solution = new Board[_moves + 1];
            Node node = _node;
            for (int i = 0; i < _moves + 1; i++) {
                _solution[i] = node._board;
                node = node._prev;
            }
        }
        else {
            _solution = null;
        }
    }

    private class BoardIterator implements Iterator<Board> {
        private int _index;

        public BoardIterator() {
            if (_solvable) {
                _index = _moves;
            }
            else {
                _index = -1;
            }
        }

        public boolean hasNext() {
            return _index >= 0;
        }

        public Board next() {
            if (!hasNext()) {
                throw new NoSuchElementException();
            }

            Board board = _solution[_index];
            _index--;
            return board;
        }
    }

    private class BoardIterable implements Iterable<Board> {
        public Iterator<Board> iterator() {
            return new BoardIterator();
        }
    }

    private final void insert(Node node, MinPQ<Node> queue) {
        Iterator<Board> iter = node._board.neighbors().iterator();
        Node temp = node._prev;
        while (iter.hasNext()) {
            Board board = iter.next();

            // equals() compares a board to a board that is not a neighbor of a neighbor
            // this suggests a bug in the critical optimization
            if (temp != null && temp._board.equals(board)) {
                continue;
            }

            queue.insert(new Node(board, node));
        }
    }

    // is the initial board solvable? (see below)
    public boolean isSolvable() {
        return _solvable;
    }

    // min number of moves to solve initial board
    public int moves() {
        if (isSolvable())
            return _moves;
        else
            return -1;
    }

    // sequence of boards in a shortest solution
    public Iterable<Board> solution() {
        if (!isSolvable()) {
            return null;
        }
        return new BoardIterable();
    }

    // test client (see below)
    public static void main(String[] args) {
        // create initial board from file
        In in = new In(args[0]);
        int n = in.readInt();
        int[][] tiles = new int[n][n];
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                tiles[i][j] = in.readInt();
        Board initial = new Board(tiles);

        // solve the puzzle
        Solver solver = new Solver(initial);

        // print solution to standard output
        if (!solver.isSolvable())
            StdOut.println("No solution possible");
        else {
            StdOut.println("Minimum number of moves = " + solver.moves());
            for (Board board : solver.solution())
                StdOut.println(board);
        }
    }
}
