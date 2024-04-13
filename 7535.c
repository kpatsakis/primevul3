static void ccp_get_dm_area(struct ccp_dm_workarea *wa, unsigned int wa_offset,
			    struct scatterlist *sg, unsigned int sg_offset,
			    unsigned int len)
{
	WARN_ON(!wa->address);

	scatterwalk_map_and_copy(wa->address + wa_offset, sg, sg_offset, len,
				 1);
}