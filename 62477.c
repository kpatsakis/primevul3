static int bzImage64_probe(const char *buf, unsigned long len)
{
	int ret = -ENOEXEC;
	struct setup_header *header;

	/* kernel should be at least two sectors long */
	if (len < 2 * 512) {
		pr_err("File is too short to be a bzImage\n");
		return ret;
	}

	header = (struct setup_header *)(buf + offsetof(struct boot_params, hdr));
	if (memcmp((char *)&header->header, "HdrS", 4) != 0) {
		pr_err("Not a bzImage\n");
		return ret;
	}

	if (header->boot_flag != 0xAA55) {
		pr_err("No x86 boot sector present\n");
		return ret;
	}

	if (header->version < 0x020C) {
		pr_err("Must be at least protocol version 2.12\n");
		return ret;
	}

	if (!(header->loadflags & LOADED_HIGH)) {
		pr_err("zImage not a bzImage\n");
		return ret;
	}

	if (!(header->xloadflags & XLF_KERNEL_64)) {
		pr_err("Not a bzImage64. XLF_KERNEL_64 is not set.\n");
		return ret;
	}

	if (!(header->xloadflags & XLF_CAN_BE_LOADED_ABOVE_4G)) {
		pr_err("XLF_CAN_BE_LOADED_ABOVE_4G is not set.\n");
		return ret;
	}

	/*
	 * Can't handle 32bit EFI as it does not allow loading kernel
	 * above 4G. This should be handled by 32bit bzImage loader
	 */
	if (efi_enabled(EFI_RUNTIME_SERVICES) && !efi_enabled(EFI_64BIT)) {
		pr_debug("EFI is 32 bit. Can't load kernel above 4G.\n");
		return ret;
	}

	/* I've got a bzImage */
	pr_debug("It's a relocatable bzImage64\n");
	ret = 0;

	return ret;
}
