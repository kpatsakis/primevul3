static void hidg_free_inst(struct usb_function_instance *f)
{
	struct f_hid_opts *opts;

	opts = container_of(f, struct f_hid_opts, func_inst);

	mutex_lock(&hidg_ida_lock);

	hidg_put_minor(opts->minor);
	if (ida_is_empty(&hidg_ida))
		ghid_cleanup();

	mutex_unlock(&hidg_ida_lock);

	if (opts->report_desc_alloc)
		kfree(opts->report_desc);

	kfree(opts);
}
