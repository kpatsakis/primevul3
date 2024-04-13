ClearCompatInfo(CompatInfo *info)
{
    free(info->name);
    darray_free(info->interps);
}
