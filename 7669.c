njs_typed_array_compare_i16(const void *a, const void *b, void *c)
{
    return *((const int16_t *) a) - *((const int16_t *) b);
}