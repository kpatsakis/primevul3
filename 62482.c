static int setup_initrd(struct boot_params *params,
		unsigned long initrd_load_addr, unsigned long initrd_len)
{
	params->hdr.ramdisk_image = initrd_load_addr & 0xffffffffUL;
	params->hdr.ramdisk_size = initrd_len & 0xffffffffUL;

	params->ext_ramdisk_image = initrd_load_addr >> 32;
	params->ext_ramdisk_size = initrd_len >> 32;

	return 0;
}
