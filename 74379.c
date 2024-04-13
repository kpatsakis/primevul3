static void hidg_free(struct usb_function *f)
{
	struct f_hidg *hidg;
	struct f_hid_opts *opts;

	hidg = func_to_hidg(f);
	opts = container_of(f->fi, struct f_hid_opts, func_inst);
	kfree(hidg->report_desc);
	kfree(hidg);
	mutex_lock(&opts->lock);
	--opts->refcnt;
	mutex_unlock(&opts->lock);
}
