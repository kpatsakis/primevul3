static int xbackup_addmbox(struct findall_data *data, void *rock)
{
    if (!data) return 0;
    ptrarray_t *list = (ptrarray_t *) rock;

    if (!data->mbname) {
        /* No partial matches */ /* FIXME ??? */
        return 0;
    }

    /* Only add shared mailboxes or user INBOXes */
    if (!mbname_localpart(data->mbname) ||
        (!mbname_isdeleted(data->mbname) &&
         !strarray_size(mbname_boxes(data->mbname)))) {

        ptrarray_append(list, mbname_dup(data->mbname));
    }

    return 0;
}
