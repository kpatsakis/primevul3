void init_cdrom_command(struct packet_command *cgc, void *buf, int len,
			int type)
{
	memset(cgc, 0, sizeof(struct packet_command));
	if (buf)
		memset(buf, 0, len);
	cgc->buffer = (char *) buf;
	cgc->buflen = len;
	cgc->data_direction = type;
	cgc->timeout = CDROM_DEF_TIMEOUT;
}
