static const GUIDParseTable *find_guid(ff_asf_guid guid)
{
    int j, ret;
    const GUIDParseTable *g;

    swap_guid(guid);
    g = gdef;
    for (j = 0; j < FF_ARRAY_ELEMS(gdef); j++) {
        if (!(ret = memcmp(guid, g->guid, sizeof(g->guid))))
            return g;
        g++;
    }

    return NULL;
}
