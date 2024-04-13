njs_typed_array_compare_i32(const void *a, const void *b, void *c)
{
    int32_t  ai, bi;

    ai = *(const int32_t *) a;
    bi = *(const int32_t *) b;

    return (ai > bi) - (ai < bi);
}