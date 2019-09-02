import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

import java.util.Iterator;

public class Permutation {
    public static void main(String[] args) {
        int n;
        if (args.length == 1)
            n = Integer.parseInt(args[0]);
        else {
            throw new java.lang.IllegalArgumentException();
        }

        RandomizedQueue<String> deque = new RandomizedQueue<String>();

        while (!StdIn.isEmpty()) {
            deque.enqueue(StdIn.readString());
        }

        Iterator<String> iter = deque.iterator();
        int i = 0;
        while (iter.hasNext() && i < n) {
            StdOut.println(iter.next());
            i++;
        }
    }
}
