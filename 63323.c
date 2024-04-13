static int wa_fill_descr(struct wahc *wa)
{
	int result;
	struct device *dev = &wa->usb_iface->dev;
	char *itr;
	struct usb_device *usb_dev = wa->usb_dev;
	struct usb_descriptor_header *hdr;
	struct usb_wa_descriptor *wa_descr;
	size_t itr_size, actconfig_idx;

	actconfig_idx = (usb_dev->actconfig - usb_dev->config) /
			sizeof(usb_dev->config[0]);
	itr = usb_dev->rawdescriptors[actconfig_idx];
	itr_size = le16_to_cpu(usb_dev->actconfig->desc.wTotalLength);
	while (itr_size >= sizeof(*hdr)) {
		hdr = (struct usb_descriptor_header *) itr;
		dev_dbg(dev, "Extra device descriptor: "
			"type %02x/%u bytes @ %zu (%zu left)\n",
			hdr->bDescriptorType, hdr->bLength,
			(itr - usb_dev->rawdescriptors[actconfig_idx]),
			itr_size);
		if (hdr->bDescriptorType == USB_DT_WIRE_ADAPTER)
			goto found;
		itr += hdr->bLength;
		itr_size -= hdr->bLength;
	}
	dev_err(dev, "cannot find Wire Adapter Class descriptor\n");
	return -ENODEV;

found:
	result = -EINVAL;
	if (hdr->bLength > itr_size) {	/* is it available? */
		dev_err(dev, "incomplete Wire Adapter Class descriptor "
			"(%zu bytes left, %u needed)\n",
			itr_size, hdr->bLength);
		goto error;
	}
	if (hdr->bLength < sizeof(*wa->wa_descr)) {
		dev_err(dev, "short Wire Adapter Class descriptor\n");
		goto error;
	}
	wa->wa_descr = wa_descr = (struct usb_wa_descriptor *) hdr;
	if (le16_to_cpu(wa_descr->bcdWAVersion) > 0x0100)
		dev_warn(dev, "Wire Adapter v%d.%d newer than groked v1.0\n",
			 (le16_to_cpu(wa_descr->bcdWAVersion) & 0xff00) >> 8,
			 le16_to_cpu(wa_descr->bcdWAVersion) & 0x00ff);
	result = 0;
error:
	return result;
}
