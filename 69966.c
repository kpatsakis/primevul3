int hsr_get_max_mtu(struct hsr_priv *hsr)
{
	unsigned int mtu_max;
	struct hsr_port *port;

	mtu_max = ETH_DATA_LEN;
	rcu_read_lock();
	hsr_for_each_port(hsr, port)
		if (port->type != HSR_PT_MASTER)
			mtu_max = min(port->dev->mtu, mtu_max);
	rcu_read_unlock();

	if (mtu_max < HSR_HLEN)
		return 0;
	return mtu_max - HSR_HLEN;
}
