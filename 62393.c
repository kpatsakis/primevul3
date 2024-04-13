static void os9_to_unix_permissions(LHAFileHeader *header)
{
	unsigned int or, ow, oe, pr, pw, pe, d;


	or = (header->os9_perms & 0x01) != 0;
	ow = (header->os9_perms & 0x02) != 0;
	oe = (header->os9_perms & 0x04) != 0;
	pr = (header->os9_perms & 0x08) != 0;
	pw = (header->os9_perms & 0x10) != 0;
	pe = (header->os9_perms & 0x20) != 0;
	d = (header->os9_perms & 0x80) != 0;

	header->extra_flags |= LHA_FILE_UNIX_PERMS;
	header->unix_perms = (d << 14)
	                   | (or << 8) | (ow << 7) | (oe << 6)  // owner
	                   | (pr << 5) | (pw << 4) | (pe << 3)  // group
	                   | (pr << 2) | (pw << 1) | (pe << 0); // everyone
}
