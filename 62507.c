static char* malloc_readlink(const char *linkname)
{
    char buf[PATH_MAX + 1];
    int len;

    len = readlink(linkname, buf, sizeof(buf)-1);
    if (len >= 0)
    {
        buf[len] = '\0';
        return xstrdup(buf);
    }
    return NULL;
}
