static int cdrom_mrw_set_lba_space(struct cdrom_device_info *cdi, int space)
{
	struct packet_command cgc;
	struct mode_page_header *mph;
	char buffer[16];
	int ret, offset, size;

	init_cdrom_command(&cgc, buffer, sizeof(buffer), CGC_DATA_READ);

	cgc.buffer = buffer;
	cgc.buflen = sizeof(buffer);

	ret = cdrom_mode_sense(cdi, &cgc, cdi->mrw_mode_page, 0);
	if (ret)
		return ret;

	mph = (struct mode_page_header *)buffer;
	offset = be16_to_cpu(mph->desc_length);
	size = be16_to_cpu(mph->mode_data_length) + 2;

	buffer[offset + 3] = space;
	cgc.buflen = size;

	ret = cdrom_mode_select(cdi, &cgc);
	if (ret)
		return ret;

	pr_info("%s: mrw address space %s selected\n",
		cdi->name, mrw_address_space[space]);
	return 0;
}
