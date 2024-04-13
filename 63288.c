struct usb_host_interface *usb_find_alt_setting(
		struct usb_host_config *config,
		unsigned int iface_num,
		unsigned int alt_num)
{
	struct usb_interface_cache *intf_cache = NULL;
	int i;

	if (!config)
		return NULL;
	for (i = 0; i < config->desc.bNumInterfaces; i++) {
		if (config->intf_cache[i]->altsetting[0].desc.bInterfaceNumber
				== iface_num) {
			intf_cache = config->intf_cache[i];
			break;
		}
	}
	if (!intf_cache)
		return NULL;
	for (i = 0; i < intf_cache->num_altsetting; i++)
		if (intf_cache->altsetting[i].desc.bAlternateSetting == alt_num)
			return &intf_cache->altsetting[i];

	printk(KERN_DEBUG "Did not find alt setting %u for intf %u, "
			"config %u\n", alt_num, iface_num,
			config->desc.bConfigurationValue);
	return NULL;
}
