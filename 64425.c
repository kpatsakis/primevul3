get_ofp14_async_config_prop_by_prop_type(uint64_t prop_type)
{
    FOR_EACH_ASYNC_PROP (ap) {
        if (prop_type == ap->prop_type) {
            return ap;
        }
    }
    return NULL;
}
