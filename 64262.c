handle_role_request(struct ofconn *ofconn, const struct ofp_header *oh)
{
    struct ofputil_role_request request;
    struct ofputil_role_request reply;
    struct ofpbuf *buf;
    enum ofperr error;

    error = ofputil_decode_role_message(oh, &request);
    if (error) {
        return error;
    }

    if (request.role != OFPCR12_ROLE_NOCHANGE) {
        if (request.role != OFPCR12_ROLE_EQUAL
            && request.have_generation_id
            && !ofconn_set_master_election_id(ofconn, request.generation_id)) {
                return OFPERR_OFPRRFC_STALE;
        }

        ofconn_set_role(ofconn, request.role);
    }

    reply.role = ofconn_get_role(ofconn);
    reply.have_generation_id = ofconn_get_master_election_id(
        ofconn, &reply.generation_id);
    buf = ofputil_encode_role_reply(oh, &reply);
    ofconn_send_reply(ofconn, buf);

    return 0;
}
