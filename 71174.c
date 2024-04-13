static void cleanup_one_si(struct smi_info *smi_info)
{
	if (!smi_info)
		return;

	list_del(&smi_info->link);

	if (smi_info->intf)
		ipmi_unregister_smi(smi_info->intf);

	if (smi_info->pdev) {
		if (smi_info->pdev_registered)
			platform_device_unregister(smi_info->pdev);
		else
			platform_device_put(smi_info->pdev);
	}

	kfree(smi_info);
}
