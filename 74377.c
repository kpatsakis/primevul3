static int hidg_bind(struct usb_configuration *c, struct usb_function *f)
{
	struct usb_ep		*ep;
	struct f_hidg		*hidg = func_to_hidg(f);
	struct usb_string	*us;
	struct device		*device;
	int			status;
	dev_t			dev;

	/* maybe allocate device-global string IDs, and patch descriptors */
	us = usb_gstrings_attach(c->cdev, ct_func_strings,
				 ARRAY_SIZE(ct_func_string_defs));
	if (IS_ERR(us))
		return PTR_ERR(us);
	hidg_interface_desc.iInterface = us[CT_FUNC_HID_IDX].id;

	/* allocate instance-specific interface IDs, and patch descriptors */
	status = usb_interface_id(c, f);
	if (status < 0)
		goto fail;
	hidg_interface_desc.bInterfaceNumber = status;

	/* allocate instance-specific endpoints */
	status = -ENODEV;
	ep = usb_ep_autoconfig(c->cdev->gadget, &hidg_fs_in_ep_desc);
	if (!ep)
		goto fail;
	hidg->in_ep = ep;

	ep = usb_ep_autoconfig(c->cdev->gadget, &hidg_fs_out_ep_desc);
	if (!ep)
		goto fail;
	hidg->out_ep = ep;

	/* set descriptor dynamic values */
	hidg_interface_desc.bInterfaceSubClass = hidg->bInterfaceSubClass;
	hidg_interface_desc.bInterfaceProtocol = hidg->bInterfaceProtocol;
	hidg->protocol = HID_REPORT_PROTOCOL;
	hidg_ss_in_ep_desc.wMaxPacketSize = cpu_to_le16(hidg->report_length);
	hidg_ss_in_comp_desc.wBytesPerInterval =
				cpu_to_le16(hidg->report_length);
	hidg_hs_in_ep_desc.wMaxPacketSize = cpu_to_le16(hidg->report_length);
	hidg_fs_in_ep_desc.wMaxPacketSize = cpu_to_le16(hidg->report_length);
	hidg_ss_out_ep_desc.wMaxPacketSize = cpu_to_le16(hidg->report_length);
	hidg_ss_out_comp_desc.wBytesPerInterval =
				cpu_to_le16(hidg->report_length);
	hidg_hs_out_ep_desc.wMaxPacketSize = cpu_to_le16(hidg->report_length);
	hidg_fs_out_ep_desc.wMaxPacketSize = cpu_to_le16(hidg->report_length);
	/*
	 * We can use hidg_desc struct here but we should not relay
	 * that its content won't change after returning from this function.
	 */
	hidg_desc.desc[0].bDescriptorType = HID_DT_REPORT;
	hidg_desc.desc[0].wDescriptorLength =
		cpu_to_le16(hidg->report_desc_length);

	hidg_hs_in_ep_desc.bEndpointAddress =
		hidg_fs_in_ep_desc.bEndpointAddress;
	hidg_hs_out_ep_desc.bEndpointAddress =
		hidg_fs_out_ep_desc.bEndpointAddress;

	hidg_ss_in_ep_desc.bEndpointAddress =
		hidg_fs_in_ep_desc.bEndpointAddress;
	hidg_ss_out_ep_desc.bEndpointAddress =
		hidg_fs_out_ep_desc.bEndpointAddress;

	status = usb_assign_descriptors(f, hidg_fs_descriptors,
			hidg_hs_descriptors, hidg_ss_descriptors, NULL);
	if (status)
		goto fail;

	spin_lock_init(&hidg->write_spinlock);
	hidg->write_pending = 1;
	hidg->req = NULL;
	spin_lock_init(&hidg->read_spinlock);
	init_waitqueue_head(&hidg->write_queue);
	init_waitqueue_head(&hidg->read_queue);
	INIT_LIST_HEAD(&hidg->completed_out_req);

	/* create char device */
	cdev_init(&hidg->cdev, &f_hidg_fops);
	dev = MKDEV(major, hidg->minor);
	status = cdev_add(&hidg->cdev, dev, 1);
	if (status)
		goto fail_free_descs;

	device = device_create(hidg_class, NULL, dev, NULL,
			       "%s%d", "hidg", hidg->minor);
	if (IS_ERR(device)) {
		status = PTR_ERR(device);
		goto del;
	}

	return 0;
del:
	cdev_del(&hidg->cdev);
fail_free_descs:
	usb_free_all_descriptors(f);
fail:
	ERROR(f->config->cdev, "hidg_bind FAILED\n");
	if (hidg->req != NULL)
		free_ep_req(hidg->in_ep, hidg->req);

	return status;
}
