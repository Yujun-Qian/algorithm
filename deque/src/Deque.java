import edu.princeton.cs.algs4.StdOut;

import java.util.Iterator;

public class Deque<Item> implements Iterable<Item> {
    private Node first = null;
    private Node last = null;
    private int size = 0;

    private class Node {
        Item item;
        Node next;
        Node prev;
    }

    private class DequeIterator implements Iterator<Item> {
        Node current = null;

        DequeIterator() {
            current = first;
        }

        public boolean hasNext() {
            return !(current == null);
        }

        public void remove() {
            /* not supported */
            throw new java.lang.UnsupportedOperationException();
        }

        public Item next() {
            if (!hasNext()) {
                throw new java.util.NoSuchElementException();
            }

            Item item = current.item;
            current = current.next;
            return item;
        }
    }

    // construct an empty deque
    public Deque() {
    }

    // is the deque empty?
    public boolean isEmpty() {
        return size == 0;
    }

    // return the number of items on the deque
    public int size() {
        return size;
    }

    // add the item to the front
    public void addFirst(Item item) {
        if (item == null)
            throw new java.lang.IllegalArgumentException();

        Node node = new Node();
        node.item = item;
        node.next = first;
        node.prev = null;

        if (first != null) {
            first.prev = node;
        }

        first = node;
        if (last == null) {
            last = node;
        }

        size++;
    }

    // add the item to the back
    public void addLast(Item item) {
        if (item == null)
            throw new java.lang.IllegalArgumentException();

        Node node = new Node();
        node.item = item;
        node.next = null;
        node.prev = last;

        if (last != null) {
            last.next = node;
        }

        last = node;
        if (first == null) {
            first = node;
        }

        size++;
    }

    // remove and return the item from the front
    public Item removeFirst() {
        if (isEmpty()) {
            throw new java.util.NoSuchElementException();
        }

        Item item = first.item;

        if (first.next != null) {
            first.next.prev = null;
        }

        first = first.next;

        if (first == null) {
            last = null;
        }

        size--;
        return item;
    }

    // remove and return the item from the back
    public Item removeLast() {
        if (isEmpty()) {
            throw new java.util.NoSuchElementException();
        }

        Item item = last.item;

        if (last.prev != null) {
            last.prev.next = null;
        }

        last = last.prev;

        if (last == null) {
            first = null;
        }

        size--;
        return item;
    }

    // return an iterator over items in order from front to back
    public Iterator<Item> iterator() {
        return new DequeIterator();
    }

    // unit testing (required)
    public static void main(String[] args) {
        int[] test = {1, 2, 4, 6, 7, 9};

        Deque<Integer> deque = new Deque<Integer>();

        for (int i = 0; i < 6; i++) {
            deque.addLast(test[i]);
        }

        Iterator<Integer> iter = deque.iterator();
        while (iter.hasNext()) {
            StdOut.print(iter.next());
        }

        while (!deque.isEmpty()) {
            StdOut.print(deque.removeLast());
        }

        for (int i = 0; i < 6; i++) {
            deque.addFirst(test[i]);
        }

        iter = deque.iterator();
        while (iter.hasNext()) {
            StdOut.print(iter.next());
        }

        while (!deque.isEmpty()) {
            StdOut.print(deque.removeFirst());
        }
    }

}
