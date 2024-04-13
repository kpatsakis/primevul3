static inline struct f_hid_opts *to_f_hid_opts(struct config_item *item)
{
	return container_of(to_config_group(item), struct f_hid_opts,
			    func_inst.group);
}
