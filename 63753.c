static void append_cs_return(cs_entry *ptr)
{
    unsigned short cr;
    int i;
    byte *p, *q, *data, *new_data;
    assert(ptr != NULL && ptr->valid && ptr->used);

    /* decrypt the cs data to t1_buf_array, append CS_RETURN */
    p = (byte *) t1_buf_array;
    data = ptr->data + 4;
    cr = 4330;
    for (i = 0; i < ptr->cslen; i++)
        *p++ = cs_getchar();
    *p = CS_RETURN;

    /* encrypt the new cs data to new_data */
    new_data = xtalloc(ptr->len + 1, byte);
    memcpy(new_data, ptr->data, 4);
    p = new_data + 4;
    q = (byte *) t1_buf_array;
    cr = 4330;
    for (i = 0; i < ptr->cslen + 1; i++)
        *p++ = cencrypt(*q++, &cr);
    memcpy(p, ptr->data + 4 + ptr->cslen, ptr->len - ptr->cslen - 4);

    /* update *ptr */
    xfree(ptr->data);
    ptr->data = new_data;
    ptr->len++;
    ptr->cslen++;
}
