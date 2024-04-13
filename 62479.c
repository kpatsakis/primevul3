prepare_add_efi_setup_data(struct boot_params *params,
		       unsigned long params_load_addr,
		       unsigned int efi_setup_data_offset)
{
	unsigned long setup_data_phys;
	struct setup_data *sd = (void *)params + efi_setup_data_offset;
	struct efi_setup_data *esd = (void *)sd + sizeof(struct setup_data);

	esd->fw_vendor = efi.fw_vendor;
	esd->runtime = efi.runtime;
	esd->tables = efi.config_table;
	esd->smbios = efi.smbios;

	sd->type = SETUP_EFI;
	sd->len = sizeof(struct efi_setup_data);

	/* Add setup data */
	setup_data_phys = params_load_addr + efi_setup_data_offset;
	sd->next = params->hdr.setup_data;
	params->hdr.setup_data = setup_data_phys;

	return 0;
}
