njs_typed_array_compare_f64(const void *a, const void *b, void *c)
{
    return njs_typed_array_compare(*(const double *) a, *(const double *) b);
}