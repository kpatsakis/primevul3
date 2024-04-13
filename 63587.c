static int dvd_read_disckey(struct cdrom_device_info *cdi, dvd_struct *s,
				struct packet_command *cgc)
{
	int ret, size;
	u_char *buf;
	const struct cdrom_device_ops *cdo = cdi->ops;

	size = sizeof(s->disckey.value) + 4;

	buf = kmalloc(size, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	init_cdrom_command(cgc, buf, size, CGC_DATA_READ);
	cgc->cmd[0] = GPCMD_READ_DVD_STRUCTURE;
	cgc->cmd[7] = s->type;
	cgc->cmd[8] = size >> 8;
	cgc->cmd[9] = size & 0xff;
	cgc->cmd[10] = s->disckey.agid << 6;

	ret = cdo->generic_packet(cdi, cgc);
	if (!ret)
		memcpy(s->disckey.value, &buf[4], sizeof(s->disckey.value));

	kfree(buf);
	return ret;
}
