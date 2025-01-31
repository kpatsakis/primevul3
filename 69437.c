write_packet(int fd, gss_buffer_t buf)
{
	uint32_t	len;
	OM_uint32	min_stat;
	int		ret = 0;

	len = htonl(buf->length);
	if ((writen(fd, &len, 4) != 4) ||
	    (writen(fd, buf->value, buf->length) != buf->length))
		ret = -1;

	gss_release_buffer (&min_stat, buf);
	return ret;
}
