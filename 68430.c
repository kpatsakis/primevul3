static int loop_set_capacity(struct loop_device *lo)
{
	if (unlikely(lo->lo_state != Lo_bound))
		return -ENXIO;

	return figure_loop_size(lo, lo->lo_offset, lo->lo_sizelimit);
}
