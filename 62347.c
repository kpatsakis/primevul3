static void check_vendor_extension(u64 paddr,
				   struct set_error_type_with_address *v5param)
{
	int	offset = v5param->vendor_extension;
	struct	vendor_error_type_extension *v;
	u32	sbdf;

	if (!offset)
		return;
	v = acpi_os_map_iomem(paddr + offset, sizeof(*v));
	if (!v)
		return;
	sbdf = v->pcie_sbdf;
	sprintf(vendor_dev, "%x:%x:%x.%x vendor_id=%x device_id=%x rev_id=%x\n",
		sbdf >> 24, (sbdf >> 16) & 0xff,
		(sbdf >> 11) & 0x1f, (sbdf >> 8) & 0x7,
		 v->vendor_id, v->device_id, v->rev_id);
	acpi_os_unmap_iomem(v, sizeof(*v));
}
