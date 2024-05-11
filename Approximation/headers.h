double fabs(double a);
double diff(double fi, double fj, double xi, double xj);
int solve_system(double *a, double *b, double *x);
void d_Akima_fill(double *x, double *f, int n, double *d);
double general_pol(double x, double xi, double xip1, double fi, double fip1, double di, double dip1);
double interpol_pol(double x, double x0, double x1, double x2, double f0, double f1, double f2, double d);