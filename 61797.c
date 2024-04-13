static jas_iccattrvalinfo_t *jas_iccattrvalinfo_lookup(jas_iccsig_t type)
{
	jas_iccattrvalinfo_t *info;
	info = jas_iccattrvalinfos;
	for (info = jas_iccattrvalinfos; info->type; ++info) {
		if (info->type == type) {
			return info;
		}
	}
	return 0;
}
