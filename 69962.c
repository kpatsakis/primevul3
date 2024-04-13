static int hsr_dev_open(struct net_device *dev)
{
	struct hsr_priv *hsr;
	struct hsr_port *port;
	char designation;

	hsr = netdev_priv(dev);
	designation = '\0';

	rcu_read_lock();
	hsr_for_each_port(hsr, port) {
		if (port->type == HSR_PT_MASTER)
			continue;
		switch (port->type) {
		case HSR_PT_SLAVE_A:
			designation = 'A';
			break;
		case HSR_PT_SLAVE_B:
			designation = 'B';
			break;
		default:
			designation = '?';
		}
		if (!is_slave_up(port->dev))
			netdev_warn(dev, "Slave %c (%s) is not up; please bring it up to get a fully working HSR network\n",
				    designation, port->dev->name);
	}
	rcu_read_unlock();

	if (designation == '\0')
		netdev_warn(dev, "No slave devices configured\n");

	return 0;
}
