int __init reserve_elfcorehdr(unsigned long *start, unsigned long *end)
{
	unsigned long length;

	/* We get the address using the kernel command line,
	 * but the size is extracted from the EFI tables.
	 * Both address and size are required for reservation
	 * to work properly.
	 */

	if (elfcorehdr_addr >= ELFCORE_ADDR_MAX)
		return -EINVAL;

	if ((length = vmcore_find_descriptor_size(elfcorehdr_addr)) == 0) {
		elfcorehdr_addr = ELFCORE_ADDR_MAX;
		return -EINVAL;
	}

	*start = (unsigned long)__va(elfcorehdr_addr);
	*end = *start + length;
	return 0;
}
