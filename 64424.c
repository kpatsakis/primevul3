get_ofp14_async_config_prop_by_oam(enum ofputil_async_msg_type oam,
                                   bool master)
{
    FOR_EACH_ASYNC_PROP (ap) {
        if (ap->oam == oam && ap->master == master) {
            return ap;
        }
    }
    return NULL;
}
