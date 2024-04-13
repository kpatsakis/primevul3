static void appendfieldlist(struct fieldlist **l, char *section,
                     strarray_t *fields, char *trail,
                     void *d, size_t size)
{
    struct fieldlist **tail = l;

    while (*tail) tail = &(*tail)->next;

    *tail = (struct fieldlist *)xmalloc(sizeof(struct fieldlist));
    (*tail)->section = xstrdup(section);
    (*tail)->fields = fields;
    (*tail)->trail = xstrdup(trail);
    if(d && size) {
        (*tail)->rock = xmalloc(size);
        memcpy((*tail)->rock, d, size);
    } else {
        (*tail)->rock = NULL;
    }
    (*tail)->next = 0;
}
