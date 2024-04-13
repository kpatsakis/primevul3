static void load_creator_from_old_format(
    FILE       *fp,
    xref_t     *xref,
    const char *buf)
{
    int            i, n_eles, length, is_escaped, obj_id;
    char          *c, *ascii, *start, *s, *saved_buf_search, *obj;
    pdf_creator_t *info;

    info = new_creator(&n_eles);

    for (i=0; i<n_eles; ++i)
    {
        if (!(c = strstr(buf, info[i].key)))
          continue;

        /* Find the value (skipping whitespace) */
        c += strlen(info[i].key);
        while (isspace(*c))
          ++c;

        /* If looking at the start of a pdf token, we have gone too far */
        if (*c == '/')
          continue;

        /* If the value is a number and not a '(' then the data is located in
         * an object we need to fetch, and not inline
         */
        obj = saved_buf_search = NULL;
        if (isdigit(*c))
        {
            obj_id = atoi(c);
            saved_buf_search = c;
            s = saved_buf_search;

            obj = get_object(fp, obj_id, xref, NULL, NULL);
            c = obj;

            /* Iterate to '(' */
            while (c && (*c != '('))
             ++c;

            /* Advance the search to the next token */
            while (s && (*s == '/'))
              ++s;
            saved_buf_search = s;
        }
          
        /* Find the end of the value */
        start = c;
        length = is_escaped = 0;
        while (c && ((*c != '\r') && (*c != '\n') && (*c != '<')))
        {
            /* Bail out if we see an un-escaped ')' closing character */
            if (!is_escaped && (*c == ')'))
              break;
            else if (*c == '\\')
              is_escaped = 1;
            else
              is_escaped = 0;

            ++c;
            ++length;
        }

        if (length == 0)
          continue;

        /* Add 1 to length so it gets the closing ')' when we copy */
        if (length)
          length += 1;
        length = (length > KV_MAX_VALUE_LENGTH) ? KV_MAX_VALUE_LENGTH : length;
        strncpy(info[i].value, start, length);
        info[i].value[KV_MAX_VALUE_LENGTH - 1] = '\0';

        /* Restore where we were searching from */
        if (saved_buf_search)
        {
            /* Release memory from get_object() called earlier */
            free(obj);
            c = saved_buf_search;
        }
    } /* For all creation information tags */

    /* Go through the values and convert if encoded */
    for (i=0; i<n_eles; ++i)
      if ((ascii = decode_text_string(info[i].value, strlen(info[i].value))))
      {
          strncpy(info[i].value, ascii, strlen(info[i].value));
          free(ascii);
      }

    xref->creator = info;
    xref->n_creator_entries = n_eles;
}
