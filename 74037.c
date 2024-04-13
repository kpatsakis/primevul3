static void fetchargs_fini (struct fetchargs *fa)
{
    section_list_free(fa->binsections);
    section_list_free(fa->sizesections);
    section_list_free(fa->bodysections);
    freefieldlist(fa->fsections);
    strarray_fini(&fa->headers);
    strarray_fini(&fa->headers_not);
    strarray_fini(&fa->entries);
    strarray_fini(&fa->attribs);

    memset(fa, 0, sizeof(struct fetchargs));
}
