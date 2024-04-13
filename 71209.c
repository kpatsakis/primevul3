setup_dell_poweredge_bt_xaction_handler(struct smi_info *smi_info)
{
	struct ipmi_device_id *id = &smi_info->device_id;
	if (id->manufacturer_id == DELL_IANA_MFR_ID &&
	    smi_info->io.si_type == SI_BT)
		register_xaction_notifier(&dell_poweredge_bt_xaction_notifier);
}
