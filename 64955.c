const char* NoMeta(const char* str)
{
    if (strchr(str, '%') != NULL)
        return "**** CORRUPTED FORMAT STRING ***";

    return str;
}
