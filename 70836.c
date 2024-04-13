static int nfc_genl_dump_targets_done(struct netlink_callback *cb)
{
	struct nfc_dev *dev = (struct nfc_dev *) cb->args[1];

	if (dev)
		nfc_put_device(dev);

	return 0;
}
