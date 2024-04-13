static inline struct f_midi *func_to_midi(struct usb_function *f)
{
	return container_of(f, struct f_midi, func);
}
