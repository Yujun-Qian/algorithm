/* *****************************************************************************
 *  Name:
 *  Date:
 *  Description:
 **************************************************************************** */

import edu.princeton.cs.algs4.StdRandom;
import edu.princeton.cs.algs4.StdStats;

public class PercolationStats {
    private final double[] results;
    private final int totalNumber;

    public PercolationStats(int n,
                            int trials)    // perform trials independent experiments on an n-by-n grid
    {
        totalNumber = trials;
        int N = n;
        results = new double[trials];

        for (int i = 0; i < totalNumber; i++) {
            int openSites = 0;
            int closeSites = N * N;

            int[] al = new int[closeSites];
            for (int j = 0; j < N * N; j++) {
                al[j] = j;
            }

            Percolation perc = new Percolation(n);

            for (int j = 0; j < N * N; j++) {
                int index = StdRandom.uniform(closeSites);
                closeSites--;

                int k = al[index];
                if (index != closeSites) {
                    al[index] = al[closeSites];
                }

                k++;
                int col = k % N;
                if (col == 0)
                    col = N;
                int row = (k - col) / N + 1;
                perc.open(row, col);

                openSites++;

                if (perc.percolates())
                    break;
            }

            results[i] = openSites / (double) (N * N);
        }
    }

    public double mean()                          // sample mean of percolation threshold
    {
        return StdStats.mean(results);
    }

    public double stddev()                        // sample standard deviation of percolation threshold
    {
        return StdStats.stddev(results);
    }

    public double confidenceLo()                  // low  endpoint of 95% confidence interval
    {
        return mean() - 1.96 * stddev() / Math.sqrt(totalNumber);
    }

    public double confidenceHi()                  // high endpoint of 95% confidence interval
    {
        return mean() + 1.96 * stddev() / Math.sqrt(totalNumber);
    }

    public static void main(String[] args) {
        int n = 0;
        int trials = 0;

        try {
            // Parse the string argument into an integer value.
            n = Integer.parseInt(args[0]);
            trials = Integer.parseInt(args[1]);

            PercolationStats stats = new PercolationStats(n, trials);

            System.out.println("mean = " + stats.mean());
            System.out.println("stddev = " + stats.stddev());
            System.out.println("95% confidence interval = [" + stats.confidenceLo() + ", " + stats
                    .confidenceHi() + "]");
        }
        catch (NumberFormatException nfe) {
            // The first argument isn't a valid integer.  Print
            // an error message, then exit with an error code.
            System.out.println("The arguments must be an integers.");
        }
    }
}
