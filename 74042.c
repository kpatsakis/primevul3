static void getannotation_response(const char *mboxname,
                                   uint32_t uid
                                        __attribute__((unused)),
                                   const char *entry,
                                   struct attvaluelist *attvalues,
                                   void *rock __attribute__((unused)))
{
    int sep = '(';
    struct attvaluelist *l;
    char *extname = *mboxname ?
        mboxname_to_external(mboxname, &imapd_namespace, imapd_userid) :
        xstrdup("");  /* server annotation */

    prot_printf(imapd_out, "* ANNOTATION ");
    prot_printastring(imapd_out, extname);
    prot_putc(' ', imapd_out);
    prot_printstring(imapd_out, entry);
    prot_putc(' ', imapd_out);
    for (l = attvalues ; l ; l = l->next) {
        prot_putc(sep, imapd_out);
        sep = ' ';
        prot_printstring(imapd_out, l->attrib);
        prot_putc(' ',  imapd_out);
        prot_printmap(imapd_out, l->value.s, l->value.len);
    }
    prot_printf(imapd_out, ")\r\n");
    free(extname);
}
