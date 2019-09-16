/* *****************************************************************************
 *  Name:
 *  Date:
 *  Description:
 **************************************************************************** */

import edu.princeton.cs.algs4.Point2D;
import edu.princeton.cs.algs4.RectHV;
import edu.princeton.cs.algs4.SET;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class PointSET {
    private final SET<Point2D> tree;

    public PointSET()                               // construct an empty set of points
    {
        tree = new SET<Point2D>();
    }

    public boolean isEmpty()                      // is the set empty?
    {
        return tree.isEmpty();
    }

    public int size()                         // number of points in the set
    {
        return tree.size();
    }

    public void insert(
            Point2D p)              // add the point to the set (if it is not already in the set)
    {
        if (p == null)
            throw new IllegalArgumentException();
        tree.add(p);
    }

    public boolean contains(Point2D p)            // does the set contain point p?
    {
        if (p == null)
            throw new IllegalArgumentException();

        return tree.contains(p);
    }

    public void draw()                         // draw all points to standard draw
    {
        Iterator<Point2D> iter = tree.iterator();

        while (iter.hasNext()) {
            iter.next().draw();
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

    public Iterable<Point2D> range(
            RectHV rect)             // all points that are inside the rectangle (or on the boundary)
    {
        if (rect == null)
            throw new IllegalArgumentException();

        Points points = new Points();
        Iterator<Point2D> iter = tree.iterator();
        while (iter.hasNext()) {
            Point2D p = iter.next();
            if (rect.contains(p)) {
                points.insert(p);
            }
        }
        return points;
    }

    public Point2D nearest(
            Point2D p)             // a nearest neighbor in the set to point p; null if the set is empty
    {
        if (p == null) {
            throw new IllegalArgumentException();
        }

        Iterator<Point2D> iter = tree.iterator();
        Double minDistance = Double.MAX_VALUE;
        Point2D ret = null;

        while (iter.hasNext()) {
            Point2D point = iter.next();

            Double distance = p.distanceSquaredTo(point);
            if (distance < minDistance) {
                minDistance = distance;
                ret = point;
            }
        }

        return ret;
    }

    public static void main(String[] args) {

    }
}
