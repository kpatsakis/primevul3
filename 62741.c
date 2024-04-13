io_port_init (void)
{
	unsigned long phys_iobase;

	/*
	 * Set `iobase' based on the EFI memory map or, failing that, the
	 * value firmware left in ar.k0.
	 *
	 * Note that in ia32 mode, IN/OUT instructions use ar.k0 to compute
	 * the port's virtual address, so ia32_load_state() loads it with a
	 * user virtual address.  But in ia64 mode, glibc uses the
	 * *physical* address in ar.k0 to mmap the appropriate area from
	 * /dev/mem, and the inX()/outX() interfaces use MMIO.  In both
	 * cases, user-mode can only use the legacy 0-64K I/O port space.
	 *
	 * ar.k0 is not involved in kernel I/O port accesses, which can use
	 * any of the I/O port spaces and are done via MMIO using the
	 * virtual mmio_base from the appropriate io_space[].
	 */
	phys_iobase = efi_get_iobase();
	if (!phys_iobase) {
		phys_iobase = ia64_get_kr(IA64_KR_IO_BASE);
		printk(KERN_INFO "No I/O port range found in EFI memory map, "
			"falling back to AR.KR0 (0x%lx)\n", phys_iobase);
	}
	ia64_iobase = (unsigned long) ioremap(phys_iobase, 0);
	ia64_set_kr(IA64_KR_IO_BASE, __pa(ia64_iobase));

	/* setup legacy IO port space */
	io_space[0].mmio_base = ia64_iobase;
	io_space[0].sparse = 1;
	num_io_spaces = 1;
}
