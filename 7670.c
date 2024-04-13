njs_typed_array_compare_i8(const void *a, const void *b, void *c)
{
    return *((const int8_t *) a) - *((const int8_t *) b);
}