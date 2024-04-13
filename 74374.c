static struct usb_function *hidg_alloc(struct usb_function_instance *fi)
{
	struct f_hidg *hidg;
	struct f_hid_opts *opts;

	/* allocate and initialize one new instance */
	hidg = kzalloc(sizeof(*hidg), GFP_KERNEL);
	if (!hidg)
		return ERR_PTR(-ENOMEM);

	opts = container_of(fi, struct f_hid_opts, func_inst);

	mutex_lock(&opts->lock);
	++opts->refcnt;

	hidg->minor = opts->minor;
	hidg->bInterfaceSubClass = opts->subclass;
	hidg->bInterfaceProtocol = opts->protocol;
	hidg->report_length = opts->report_length;
	hidg->report_desc_length = opts->report_desc_length;
	if (opts->report_desc) {
		hidg->report_desc = kmemdup(opts->report_desc,
					    opts->report_desc_length,
					    GFP_KERNEL);
		if (!hidg->report_desc) {
			kfree(hidg);
			mutex_unlock(&opts->lock);
			return ERR_PTR(-ENOMEM);
		}
	}

	mutex_unlock(&opts->lock);

	hidg->func.name    = "hid";
	hidg->func.bind    = hidg_bind;
	hidg->func.unbind  = hidg_unbind;
	hidg->func.set_alt = hidg_set_alt;
	hidg->func.disable = hidg_disable;
	hidg->func.setup   = hidg_setup;
	hidg->func.free_func = hidg_free;

	/* this could me made configurable at some point */
	hidg->qlen	   = 4;

	return &hidg->func;
}
