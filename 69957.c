static bool hsr_check_carrier(struct hsr_port *master)
{
	struct hsr_port *port;
	bool has_carrier;

	has_carrier = false;

	rcu_read_lock();
	hsr_for_each_port(master->hsr, port)
		if ((port->type != HSR_PT_MASTER) && is_slave_up(port->dev)) {
			has_carrier = true;
			break;
		}
	rcu_read_unlock();

	if (has_carrier)
		netif_carrier_on(master->dev);
	else
		netif_carrier_off(master->dev);

	return has_carrier;
}
