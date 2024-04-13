static void hidg_unbind(struct usb_configuration *c, struct usb_function *f)
{
	struct f_hidg *hidg = func_to_hidg(f);

	device_destroy(hidg_class, MKDEV(major, hidg->minor));
	cdev_del(&hidg->cdev);

	usb_free_all_descriptors(f);
}
