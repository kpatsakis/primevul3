static inline int arp_checkentry(const struct arpt_arp *arp)
{
	if (arp->flags & ~ARPT_F_MASK)
		return 0;
	if (arp->invflags & ~ARPT_INV_MASK)
		return 0;

	return 1;
}
