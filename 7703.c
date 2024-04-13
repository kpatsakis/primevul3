njs_typed_array_compare_u32(const void *a, const void *b, void *c)
{
    uint32_t  au, bu;

    au = *(const uint32_t *) a;
    bu = *(const uint32_t *) b;

    return (au > bu) - (au < bu);
}