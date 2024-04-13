ofproto_set_controllers(struct ofproto *p,
                        const struct ofproto_controller *controllers,
                        size_t n_controllers, uint32_t allowed_versions)
{
    connmgr_set_controllers(p->connmgr, controllers, n_controllers,
                            allowed_versions);
}
