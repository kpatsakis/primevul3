static void load_kids(FILE *fp, int pages_id, xref_t *xref)
{
    int   dummy, buf_idx, kid_id;
    char *data, *c, buf[32];

    /* Get kids */
    data = get_object(fp, pages_id, xref, NULL, &dummy);
    if (!data || !(c = strstr(data, "/Kids")))
    {
        free(data);
        return;
    }
    
    c = strchr(c, '[');
    buf_idx = 0;
    memset(buf, 0, sizeof(buf));
    while (*(++c) != ']')
    {
        if (isdigit(*c) || (*c == ' '))
          buf[buf_idx++] = *c;
        else if (isalpha(*c))
        {
            kid_id = atoi(buf);
            add_kid(kid_id, xref);
            buf_idx = 0;
            memset(buf, 0, sizeof(buf));

            /* Check kids of kid */
            load_kids(fp, kid_id, xref);
        }
        else if (*c == ']')
          break;
    }

    free(data);
}
