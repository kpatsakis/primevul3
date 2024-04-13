handle_nxt_set_controller_id(struct ofconn *ofconn,
                             const struct ofp_header *oh)
{
    const struct nx_controller_id *nci = ofpmsg_body(oh);

    if (!is_all_zeros(nci->zero, sizeof nci->zero)) {
        return OFPERR_NXBRC_MUST_BE_ZERO;
    }

    ofconn_set_controller_id(ofconn, ntohs(nci->controller_id));
    return 0;
}
