njs_typed_array_compare_f32(const void *a, const void *b, void *c)
{
    return njs_typed_array_compare(*(const float *) a, *(const float *) b);
}