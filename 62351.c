static void __exit einj_exit(void)
{
	struct apei_exec_context ctx;

	if (einj_param) {
		acpi_size size = (acpi5) ?
			sizeof(struct set_error_type_with_address) :
			sizeof(struct einj_parameter);

		acpi_os_unmap_iomem(einj_param, size);
	}
	einj_exec_ctx_init(&ctx);
	apei_exec_post_unmap_gars(&ctx);
	apei_resources_release(&einj_resources);
	apei_resources_fini(&einj_resources);
	debugfs_remove_recursive(einj_debug_dir);
}
