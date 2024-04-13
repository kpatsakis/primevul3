megasas_dump_frame(void *mpi_request, int sz)
{
	int i;
	__le32 *mfp = (__le32 *)mpi_request;

	printk(KERN_INFO "IO request frame:\n\t");
	for (i = 0; i < sz / sizeof(__le32); i++) {
		if (i && ((i % 8) == 0))
			printk("\n\t");
		printk("%08x ", le32_to_cpu(mfp[i]));
	}
	printk("\n");
}
