/* *****************************************************************************
 *  Name:
 *  Date:
 *  Description:
 **************************************************************************** */

public class Percolation {
    private int[] roots;
    private int[] sizes;
    private int[] status;
    private int opened = 0;
    final private int N;
    final private int size;

    public Percolation(int n)                // create n-by-n grid, with all sites blocked
    {
        N = n;
        size = n * n + 2; // plus two virtual sites
        roots = new int[size];
        sizes = new int[size];
        status = new int[size];

        for (int i = 0; i < size; i++) {
            roots[i] = i;
            sizes[i] = 1;
            status[i] = 0;
        }

        status[0] = 1;
        status[size - 1] = 1;
    }

    private void connect(int a, int b)  // connect two sites
    {
        while (roots[a] != a) {
            roots[a] = roots[roots[a]];
            a = roots[a];
        }

        while (roots[b] != b) {
            roots[b] = roots[roots[b]];
            b = roots[b];
        }

        // already connected
        if (roots[a] == roots[b])
            return;

        if (sizes[a] < sizes[b]) {
            roots[a] = roots[b];
            sizes[b] += sizes[a];
        }
        else {
            roots[b] = roots[a];
            sizes[a] += sizes[b];
        }
    }

    public void open(int row, int col)    // open site (row, col) if it is not open already
    {
        int i = (row - 1) * N + col;
        if (status[i] == 1) {
            return;
        }

        status[i] = 1;
        opened++;

        if (col != 1 && status[i - 1] == 1) {
            connect(i, i - 1);
        }

        if (col != N && status[i + 1] == 1) {
            connect(i, i + 1);
        }

        if (row != 1 && status[i - N] == 1) {
            connect(i, i - N);
        }

        if (row != N && status[i + N] == 1) {
            connect(i, i + N);
        }

        if (row == 1) {
            connect(i, 0);
        }

        /*
        if (row == N) {
            connect(i, size - 1);
        }
        */
    }

    public boolean isOpen(int row, int col)  // is site (row, col) open?
    {
        int i = (row - 1) * N + col;
        return (status[i] == 1);
    }

    private boolean connected(int a, int b)  // is the two sites connected?
    {
        while (roots[a] != a) {
            roots[a] = roots[roots[a]];
            a = roots[a];
        }

        while (roots[b] != b) {
            roots[b] = roots[roots[b]];
            b = roots[b];
        }
        return (roots[a] == roots[b]);
    }

    public boolean isFull(int row, int col)  // is site (row, col) full?
    {
        int i = (row - 1) * N + col;
        return connected(0, i);
    }

    public int numberOfOpenSites()       // number of open sites
    {
        return opened;
    }

    public boolean percolates()              // does the system percolate?
    {
        boolean ret = false;
        for (int i = size - N - 1; i < size - 1; i++) {
            if (connected(0, i)) {
                ret = true;
                break;
            }
        }
        return ret;

        //return connected(0, size - 1);
    }
}
