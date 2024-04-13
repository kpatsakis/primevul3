static struct usb_function_instance *f_midi_alloc_inst(void)
{
	struct f_midi_opts *opts;

	opts = kzalloc(sizeof(*opts), GFP_KERNEL);
	if (!opts)
		return ERR_PTR(-ENOMEM);

	mutex_init(&opts->lock);
	opts->func_inst.free_func_inst = f_midi_free_inst;
	opts->index = SNDRV_DEFAULT_IDX1;
	opts->id = SNDRV_DEFAULT_STR1;
	opts->buflen = 512;
	opts->qlen = 32;
	opts->in_ports = 1;
	opts->out_ports = 1;

	config_group_init_type_name(&opts->func_inst.group, "",
				    &midi_func_type);

	return &opts->func_inst;
}
