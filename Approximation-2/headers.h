double fabs(double a);
void xi_fill(double *x, double *xi, int n);
void v_fill(double *vi, double *x, double *f, double *xi, int n);
int solve_system(double *a, double *b, double *x, int n);
double general(double x, double vi, double vip1, double fi, double xi, double xii, double xiip1);
void test_sin(int amount, int prec, double left, double right);
void test_abs(int amount, int prec, double left, double right);
void test_sin_outl(int amount, int prec, double left, double right);