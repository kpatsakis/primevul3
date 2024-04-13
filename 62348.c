static int einj_check_table(struct acpi_table_einj *einj_tab)
{
	if ((einj_tab->header_length !=
	     (sizeof(struct acpi_table_einj) - sizeof(einj_tab->header)))
	    && (einj_tab->header_length != sizeof(struct acpi_table_einj)))
		return -EINVAL;
	if (einj_tab->header.length < sizeof(struct acpi_table_einj))
		return -EINVAL;
	if (einj_tab->entries !=
	    (einj_tab->header.length - sizeof(struct acpi_table_einj)) /
	    sizeof(struct acpi_einj_entry))
		return -EINVAL;

	return 0;
}
