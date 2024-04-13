static int f_midi_start_ep(struct f_midi *midi,
			   struct usb_function *f,
			   struct usb_ep *ep)
{
	int err;
	struct usb_composite_dev *cdev = f->config->cdev;

	usb_ep_disable(ep);

	err = config_ep_by_speed(midi->gadget, f, ep);
	if (err) {
		ERROR(cdev, "can't configure %s: %d\n", ep->name, err);
		return err;
	}

	err = usb_ep_enable(ep);
	if (err) {
		ERROR(cdev, "can't start %s: %d\n", ep->name, err);
		return err;
	}

	ep->driver_data = midi;

	return 0;
}
