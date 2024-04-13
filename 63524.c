int cdrom_get_media_event(struct cdrom_device_info *cdi,
			  struct media_event_desc *med)
{
	struct packet_command cgc;
	unsigned char buffer[8];
	struct event_header *eh = (struct event_header *)buffer;

	init_cdrom_command(&cgc, buffer, sizeof(buffer), CGC_DATA_READ);
	cgc.cmd[0] = GPCMD_GET_EVENT_STATUS_NOTIFICATION;
	cgc.cmd[1] = 1;		/* IMMED */
	cgc.cmd[4] = 1 << 4;	/* media event */
	cgc.cmd[8] = sizeof(buffer);
	cgc.quiet = 1;

	if (cdi->ops->generic_packet(cdi, &cgc))
		return 1;

	if (be16_to_cpu(eh->data_len) < sizeof(*med))
		return 1;

	if (eh->nea || eh->notification_class != 0x4)
		return 1;

	memcpy(med, &buffer[sizeof(*eh)], sizeof(*med));
	return 0;
}
