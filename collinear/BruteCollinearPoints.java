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
import java.util.Iterator;

public class BruteCollinearPoints {
    private Stack<LineSegment> lines = new Stack<>();

    public BruteCollinearPoints(Point[] points)    // finds all line segments containing 4 points
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

        for (int i = 0; i < points.length; i++)
            for (int j = i + 1; j < points.length; j++)
                for (int m = j + 1; m < points.length; m++)
                    for (int n = m + 1; n < points.length; n++) {
                        if (points[i].slopeOrder().compare(points[j], points[m]) == 0
                                && points[i].slopeOrder().compare(points[j], points[n]) == 0) {
                            lines.push(new LineSegment(points[i], points[n]));
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
        BruteCollinearPoints collinear = new BruteCollinearPoints(points);
        for (LineSegment segment : collinear.segments()) {
            StdOut.println(segment);
            segment.draw();
        }
        StdDraw.show();
    }
}
