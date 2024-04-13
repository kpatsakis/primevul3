static int sas_get_phy_attached_dev(struct domain_device *dev, int phy_id,
				    u8 *sas_addr, enum sas_device_type *type)
{
	int res;
	struct smp_resp *disc_resp;
	struct discover_resp *dr;

	disc_resp = alloc_smp_resp(DISCOVER_RESP_SIZE);
	if (!disc_resp)
		return -ENOMEM;
	dr = &disc_resp->disc;

	res = sas_get_phy_discover(dev, phy_id, disc_resp);
	if (res == 0) {
		memcpy(sas_addr, disc_resp->disc.attached_sas_addr, 8);
		*type = to_dev_type(dr);
		if (*type == 0)
			memset(sas_addr, 0, 8);
	}
	kfree(disc_resp);
	return res;
}
