netdev_port_features_to_ofp10(enum netdev_features features)
{
    return htonl((features & 0x7f) | ((features & 0xf800) >> 4));
}
