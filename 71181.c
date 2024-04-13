static int get_smi_info(void *send_info, struct ipmi_smi_info *data)
{
	struct smi_info *smi = send_info;

	data->addr_src = smi->io.addr_source;
	data->dev = smi->io.dev;
	data->addr_info = smi->io.addr_info;
	get_device(smi->io.dev);

	return 0;
}
