static int __init einj_init(void)
{
	int rc;
	acpi_status status;
	struct dentry *fentry;
	struct apei_exec_context ctx;

	if (acpi_disabled)
		return -ENODEV;

	status = acpi_get_table(ACPI_SIG_EINJ, 0,
				(struct acpi_table_header **)&einj_tab);
	if (status == AE_NOT_FOUND)
		return -ENODEV;
	else if (ACPI_FAILURE(status)) {
		const char *msg = acpi_format_exception(status);
		pr_err(EINJ_PFX "Failed to get table, %s\n", msg);
		return -EINVAL;
	}

	rc = einj_check_table(einj_tab);
	if (rc) {
		pr_warning(FW_BUG EINJ_PFX "EINJ table is invalid\n");
		return -EINVAL;
	}

	rc = -ENOMEM;
	einj_debug_dir = debugfs_create_dir("einj", apei_get_debugfs_dir());
	if (!einj_debug_dir)
		goto err_cleanup;
	fentry = debugfs_create_file("available_error_type", S_IRUSR,
				     einj_debug_dir, NULL,
				     &available_error_type_fops);
	if (!fentry)
		goto err_cleanup;
	fentry = debugfs_create_file("error_type", S_IRUSR | S_IWUSR,
				     einj_debug_dir, NULL, &error_type_fops);
	if (!fentry)
		goto err_cleanup;
	fentry = debugfs_create_file("error_inject", S_IWUSR,
				     einj_debug_dir, NULL, &error_inject_fops);
	if (!fentry)
		goto err_cleanup;

	apei_resources_init(&einj_resources);
	einj_exec_ctx_init(&ctx);
	rc = apei_exec_collect_resources(&ctx, &einj_resources);
	if (rc)
		goto err_fini;
	rc = apei_resources_request(&einj_resources, "APEI EINJ");
	if (rc)
		goto err_fini;
	rc = apei_exec_pre_map_gars(&ctx);
	if (rc)
		goto err_release;

	rc = -ENOMEM;
	einj_param = einj_get_parameter_address();
	if ((param_extension || acpi5) && einj_param) {
		fentry = debugfs_create_x32("flags", S_IRUSR | S_IWUSR,
					    einj_debug_dir, &error_flags);
		if (!fentry)
			goto err_unmap;
		fentry = debugfs_create_x64("param1", S_IRUSR | S_IWUSR,
					    einj_debug_dir, &error_param1);
		if (!fentry)
			goto err_unmap;
		fentry = debugfs_create_x64("param2", S_IRUSR | S_IWUSR,
					    einj_debug_dir, &error_param2);
		if (!fentry)
			goto err_unmap;
		fentry = debugfs_create_x64("param3", S_IRUSR | S_IWUSR,
					    einj_debug_dir, &error_param3);
		if (!fentry)
			goto err_unmap;
		fentry = debugfs_create_x64("param4", S_IRUSR | S_IWUSR,
					    einj_debug_dir, &error_param4);
		if (!fentry)
			goto err_unmap;

		fentry = debugfs_create_x32("notrigger", S_IRUSR | S_IWUSR,
					    einj_debug_dir, &notrigger);
		if (!fentry)
			goto err_unmap;
	}

	if (vendor_dev[0]) {
		vendor_blob.data = vendor_dev;
		vendor_blob.size = strlen(vendor_dev);
		fentry = debugfs_create_blob("vendor", S_IRUSR,
					     einj_debug_dir, &vendor_blob);
		if (!fentry)
			goto err_unmap;
		fentry = debugfs_create_x32("vendor_flags", S_IRUSR | S_IWUSR,
					    einj_debug_dir, &vendor_flags);
		if (!fentry)
			goto err_unmap;
	}

	pr_info(EINJ_PFX "Error INJection is initialized.\n");

	return 0;

err_unmap:
	if (einj_param) {
		acpi_size size = (acpi5) ?
			sizeof(struct set_error_type_with_address) :
			sizeof(struct einj_parameter);

		acpi_os_unmap_iomem(einj_param, size);
	}
	apei_exec_post_unmap_gars(&ctx);
err_release:
	apei_resources_release(&einj_resources);
err_fini:
	apei_resources_fini(&einj_resources);
err_cleanup:
	debugfs_remove_recursive(einj_debug_dir);

	return rc;
}
