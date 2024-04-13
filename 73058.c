static int tun_get_coalesce(struct net_device *dev,
			    struct ethtool_coalesce *ec)
{
	struct tun_struct *tun = netdev_priv(dev);

	ec->rx_max_coalesced_frames = tun->rx_batched;

	return 0;
}
