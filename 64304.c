ofproto_free_ofproto_controller_info(struct shash *info)
{
    connmgr_free_controller_info(info);
}
