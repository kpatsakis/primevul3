static int __einj_error_trigger(u64 trigger_paddr, u32 type,
				u64 param1, u64 param2)
{
	struct acpi_einj_trigger *trigger_tab = NULL;
	struct apei_exec_context trigger_ctx;
	struct apei_resources trigger_resources;
	struct acpi_whea_header *trigger_entry;
	struct resource *r;
	u32 table_size;
	int rc = -EIO;
	struct acpi_generic_address *trigger_param_region = NULL;

	r = request_mem_region(trigger_paddr, sizeof(*trigger_tab),
			       "APEI EINJ Trigger Table");
	if (!r) {
		pr_err(EINJ_PFX
	"Can not request [mem %#010llx-%#010llx] for Trigger table\n",
		       (unsigned long long)trigger_paddr,
		       (unsigned long long)trigger_paddr +
			    sizeof(*trigger_tab) - 1);
		goto out;
	}
	trigger_tab = ioremap_cache(trigger_paddr, sizeof(*trigger_tab));
	if (!trigger_tab) {
		pr_err(EINJ_PFX "Failed to map trigger table!\n");
		goto out_rel_header;
	}
	rc = einj_check_trigger_header(trigger_tab);
	if (rc) {
		pr_warning(FW_BUG EINJ_PFX
			   "The trigger error action table is invalid\n");
		goto out_rel_header;
	}

	/* No action structures in the TRIGGER_ERROR table, nothing to do */
	if (!trigger_tab->entry_count)
		goto out_rel_header;

	rc = -EIO;
	table_size = trigger_tab->table_size;
	r = request_mem_region(trigger_paddr + sizeof(*trigger_tab),
			       table_size - sizeof(*trigger_tab),
			       "APEI EINJ Trigger Table");
	if (!r) {
		pr_err(EINJ_PFX
"Can not request [mem %#010llx-%#010llx] for Trigger Table Entry\n",
		       (unsigned long long)trigger_paddr + sizeof(*trigger_tab),
		       (unsigned long long)trigger_paddr + table_size - 1);
		goto out_rel_header;
	}
	iounmap(trigger_tab);
	trigger_tab = ioremap_cache(trigger_paddr, table_size);
	if (!trigger_tab) {
		pr_err(EINJ_PFX "Failed to map trigger table!\n");
		goto out_rel_entry;
	}
	trigger_entry = (struct acpi_whea_header *)
		((char *)trigger_tab + sizeof(struct acpi_einj_trigger));
	apei_resources_init(&trigger_resources);
	apei_exec_ctx_init(&trigger_ctx, einj_ins_type,
			   ARRAY_SIZE(einj_ins_type),
			   trigger_entry, trigger_tab->entry_count);
	rc = apei_exec_collect_resources(&trigger_ctx, &trigger_resources);
	if (rc)
		goto out_fini;
	rc = apei_resources_sub(&trigger_resources, &einj_resources);
	if (rc)
		goto out_fini;
	/*
	 * Some firmware will access target address specified in
	 * param1 to trigger the error when injecting memory error.
	 * This will cause resource conflict with regular memory.  So
	 * remove it from trigger table resources.
	 */
	if ((param_extension || acpi5) && (type & MEM_ERROR_MASK) && param2) {
		struct apei_resources addr_resources;
		apei_resources_init(&addr_resources);
		trigger_param_region = einj_get_trigger_parameter_region(
			trigger_tab, param1, param2);
		if (trigger_param_region) {
			rc = apei_resources_add(&addr_resources,
				trigger_param_region->address,
				trigger_param_region->bit_width/8, true);
			if (rc)
				goto out_fini;
			rc = apei_resources_sub(&trigger_resources,
					&addr_resources);
		}
		apei_resources_fini(&addr_resources);
		if (rc)
			goto out_fini;
	}
	rc = apei_resources_request(&trigger_resources, "APEI EINJ Trigger");
	if (rc)
		goto out_fini;
	rc = apei_exec_pre_map_gars(&trigger_ctx);
	if (rc)
		goto out_release;

	rc = apei_exec_run(&trigger_ctx, ACPI_EINJ_TRIGGER_ERROR);

	apei_exec_post_unmap_gars(&trigger_ctx);
out_release:
	apei_resources_release(&trigger_resources);
out_fini:
	apei_resources_fini(&trigger_resources);
out_rel_entry:
	release_mem_region(trigger_paddr + sizeof(*trigger_tab),
			   table_size - sizeof(*trigger_tab));
out_rel_header:
	release_mem_region(trigger_paddr, sizeof(*trigger_tab));
out:
	if (trigger_tab)
		iounmap(trigger_tab);

	return rc;
}
