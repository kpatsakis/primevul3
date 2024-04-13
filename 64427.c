make_echo_reply(const struct ofp_header *rq)
{
    struct ofpbuf rq_buf = ofpbuf_const_initializer(rq, ntohs(rq->length));
    ofpraw_pull_assert(&rq_buf);

    struct ofpbuf *reply = ofpraw_alloc_reply(OFPRAW_OFPT_ECHO_REPLY,
                                              rq, rq_buf.size);
    ofpbuf_put(reply, rq_buf.data, rq_buf.size);
    return reply;
}
