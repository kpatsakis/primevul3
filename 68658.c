static void init_gauss(void) {
    /* init gauss table */
    int n = gauss_n;
    int m, i, k, n_half = (n >> 1);
    int j;
    int sign;
    double ck;
    double x, x_inc, xz;
    double z[35];
    double *gptr, *t;

    _WM_Lock(&gauss_lock);
    if (gauss_table) {
        _WM_Unlock(&gauss_lock);
        return;
    }

    newt_coeffs[0][0] = 1;
    for (i = 0; i <= n; i++) {
        newt_coeffs[i][0] = 1;
        newt_coeffs[i][i] = 1;

        if (i > 1) {
            newt_coeffs[i][0] = newt_coeffs[i - 1][0] / i;
            newt_coeffs[i][i] = newt_coeffs[i - 1][0] / i;
        }

        for (j = 1; j < i; j++) {
            newt_coeffs[i][j] = newt_coeffs[i - 1][j - 1]
                    + newt_coeffs[i - 1][j];
            if (i > 1)
                newt_coeffs[i][j] /= i;
        }
        z[i] = i / (4 * M_PI);
    }

    for (i = 0; i <= n; i++)
        for (j = 0, sign = (int) pow(-1, i); j <= i; j++, sign *= -1)
            newt_coeffs[i][j] *= sign;

    t = malloc((1<<FPBITS) * (n + 1) * sizeof(double));
    x_inc = 1.0 / (1<<FPBITS);
    for (m = 0, x = 0.0; m < (1<<FPBITS); m++, x += x_inc) {
        xz = (x + n_half) / (4 * M_PI);
        gptr = &t[m * (n + 1)];

        for (k = 0; k <= n; k++) {
            ck = 1.0;

            for (i = 0; i <= n; i++) {
                if (i == k)
                    continue;

                ck *= (sin(xz - z[i])) / (sin(z[k] - z[i]));
            }
            *gptr++ = ck;
        }
    }

    gauss_table = t;
    _WM_Unlock(&gauss_lock);
}
