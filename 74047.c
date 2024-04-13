static void getmetadata_response(const char *mboxname,
                                 uint32_t uid __attribute__((unused)),
                                 const char *entry,
                                 struct attvaluelist *attvalues,
                                 void *rock)
{
    struct getmetadata_options *opts = (struct getmetadata_options *)rock;

    if (strcmpsafe(mboxname, opts->lastname) || !entry) {
        if (opts->items.count) {
            char *extname = NULL;
            size_t i;
            if (opts->lastname)
                extname = mboxname_to_external(opts->lastname, &imapd_namespace, imapd_userid);
            else
                extname = xstrdup("");
            prot_printf(imapd_out, "* METADATA ");
            prot_printastring(imapd_out, extname);
            prot_putc(' ', imapd_out);
            for (i = 0; i + 1 < opts->items.count; i+=2) {
                prot_putc(i ? ' ' : '(', imapd_out);
                const struct buf *key = bufarray_nth(&opts->items, i);
                prot_printmap(imapd_out, key->s, key->len);
                prot_putc(' ', imapd_out);
                const struct buf *val = bufarray_nth(&opts->items, i+1);
                prot_printmap(imapd_out, val->s, val->len);
            }
            prot_printf(imapd_out, ")\r\n");
            free(extname);
        }
        free(opts->lastname);
        opts->lastname = xstrdupnull(mboxname);
        bufarray_fini(&opts->items);
    }

    struct attvaluelist *l;
    struct buf buf = BUF_INITIALIZER;

    for (l = attvalues ; l ; l = l->next) {
        /* size check */
        if (opts->maxsize && l->value.len >= opts->maxsize) {
            if (l->value.len > opts->biggest) opts->biggest = l->value.len;
            continue;
        }
        /* check if it's a value we print... */
        buf_reset(&buf);
        if (!strcmp(l->attrib, "value.shared"))
            buf_appendcstr(&buf, "/shared");
        else if (!strcmp(l->attrib, "value.priv"))
            buf_appendcstr(&buf, "/private");
        else
            continue;
        buf_appendcstr(&buf, entry);

        bufarray_append(&opts->items, &buf);
        bufarray_append(&opts->items, &l->value);
    }
    buf_free(&buf);
}
