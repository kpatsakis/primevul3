RemoveListItem(list_item_t **pfirst, match_fn_t match, LPVOID ctx)
{
    LPVOID data = NULL;
    list_item_t **pnext;

    for (pnext = pfirst; *pnext; pnext = &(*pnext)->next)
    {
        list_item_t *item = *pnext;
        if (!match(item->data, ctx))
        {
            continue;
        }

        /* Found item, remove from the list and free memory */
        *pnext = item->next;
        data = item->data;
        free(item);
        break;
    }
    return data;
}
