njs_typed_array_compare(double a, double b)
{
    if (njs_slow_path(isnan(a))) {
        if (isnan(b)) {
            return 0;
        }

        return 1;
    }

    if (njs_slow_path(isnan(b))) {
        return -1;
    }

    if (a < b) {
        return -1;
    }

    if (a > b) {
        return 1;
    }

    return signbit(b) - signbit(a);
}