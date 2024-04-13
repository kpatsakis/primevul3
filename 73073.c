tun_net_get_stats64(struct net_device *dev, struct rtnl_link_stats64 *stats)
{
	u32 rx_dropped = 0, tx_dropped = 0, rx_frame_errors = 0;
	struct tun_struct *tun = netdev_priv(dev);
	struct tun_pcpu_stats *p;
	int i;

	for_each_possible_cpu(i) {
		u64 rxpackets, rxbytes, txpackets, txbytes;
		unsigned int start;

		p = per_cpu_ptr(tun->pcpu_stats, i);
		do {
			start = u64_stats_fetch_begin(&p->syncp);
			rxpackets	= p->rx_packets;
			rxbytes		= p->rx_bytes;
			txpackets	= p->tx_packets;
			txbytes		= p->tx_bytes;
		} while (u64_stats_fetch_retry(&p->syncp, start));

		stats->rx_packets	+= rxpackets;
		stats->rx_bytes		+= rxbytes;
		stats->tx_packets	+= txpackets;
		stats->tx_bytes		+= txbytes;

		/* u32 counters */
		rx_dropped	+= p->rx_dropped;
		rx_frame_errors	+= p->rx_frame_errors;
		tx_dropped	+= p->tx_dropped;
	}
	stats->rx_dropped  = rx_dropped;
	stats->rx_frame_errors = rx_frame_errors;
	stats->tx_dropped = tx_dropped;
}
