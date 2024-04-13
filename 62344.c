static int __einj_get_available_error_type(u32 *type)
{
	struct apei_exec_context ctx;
	int rc;

	einj_exec_ctx_init(&ctx);
	rc = apei_exec_run(&ctx, ACPI_EINJ_GET_ERROR_TYPE);
	if (rc)
		return rc;
	*type = apei_exec_ctx_get_output(&ctx);

	return 0;
}
