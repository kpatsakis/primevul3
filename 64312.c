ofproto_get_netflow_ids(const struct ofproto *ofproto,
                        uint8_t *engine_type, uint8_t *engine_id)
{
    ofproto->ofproto_class->get_netflow_ids(ofproto, engine_type, engine_id);
}
