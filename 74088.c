static int set_haschildren(const mbentry_t *mbentry __attribute__((unused)),
                           void *rock)
{
    uint32_t *attributes = (uint32_t *)rock;
    list_callback_calls++;
    *attributes |= MBOX_ATTRIBUTE_HASCHILDREN;
    return CYRUSDB_DONE;
}
