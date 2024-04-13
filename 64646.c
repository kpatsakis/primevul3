ofputil_pull_queue_get_config_reply(struct ofpbuf *msg,
                                    struct ofputil_queue_config *queue)
{
    enum ofpraw raw;
    if (!msg->header) {
        /* Pull OpenFlow header. */
        raw = ofpraw_pull_assert(msg);

        /* Pull protocol-specific ofp_queue_get_config_reply header (OF1.4
         * doesn't have one at all). */
        if (raw == OFPRAW_OFPT10_QUEUE_GET_CONFIG_REPLY) {
            ofpbuf_pull(msg, sizeof(struct ofp10_queue_get_config_reply));
        } else if (raw == OFPRAW_OFPT11_QUEUE_GET_CONFIG_REPLY) {
            ofpbuf_pull(msg, sizeof(struct ofp11_queue_get_config_reply));
        } else {
            ovs_assert(raw == OFPRAW_OFPST14_QUEUE_DESC_REPLY);
        }
    } else {
        raw = ofpraw_decode_assert(msg->header);
    }

    queue->min_rate = UINT16_MAX;
    queue->max_rate = UINT16_MAX;

    if (!msg->size) {
        return EOF;
    } else if (raw == OFPRAW_OFPST14_QUEUE_DESC_REPLY) {
        return ofputil_pull_queue_get_config_reply14(msg, queue);
    } else {
        return ofputil_pull_queue_get_config_reply10(msg, queue);
    }
}
