add_to_buffer(struct reqelem * req, const unsigned char * data, int len)
{
	unsigned char * tmp;
	if(req->output_buffer_offset > 0) {
		memmove(req->output_buffer, req->output_buffer + req->output_buffer_offset, req->output_buffer_len);
		req->output_buffer_offset = 0;
	}
	tmp = realloc(req->output_buffer, req->output_buffer_len + len);
	if(tmp == NULL) {
		syslog(LOG_ERR, "%s: failed to allocate %d bytes",
		       __func__, req->output_buffer_len + len);
		return -1;
	}
	req->output_buffer = tmp;
	memcpy(req->output_buffer + req->output_buffer_len, data, len);
	req->output_buffer_len += len;
	return len;
}
