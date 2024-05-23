#include <stdio.h>

typedef struct {
    double x, y;
} Point;

Point avg(Point a, Point b) {
    Point ans;
    ans.x = (a.x + b.x) / 2;
    ans.y = (a.y + b.y) / 2;
    return ans;
}

double fabs(double x) {
    return x > 0 ? x : -1*x;
}

double L(Point p, Point p1, Point p2) {
    return (p.x - p1.x) * (p2.y - p1.y) - (p.y - p1.y) * (p2.x - p1.x);
}

double phi(Point p, Point pi, Point p1, Point p2, Point p3, Point p4) {
    return L(p, p1, p2) * L(p, p3, p4) / (L(pi, p1, p2) * L(pi, p3, p4));
}

void triangulation(Point p, double *x, double *y, int n, Point *p1, Point *p2, Point *p3) {
    double hx, hy;
    int i, j;
    hx = (x[n] - x[0]) / n;
    hy = (y[n] - y[0]) / n;
    i = (int)((p.x - x[0]) / hx);
    j = (int)((p.y - y[0]) / hy);

    if (i != n && j != n) {
        if ((p.y - y[j]) * (x[i + 1] - x[i]) - (p.x - x[i]) * (y[j + 1] - y[j]) < 0) {
            p1->x = x[i];
            p1->y = y[j];
            p2->x = x[i + 1];
            p2->y = y[j];
            p3->x = x[i + 1];
            p3->y = y[j + 1];
        } else {
            p1->x = x[i];
            p1->y = y[j];
            p2->x = x[i];
            p2->y = y[j + 1];
            p3->x = x[i + 1];
            p3->y = y[j + 1];
        }
    } else if (i != n && j == n) {
        p1->x = x[i];
        p1->y = y[j - 1];
        p2->x = x[i];
        p2->y = y[j];
        p3->x = x[i + 1];
        p3->y = y[j];
    } else if (i == n && j != n) {
        p1->x = x[i - 1];
        p1->y = y[j];
        p2->x = x[i];
        p2->y = y[j];
        p3->x = x[i];
        p3->y = y[j + 1];
    } else if (i == n && j == n) {
        p1->x = x[i - 1];
        p1->y = y[j - 1];
        p2->x = x[i];
        p2->y = y[j - 1];
        p3->x = x[i];
        p3->y = y[j];
    }
}

double value(Point p, Point p1, Point p2, Point p3, double *f) {
    double ans, phi1, phi2, phi3, phi4, phi5, phi6;
    double lphi[6];
    Point p4, p5, p6;

    ans = 0;

    p4 = avg(p1, p2);
    p5 = avg(p2, p3);
    p6 = avg(p1, p3);

    phi1 = phi(p, p1, p2, p3, p4, p6);
    phi2 = phi(p, p2, p1, p3, p4, p5);
    phi3 = phi(p, p3, p1, p2, p5, p6);
    phi4 = phi(p, p4, p2, p3, p1, p3);
    phi5 = phi(p, p5, p1, p3, p1, p2);
    phi6 = phi(p, p6, p2, p3, p1, p2);

    lphi[0] = phi1;
    lphi[1] = phi2;
    lphi[2] = phi3;
    lphi[3] = phi4;
    lphi[4] = phi5;
    lphi[5] = phi6;

    for (int i = 0; i < 6; i++) {
        ans = ans + f[i]*lphi[i];
    }

    return ans;
}