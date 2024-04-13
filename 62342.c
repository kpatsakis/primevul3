static int __einj_error_inject(u32 type, u32 flags, u64 param1, u64 param2,
			       u64 param3, u64 param4)
{
	struct apei_exec_context ctx;
	u64 val, trigger_paddr, timeout = FIRMWARE_TIMEOUT;
	int rc;

	einj_exec_ctx_init(&ctx);

	rc = apei_exec_run_optional(&ctx, ACPI_EINJ_BEGIN_OPERATION);
	if (rc)
		return rc;
	apei_exec_ctx_set_input(&ctx, type);
	if (acpi5) {
		struct set_error_type_with_address *v5param = einj_param;

		v5param->type = type;
		if (type & ACPI5_VENDOR_BIT) {
			switch (vendor_flags) {
			case SETWA_FLAGS_APICID:
				v5param->apicid = param1;
				break;
			case SETWA_FLAGS_MEM:
				v5param->memory_address = param1;
				v5param->memory_address_range = param2;
				break;
			case SETWA_FLAGS_PCIE_SBDF:
				v5param->pcie_sbdf = param1;
				break;
			}
			v5param->flags = vendor_flags;
		} else if (flags) {
				v5param->flags = flags;
				v5param->memory_address = param1;
				v5param->memory_address_range = param2;
				v5param->apicid = param3;
				v5param->pcie_sbdf = param4;
		} else {
			switch (type) {
			case ACPI_EINJ_PROCESSOR_CORRECTABLE:
			case ACPI_EINJ_PROCESSOR_UNCORRECTABLE:
			case ACPI_EINJ_PROCESSOR_FATAL:
				v5param->apicid = param1;
				v5param->flags = SETWA_FLAGS_APICID;
				break;
			case ACPI_EINJ_MEMORY_CORRECTABLE:
			case ACPI_EINJ_MEMORY_UNCORRECTABLE:
			case ACPI_EINJ_MEMORY_FATAL:
				v5param->memory_address = param1;
				v5param->memory_address_range = param2;
				v5param->flags = SETWA_FLAGS_MEM;
				break;
			case ACPI_EINJ_PCIX_CORRECTABLE:
			case ACPI_EINJ_PCIX_UNCORRECTABLE:
			case ACPI_EINJ_PCIX_FATAL:
				v5param->pcie_sbdf = param1;
				v5param->flags = SETWA_FLAGS_PCIE_SBDF;
				break;
			}
		}
	} else {
		rc = apei_exec_run(&ctx, ACPI_EINJ_SET_ERROR_TYPE);
		if (rc)
			return rc;
		if (einj_param) {
			struct einj_parameter *v4param = einj_param;
			v4param->param1 = param1;
			v4param->param2 = param2;
		}
	}
	rc = apei_exec_run(&ctx, ACPI_EINJ_EXECUTE_OPERATION);
	if (rc)
		return rc;
	for (;;) {
		rc = apei_exec_run(&ctx, ACPI_EINJ_CHECK_BUSY_STATUS);
		if (rc)
			return rc;
		val = apei_exec_ctx_get_output(&ctx);
		if (!(val & EINJ_OP_BUSY))
			break;
		if (einj_timedout(&timeout))
			return -EIO;
	}
	rc = apei_exec_run(&ctx, ACPI_EINJ_GET_COMMAND_STATUS);
	if (rc)
		return rc;
	val = apei_exec_ctx_get_output(&ctx);
	if (val != EINJ_STATUS_SUCCESS)
		return -EBUSY;

	rc = apei_exec_run(&ctx, ACPI_EINJ_GET_TRIGGER_TABLE);
	if (rc)
		return rc;
	trigger_paddr = apei_exec_ctx_get_output(&ctx);
	if (notrigger == 0) {
		rc = __einj_error_trigger(trigger_paddr, type, param1, param2);
		if (rc)
			return rc;
	}
	rc = apei_exec_run_optional(&ctx, ACPI_EINJ_END_OPERATION);

	return rc;
}
