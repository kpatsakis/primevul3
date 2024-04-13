static int nfc_genl_get_device(struct sk_buff *skb, struct genl_info *info)
{
	struct sk_buff *msg;
	struct nfc_dev *dev;
	u32 idx;
	int rc = -ENOBUFS;

	if (!info->attrs[NFC_ATTR_DEVICE_INDEX])
		return -EINVAL;

	idx = nla_get_u32(info->attrs[NFC_ATTR_DEVICE_INDEX]);

	dev = nfc_get_device(idx);
	if (!dev)
		return -ENODEV;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg) {
		rc = -ENOMEM;
		goto out_putdev;
	}

	rc = nfc_genl_send_device(msg, dev, info->snd_portid, info->snd_seq,
				  NULL, 0);
	if (rc < 0)
		goto out_free;

	nfc_put_device(dev);

	return genlmsg_reply(msg, info);

out_free:
	nlmsg_free(msg);
out_putdev:
	nfc_put_device(dev);
	return rc;
}
