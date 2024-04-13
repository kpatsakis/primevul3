static struct usb_function *f_midi_alloc(struct usb_function_instance *fi)
{
	struct f_midi *midi = NULL;
	struct f_midi_opts *opts;
	int status, i;

	opts = container_of(fi, struct f_midi_opts, func_inst);

	mutex_lock(&opts->lock);
	/* sanity check */
	if (opts->in_ports > MAX_PORTS || opts->out_ports > MAX_PORTS) {
		status = -EINVAL;
		goto setup_fail;
	}

	/* allocate and initialize one new instance */
	midi = kzalloc(
		sizeof(*midi) + opts->in_ports * sizeof(*midi->in_ports_array),
		GFP_KERNEL);
	if (!midi) {
		status = -ENOMEM;
		goto setup_fail;
	}

	for (i = 0; i < opts->in_ports; i++)
		midi->in_ports_array[i].cable = i;

	/* set up ALSA midi devices */
	midi->id = kstrdup(opts->id, GFP_KERNEL);
	if (opts->id && !midi->id) {
		status = -ENOMEM;
		goto setup_fail;
	}
	midi->in_ports = opts->in_ports;
	midi->out_ports = opts->out_ports;
	midi->index = opts->index;
	midi->buflen = opts->buflen;
	midi->qlen = opts->qlen;
	midi->in_last_port = 0;
	midi->free_ref = 1;

	status = kfifo_alloc(&midi->in_req_fifo, midi->qlen, GFP_KERNEL);
	if (status)
		goto setup_fail;

	spin_lock_init(&midi->transmit_lock);

	++opts->refcnt;
	mutex_unlock(&opts->lock);

	midi->func.name		= "gmidi function";
	midi->func.bind		= f_midi_bind;
	midi->func.unbind	= f_midi_unbind;
	midi->func.set_alt	= f_midi_set_alt;
	midi->func.disable	= f_midi_disable;
	midi->func.free_func	= f_midi_free;

	return &midi->func;

setup_fail:
	mutex_unlock(&opts->lock);
	kfree(midi);
	return ERR_PTR(status);
}
