static struct ovl_entry *ovl_alloc_entry(void)
{
	return kzalloc(sizeof(struct ovl_entry), GFP_KERNEL);
}
