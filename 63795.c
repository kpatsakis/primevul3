static int comp_t1_glyphs(const void *pa, const void *pb, void *p
    __attribute__ ((unused)))
{
    return strcmp(*(const char *const *) pa, *(const char *const *) pb);
}
