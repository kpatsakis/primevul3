static int parse_getmetadata_options(const strarray_t *sa,
                                     struct getmetadata_options *opts)
{
    int i;
    int n = 0;
    struct getmetadata_options dummy = OPTS_INITIALIZER;

    if (!opts) opts = &dummy;

    for (i = 0 ; i < sa->count ; i+=2) {
        const char *option = sa->data[i];
        const char *value = sa->data[i+1];
        if (!value)
            return -1;
        if (!strcasecmp(option, "MAXSIZE")) {
            char *end = NULL;
            /* we add one so that it's "less than" maxsize
             * and zero works but is still true */
            opts->maxsize = strtoul(value, &end, 10) + 1;
            if (!end || *end || end == value)
                return -1;
            n++;
        }
        else if (!strcasecmp(option, "DEPTH")) {
            if (!strcmp(value, "0"))
                opts->depth = 0;
            else if (!strcmp(value, "1"))
                opts->depth = 1;
            else if (!strcasecmp(value, "infinity"))
                opts->depth = -1;
            else
                return -1;
            n++;
        }
        else {
            return 0;
        }
    }

    return n;
}
