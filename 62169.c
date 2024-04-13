static void find_best_method(BlurMethod *blur, double r2)
{
    static const int index[][4] = {
        { 1, 2, 3, 4 },
        { 1, 2, 3, 5 },
        { 1, 2, 4, 6 },
    };

    double mu[5];
    if (r2 < 1.9) {
        blur->level = blur->prefilter = blur->filter = 0;

        if (r2 < 0.5) {
            mu[2] = 0.085 * r2 * r2 * r2;
            mu[1] = 0.5 * r2 - 4 * mu[2];
            mu[3] = mu[4] = 0;
        } else {
            calc_gauss(mu, 4, r2);
        }
    } else {
        double mul = 1;
        if (r2 < 6.693) {
            blur->level = 0;

            if (r2 < 2.8)
                blur->prefilter = 1;
            else if (r2 < 4.4)
                blur->prefilter = 2;
            else
                blur->prefilter = 3;

            blur->filter = blur->prefilter - 1;
        } else {
            frexp((r2 + 0.7) / 26.5, &blur->level);
            blur->level = (blur->level + 3) >> 1;
            mul = pow(0.25, blur->level);
            r2 *= mul;

            if (r2 < 3.15 - 1.5 * mul)
                blur->prefilter = 0;
            else if (r2 < 5.3 - 5.2 * mul)
                blur->prefilter = 1;
            else
                blur->prefilter = 2;

            blur->filter = blur->prefilter;
        }
        calc_coeff(mu + 1, index[blur->filter], blur->prefilter, r2, mul);
    }

    for (int i = 1; i <= 4; ++i)
        blur->coeff[i - 1] = (int)(0x10000 * mu[i] + 0.5);
}
