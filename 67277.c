int cdrom_dummy_generic_packet(struct cdrom_device_info *cdi,
			       struct packet_command *cgc)
{
	if (cgc->sense) {
		cgc->sense->sense_key = 0x05;
		cgc->sense->asc = 0x20;
		cgc->sense->ascq = 0x00;
	}

	cgc->stat = -EIO;
	return -EIO;
}
