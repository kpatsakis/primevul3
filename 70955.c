static int nfc_llcp_wks_sap(char *service_name, size_t service_name_len)
{
	int sap, num_wks;

	pr_debug("%s\n", service_name);

	if (service_name == NULL)
		return -EINVAL;

	num_wks = ARRAY_SIZE(wks);

	for (sap = 0; sap < num_wks; sap++) {
		if (wks[sap] == NULL)
			continue;

		if (strncmp(wks[sap], service_name, service_name_len) == 0)
			return sap;
	}

	return -EINVAL;
}
