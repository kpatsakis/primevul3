static int dvd_read_physical(struct cdrom_device_info *cdi, dvd_struct *s,
				struct packet_command *cgc)
{
	unsigned char buf[21], *base;
	struct dvd_layer *layer;
	const struct cdrom_device_ops *cdo = cdi->ops;
	int ret, layer_num = s->physical.layer_num;

	if (layer_num >= DVD_LAYERS)
		return -EINVAL;

	init_cdrom_command(cgc, buf, sizeof(buf), CGC_DATA_READ);
	cgc->cmd[0] = GPCMD_READ_DVD_STRUCTURE;
	cgc->cmd[6] = layer_num;
	cgc->cmd[7] = s->type;
	cgc->cmd[9] = cgc->buflen & 0xff;

	/*
	 * refrain from reporting errors on non-existing layers (mainly)
	 */
	cgc->quiet = 1;

	ret = cdo->generic_packet(cdi, cgc);
	if (ret)
		return ret;

	base = &buf[4];
	layer = &s->physical.layer[layer_num];

	/*
	 * place the data... really ugly, but at least we won't have to
	 * worry about endianess in userspace.
	 */
	memset(layer, 0, sizeof(*layer));
	layer->book_version = base[0] & 0xf;
	layer->book_type = base[0] >> 4;
	layer->min_rate = base[1] & 0xf;
	layer->disc_size = base[1] >> 4;
	layer->layer_type = base[2] & 0xf;
	layer->track_path = (base[2] >> 4) & 1;
	layer->nlayers = (base[2] >> 5) & 3;
	layer->track_density = base[3] & 0xf;
	layer->linear_density = base[3] >> 4;
	layer->start_sector = base[5] << 16 | base[6] << 8 | base[7];
	layer->end_sector = base[9] << 16 | base[10] << 8 | base[11];
	layer->end_sector_l0 = base[13] << 16 | base[14] << 8 | base[15];
	layer->bca = base[16] >> 7;

	return 0;
}
