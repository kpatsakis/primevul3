static void setup_dell_poweredge_oem_data_handler(struct smi_info *smi_info)
{
	struct ipmi_device_id *id = &smi_info->device_id;
	if (id->manufacturer_id == DELL_IANA_MFR_ID) {
		if (id->device_id       == DELL_POWEREDGE_8G_BMC_DEVICE_ID  &&
		    id->device_revision == DELL_POWEREDGE_8G_BMC_DEVICE_REV &&
		    id->ipmi_version   == DELL_POWEREDGE_8G_BMC_IPMI_VERSION) {
			smi_info->oem_data_avail_handler =
				oem_data_avail_to_receive_msg_avail;
		} else if (ipmi_version_major(id) < 1 ||
			   (ipmi_version_major(id) == 1 &&
			    ipmi_version_minor(id) < 5)) {
			smi_info->oem_data_avail_handler =
				oem_data_avail_to_receive_msg_avail;
		}
	}
}
