static inline struct f_hidg *func_to_hidg(struct usb_function *f)
{
	return container_of(f, struct f_hidg, func);
}
