static void midi_attr_release(struct config_item *item)
{
	struct f_midi_opts *opts = to_f_midi_opts(item);

	usb_put_function_instance(&opts->func_inst);
}
