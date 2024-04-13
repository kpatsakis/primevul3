static const char **check_cs_token_pair(void)
{
    const char **p = (const char **) cs_token_pairs_list;
    for (; p[0] != NULL; ++p)
        if (t1_buf_prefix(p[0]) && t1_buf_suffix(p[1]))
            return p;
    return NULL;
}
