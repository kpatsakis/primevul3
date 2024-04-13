njs_typed_array_compare_u8(const void *a, const void *b, void *c)
{
    return *((const uint8_t *) a) - *((const uint8_t *) b);
}