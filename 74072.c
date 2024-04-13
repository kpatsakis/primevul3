partition_list_add(char *name, struct partition_list *pl)
{
    struct partition_list *p;

    /* Is name already on list? */
    for (p=pl; p; p = p->next) {
        if (!strcmp(p->name, name))
            return(pl);
    }

    /* Add entry to start of list and return new list */
    p = xzmalloc(sizeof(struct partition_list));
    p->next = pl;
    p->name = xstrdup(name);

    return(p);
}
