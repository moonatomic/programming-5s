typedef struct {double x, y;} Point;
Point avg(Point a, Point b);
double fabs(double x);
double L(Point p, Point p1, Point p2);
double phi(Point p, Point pi, Point p1, Point p2, Point p3, Point p4);
void triangulation(Point p, double *x, double *y, int n, Point *p1, Point *p2, Point *p3);
double value(Point p, Point p1, Point p2, Point p3, double *f);
void test_sin(double left, double right, int amount, int prec);
void test_abs(double left, double right, int amount, int prec);