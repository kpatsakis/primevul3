netdev_port_features_from_ofp11(ovs_be32 ofp11)
{
    return ntohl(ofp11) & 0xffff;
}
