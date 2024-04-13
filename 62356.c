static int error_type_set(void *data, u64 val)
{
	int rc;
	u32 available_error_type = 0;
	u32 tval, vendor;

	/*
	 * Vendor defined types have 0x80000000 bit set, and
	 * are not enumerated by ACPI_EINJ_GET_ERROR_TYPE
	 */
	vendor = val & ACPI5_VENDOR_BIT;
	tval = val & 0x7fffffff;

	/* Only one error type can be specified */
	if (tval & (tval - 1))
		return -EINVAL;
	if (!vendor) {
		rc = einj_get_available_error_type(&available_error_type);
		if (rc)
			return rc;
		if (!(val & available_error_type))
			return -EINVAL;
	}
	error_type = val;

	return 0;
}
