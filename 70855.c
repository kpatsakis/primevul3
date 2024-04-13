static int nfc_genl_start_poll(struct sk_buff *skb, struct genl_info *info)
{
	struct nfc_dev *dev;
	int rc;
	u32 idx;
	u32 im_protocols = 0, tm_protocols = 0;

	pr_debug("Poll start\n");

	if (!info->attrs[NFC_ATTR_DEVICE_INDEX] ||
	    ((!info->attrs[NFC_ATTR_IM_PROTOCOLS] &&
	      !info->attrs[NFC_ATTR_PROTOCOLS]) &&
	      !info->attrs[NFC_ATTR_TM_PROTOCOLS]))
		return -EINVAL;

	idx = nla_get_u32(info->attrs[NFC_ATTR_DEVICE_INDEX]);

	if (info->attrs[NFC_ATTR_TM_PROTOCOLS])
		tm_protocols = nla_get_u32(info->attrs[NFC_ATTR_TM_PROTOCOLS]);

	if (info->attrs[NFC_ATTR_IM_PROTOCOLS])
		im_protocols = nla_get_u32(info->attrs[NFC_ATTR_IM_PROTOCOLS]);
	else if (info->attrs[NFC_ATTR_PROTOCOLS])
		im_protocols = nla_get_u32(info->attrs[NFC_ATTR_PROTOCOLS]);

	dev = nfc_get_device(idx);
	if (!dev)
		return -ENODEV;

	mutex_lock(&dev->genl_data.genl_data_mutex);

	rc = nfc_start_poll(dev, im_protocols, tm_protocols);
	if (!rc)
		dev->genl_data.poll_req_portid = info->snd_portid;

	mutex_unlock(&dev->genl_data.genl_data_mutex);

	nfc_put_device(dev);
	return rc;
}
