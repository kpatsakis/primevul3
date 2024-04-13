reject_slave_controller(struct ofconn *ofconn)
{
    if (ofconn_get_type(ofconn) == OFCONN_PRIMARY
        && ofconn_get_role(ofconn) == OFPCR12_ROLE_SLAVE) {
        return OFPERR_OFPBRC_IS_SLAVE;
    } else {
        return 0;
    }
}
