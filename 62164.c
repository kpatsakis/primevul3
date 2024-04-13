static void calc_gauss(double *res, int n, double r2)
{
    double alpha = 0.5 / r2;
    double mul = exp(-alpha), mul2 = mul * mul;
    double cur = sqrt(alpha / M_PI);

    res[0] = cur;
    cur *= mul;
    res[1] = cur;
    for (int i = 2; i <= n; ++i) {
        mul *= mul2;
        cur *= mul;
        res[i] = cur;
    }
}
