ClearKeyNamesInfo(KeyNamesInfo *info)
{
    free(info->name);
    darray_free(info->key_names);
    darray_free(info->aliases);
}
