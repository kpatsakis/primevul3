static void section_list_free(struct section *l)
{
    struct section *n;

    while (l) {
        n = l->next;
        free(l->name);
        free(l);
        l = n;
    }
}
