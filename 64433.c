netdev_port_features_from_ofp10(ovs_be32 ofp10_)
{
    uint32_t ofp10 = ntohl(ofp10_);
    return (ofp10 & 0x7f) | ((ofp10 & 0xf80) << 4);
}
