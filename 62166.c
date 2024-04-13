static void coeff_blur121(double *coeff, int n)
{
    double prev = coeff[1];
    for (int i = 0; i <= n; ++i) {
        double res = (prev + 2 * coeff[i] + coeff[i + 1]) / 4;
        prev = coeff[i];
        coeff[i] = res;
    }
}
