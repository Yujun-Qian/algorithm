/* *****************************************************************************
 *  Name:
 *  Date:
 *  Description:
 **************************************************************************** */

import edu.princeton.cs.algs4.Point2D;
import edu.princeton.cs.algs4.RectHV;
import edu.princeton.cs.algs4.StdDraw;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class KdTree {
    private class Node {
        final public double x;
        final public double y;
        final public int axis; // 0 - vertical; 1 - horizontal
        //corresponding rectangle
        final public double minX;
        final public double minY;
        final public double maxX;
        final public double maxY;
        public Node left;
        public Node right;
        public int count;

        public Node(Double x_, Double y_, Double minX_, Double minY_, Double maxX_, Double maxY_,
                    int axis_) {
            x = x_;
            y = y_;
            minX = minX_;
            minY = minY_;
            maxX = maxX_;
            maxY = maxY_;
            axis = axis_;
            left = null;
            right = null;
            count = 1;
        }

        void draw() {
            StdDraw.setPenRadius(0.03);
            StdDraw.setPenColor(StdDraw.BLACK);
            StdDraw.point(x, y);

            //draw splitting line
            StdDraw.setPenRadius(0.01);
            if (axis == 0) {
                StdDraw.setPenColor(StdDraw.RED);
                StdDraw.line(x, minY, x, maxY);
            }
            else {
                StdDraw.setPenColor(StdDraw.BLUE);
                StdDraw.line(minX, y, maxX, y);
            }
        }
    }

    private Node root;

    public KdTree() {
        root = null;
    }

    public boolean isEmpty() {
        return root == null;
    }

    public int size() {
        if (root != null)
            return root.count;

        return 0;
    }

    private int compare(Double a, Double b) {
        if (a > b)
            return 1;
        if (a < b)
            return -1;
        return 0;
    }

    private Node insert(Node node, Point2D p, int axis, Double minX, Double minY,
                        Double maxX, Double maxY) {
        if (p == null)
            throw new IllegalArgumentException();

        if (node == null) {
            return new Node(p.x(), p.y(), minX, minY, maxX, maxY, axis);
        }

        if (p.x() == node.x && p.y() == node.y) {
            return node;
        }

        int compareResult = 0;
        int newAxis = 1 - axis;
        if (axis == 0) {
            compareResult = compare(p.x(), node.x);
            if (compareResult < 0) {
                node.left = insert(node.left, p, newAxis, node.minX, node.minY, node.x, node.maxY);
            }
            else {
                node.right = insert(node.right, p, newAxis, node.x, node.minY, node.maxX,
                                    node.maxY);
            }
        }
        else {
            compareResult = compare(p.y(), node.y);
            if (compareResult < 0) {
                node.left = insert(node.left, p, newAxis, node.minX, node.minY, node.maxX, node.y);
            }
            else {
                node.right = insert(node.right, p, newAxis, node.minX, node.y, node.maxX,
                                    node.maxY);
            }
        }

        node.count = 1;
        if (node.right != null)
            node.count += node.right.count;
        if (node.left != null)
            node.count += node.left.count;
        return node;
    }

    public void insert(Point2D p) {
        if (p == null)
            throw new IllegalArgumentException();
        root = insert(root, p, 0, 0.0, 0.0, 1.0, 1.0);
    }

    private Node search(Node node, Point2D p) {
        if (p == null)
            throw new IllegalArgumentException();

        if (node == null)
            return null;

        if (node.x == p.x() && node.y == p.y()) {
            return node;
        }

        int compareResult;
        if (node.axis == 0) {
            compareResult = compare(p.x(), node.x);
        }
        else {
            compareResult = compare(p.y(), node.y);
        }

        if (compareResult < 0) {
            return search(node.left, p);
        }
        else {
            return search(node.right, p);
        }
    }

    public boolean contains(Point2D p) {
        if (p == null)
            throw new IllegalArgumentException();

        if (search(root, p) == null)
            return false;

        return true;
    }

    public void draw() {
        Node node = root;

        //Morris traversal
        while (node != null) {
            if (node.left != null) {
                Node temp = node.left;
                while (temp.right != null && temp.right != node) {
                    temp = temp.right;
                }

                if (temp.right == node) {
                    temp.right = null;
                    node.draw();
                    node = node.right;
                }
                else {
                    temp.right = node;
                    node = node.left;
                }

            }
            else {
                node.draw();
                node = node.right;
            }
        }
    }

    private class Points implements Iterable<Point2D> {
        private List<Point2D> points = new ArrayList<Point2D>();

        public Iterator<Point2D> iterator() {
            return points.iterator();
        }

        public void insert(Point2D p) {
            if (p == null)
                throw new IllegalArgumentException();

            points.add(p);
        }
    }

    private void range(Node node, RectHV rect, Points points) {
        if (node == null)
            return;

        Point2D p = new Point2D(node.x, node.y);
        if (rect.contains(p)) {
            points.insert(p);
        }

        if ((node.axis == 0 && node.x > rect.xmin()) || (node.axis == 1 && node.y > rect.ymin())) {
            range(node.left, rect, points);
        }

        if ((node.axis == 0 && node.x <= rect.xmax()) || (node.axis == 1 && node.y <= rect
                .ymax())) {
            range(node.right, rect, points);
        }
    }

    public Iterable<Point2D> range(RectHV rect) {
        if (rect == null) {
            throw new IllegalArgumentException();
        }

        Points points = new Points();
        range(root, rect, points);
        return points;
    }

    private boolean goLeft(Node node, Point2D p) {
        if (node.axis == 0) {
            return p.x() < node.x;
        }
        else {
            return p.y() < node.y;
        }
    }

    // params[0] => nearest distance so far
    // result[0] => champion point so far
    private void nearest(Node node, Point2D p, Double[] params, Point2D[] result) {
        if (node == null) {
            return;
        }

        Point2D self = new Point2D(node.x, node.y);
        if (p.distanceSquaredTo(self) < params[0]) {
            params[0] = p.distanceSquaredTo(self);
            result[0] = self;
        }

        if (goLeft(node, p)) {
            if (node.left != null) {
                RectHV rect = new RectHV(node.left.minX, node.left.minY, node.left.maxX,
                                         node.left.maxY);
                if (rect.distanceSquaredTo(p) < params[0]) {
                    nearest(node.left, p, params, result);
                }
            }

            if (node.right != null) {
                RectHV rect = new RectHV(node.right.minX, node.right.minY, node.right.maxX,
                                         node.right.maxY);
                if (rect.distanceSquaredTo(p) < params[0]) {
                    nearest(node.right, p, params, result);
                }
            }
        }
        else {
            if (node.right != null) {
                RectHV rect = new RectHV(node.right.minX, node.right.minY, node.right.maxX,
                                         node.right.maxY);
                if (rect.distanceSquaredTo(p) < params[0]) {
                    nearest(node.right, p, params, result);
                }
            }

            if (node.left != null) {
                RectHV rect = new RectHV(node.left.minX, node.left.minY, node.left.maxX,
                                         node.left.maxY);
                if (rect.distanceSquaredTo(p) < params[0]) {
                    nearest(node.left, p, params, result);
                }
            }
        }
    }

    public Point2D nearest(Point2D p) {
        if (p == null)
            throw new IllegalArgumentException();

        Double nearestSoFar = Double.MAX_VALUE;
        Double params[] = {
                nearestSoFar
        };

        Point2D[] result = new Point2D[1];
        result[0] = null;

        nearest(root, p, params, result);
        return result[0];
    }

    public static void main(String[] args) {

    }
}
