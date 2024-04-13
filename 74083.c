static void section_list_append(struct section **l,
                                const char *name,
                                const struct octetinfo *oi)
{
    struct section **tail = l;

    while (*tail) tail = &(*tail)->next;

    *tail = xzmalloc(sizeof(struct section));
    (*tail)->name = xstrdup(name);
    (*tail)->octetinfo = *oi;
    (*tail)->next = NULL;
}
