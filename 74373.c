static void hid_attr_release(struct config_item *item)
{
	struct f_hid_opts *opts = to_f_hid_opts(item);

	usb_put_function_instance(&opts->func_inst);
}
