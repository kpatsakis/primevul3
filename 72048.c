static inline struct f_midi_opts *to_f_midi_opts(struct config_item *item)
{
	return container_of(to_config_group(item), struct f_midi_opts,
			    func_inst.group);
}
