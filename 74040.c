static void freefieldlist(struct fieldlist *l)
{
    struct fieldlist *n;

    while (l) {
        n = l->next;
        free(l->section);
        strarray_free(l->fields);
        free(l->trail);
        if (l->rock) free(l->rock);
        free((char *)l);
        l = n;
    }
}
