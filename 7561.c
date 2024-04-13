static int ccp_set_dm_area(struct ccp_dm_workarea *wa, unsigned int wa_offset,
			   struct scatterlist *sg, unsigned int sg_offset,
			   unsigned int len)
{
	WARN_ON(!wa->address);

	if (len > (wa->length - wa_offset))
		return -EINVAL;

	scatterwalk_map_and_copy(wa->address + wa_offset, sg, sg_offset, len,
				 0);
	return 0;
}