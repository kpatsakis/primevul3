static inline void hidg_put_minor(int minor)
{
	ida_simple_remove(&hidg_ida, minor);
}
