static void hsr_set_operstate(struct hsr_port *master, bool has_carrier)
{
	if (!is_admin_up(master->dev)) {
		__hsr_set_operstate(master->dev, IF_OPER_DOWN);
		return;
	}

	if (has_carrier)
		__hsr_set_operstate(master->dev, IF_OPER_UP);
	else
		__hsr_set_operstate(master->dev, IF_OPER_LOWERLAYERDOWN);
}
