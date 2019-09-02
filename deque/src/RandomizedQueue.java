import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdRandom;

import java.util.Iterator;

public class RandomizedQueue<Item> implements Iterable<Item> {
    private int size;
    private int index;
    private Item a[];

    private boolean isFull() {
        return size == index;
    }

    private class RandomizedQueueIterator implements Iterator<Item> {
        private int indexOfA[];
        private int currentIndex;

        RandomizedQueueIterator() {
            indexOfA = new int[index];
            currentIndex = index;
            for (int i = 0; i < index; i++) {
                indexOfA[i] = i;
            }
        }

        public boolean hasNext() {
            return !(currentIndex == 0);
        }

        public Item next() {
            if (!hasNext()) {
                throw new java.util.NoSuchElementException();
            }
            int i = StdRandom.uniform(currentIndex);
            Item item = a[indexOfA[i]];
            indexOfA[i] = indexOfA[--currentIndex];
            return item;
        }

        public void remove() {
            throw new java.lang.UnsupportedOperationException();
        }
    }

    // construct an empty randomized queue
    public RandomizedQueue() {
        index = 0;
        size = 1;
        a = (Item[]) new Object[1];
    }

    // is the randomized queue empty?
    public boolean isEmpty() {
        return index == 0;
    }

    // return the number of items on the randomized queue
    public int size() {
        return index;
    }

    // add the item
    public void enqueue(Item item) {
        if (item == null) {
            throw new java.lang.IllegalArgumentException();
        }

        Item old[];
        if (isFull()) {
            size = size * 2;
            old = a;
            a = (Item[]) new Object[size];

            for (int i = 0; i < index; i++) {
                a[i] = old[i];
            }
        }

        a[index++] = item;
    }

    // remove and return a random item
    public Item dequeue() {
        if (isEmpty()) {
            throw new java.util.NoSuchElementException();
        }
        int i = StdRandom.uniform(index);
        Item item = a[i];

        for (; i < index - 1; i++) {
            a[i] = a[i + 1];
        }

        a[--index] = null;

        if (index < size / 4) {
            size = size / 2;

            Item[] old = a;
            a = (Item[]) new Object[size];

            for (i = 0; i < index; i++) {
                a[i] = old[i];
            }
        }
        return item;
    }

    // return a random item (but do not remove it)
    public Item sample() {
        if (isEmpty()) {
            throw new java.util.NoSuchElementException();
        }
        int i = StdRandom.uniform(index);
        return a[i];
    }

    // return an independent iterator over items in random order
    public Iterator<Item> iterator() {
        return new RandomizedQueueIterator();
    }

    // unit testing (required)
    public static void main(String[] args) {
        int[] test = {1, 2, 4, 6, 7, 9};

        RandomizedQueue<Integer> deque = new RandomizedQueue<Integer>();

        for (int i = 0; i < 6; i++) {
            deque.enqueue(test[i]);
        }

        Iterator<Integer> iter = deque.iterator();
        while (iter.hasNext()) {
            StdOut.print(iter.next());
        }

        while (!deque.isEmpty()) {
            StdOut.print(deque.dequeue());
        }

        for (int i = 0; i < 6; i++) {
            deque.enqueue(test[i]);
        }

        iter = deque.iterator();
        while (iter.hasNext()) {
            StdOut.print(iter.next());
        }

        while (!deque.isEmpty()) {
            StdOut.print(deque.dequeue());
        }
    }
}
