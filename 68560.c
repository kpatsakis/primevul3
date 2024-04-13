ip_checkentry(const struct ipt_ip *ip)
{
	if (ip->flags & ~IPT_F_MASK)
		return false;
	if (ip->invflags & ~IPT_INV_MASK)
		return false;
	return true;
}
