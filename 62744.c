static int __init register_memory(void)
{
	code_resource.start = ia64_tpa(_text);
	code_resource.end   = ia64_tpa(_etext) - 1;
	data_resource.start = ia64_tpa(_etext);
	data_resource.end   = ia64_tpa(_edata) - 1;
	bss_resource.start  = ia64_tpa(__bss_start);
	bss_resource.end    = ia64_tpa(_end) - 1;
	efi_initialize_iomem_resources(&code_resource, &data_resource,
			&bss_resource);

	return 0;
}
