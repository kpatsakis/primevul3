netdev_port_features_to_ofp11(enum netdev_features features)
{
    return htonl(features & 0xffff);
}
