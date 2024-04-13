njs_typed_array_compare_u16(const void *a, const void *b, void *c)
{
    return *((const uint16_t *) a) - *((const uint16_t *) b);
}