/* *****************************************************************************
 *  Name:
 *  Date:
 *  Description:
 **************************************************************************** */

import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.Stack;
import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.StdOut;

import java.util.Arrays;
import java.util.Comparator;
import java.util.Iterator;

public class FastCollinearPoints {
    private Stack<LineSegment> lines = new Stack<>();

    public FastCollinearPoints(Point[] points)    // finds all line segments containing 4 points
    {
        if (points == null) {
            throw new java.lang.IllegalArgumentException();
        }

        for (int i = 0; i < points.length; i++) {
            if (points[i] == null)
                throw new java.lang.IllegalArgumentException();
        }

        Arrays.sort(points);

        for (int i = 1; i < points.length; i++) {
            if (points[i].compareTo(points[i - 1]) == 0)
                throw new java.lang.IllegalArgumentException();
        }

        if (points.length < 4) {
            return;
        }

        for (int i = 0; i < points.length; i++) {
            int size = points.length - i - 1;
            Point[] aux = new Point[size];
            for (int j = i + 1, k = 0; j < points.length; j++, k++) {
                aux[k] = points[j];
            }

            Comparator<Point> slopeOrder = points[i].slopeOrder();
            Arrays.sort(aux, slopeOrder);

            int numOfPoints = 1;
            int k = 1;
            for (k = 1; k < points.length - i - 1; k++) {
                if (slopeOrder.compare(aux[k - 1], aux[k]) == 0) {
                    numOfPoints++;
                }
                else {
                    if (numOfPoints >= 3) {
                        lines.push(new LineSegment(points[i], aux[k - 1]));
                    }

                    // reset the number of points in the same line
                    numOfPoints = 1;
                }
            }

            if (numOfPoints >= 3) {
                lines.push(new LineSegment(points[i], aux[k - 1]));
            }
        }
    }

    public int numberOfSegments()        // the number of line segments
    {
        return lines.size();
    }

    public LineSegment[] segments()                // the line segments
    {
        LineSegment[] ret = new LineSegment[lines.size()];

        Iterator<LineSegment> iterator = lines.iterator();
        int i = 0;
        while (iterator.hasNext()) {
            ret[i++] = iterator.next();
        }

        return ret;
    }

    public static void main(String[] args) {
        // read the n points from a file
        In in = new In(args[0]);
        int n = in.readInt();
        Point[] points = new Point[n];
        for (int i = 0; i < n; i++) {
            int x = in.readInt();
            int y = in.readInt();
            points[i] = new Point(x, y);
        }

        // draw the points
        StdDraw.enableDoubleBuffering();
        StdDraw.setXscale(0, 32768);
        StdDraw.setYscale(0, 32768);
        for (Point p : points) {
            p.draw();
        }
        StdDraw.show();

        // print and draw the line segments
        FastCollinearPoints collinear = new FastCollinearPoints(points);
        for (LineSegment segment : collinear.segments()) {
            StdOut.println(segment);
            segment.draw();
        }
        StdDraw.show();
    }
}
