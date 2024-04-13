void sanitize_format(union cdrom_addr *addr,
		     u_char * curr, u_char requested)
{
	if (*curr == requested)
		return;                 /* nothing to be done! */
	if (requested == CDROM_LBA) {
		addr->lba = (int) addr->msf.frame +
			75 * (addr->msf.second - 2 + 60 * addr->msf.minute);
	} else {                        /* CDROM_MSF */
		int lba = addr->lba;
		addr->msf.frame = lba % 75;
		lba /= 75;
		lba += 2;
		addr->msf.second = lba % 60;
		addr->msf.minute = lba / 60;
	}
	*curr = requested;
}
