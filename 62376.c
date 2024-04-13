write_buffer(struct reqelem * req)
{
	if(req->output_buffer && req->output_buffer_len > 0) {
		int n = write(req->socket,
		              req->output_buffer + req->output_buffer_offset,
		              req->output_buffer_len);
		if(n >= 0) {
			req->output_buffer_offset += n;
			req->output_buffer_len -= n;
		} else if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN) {
			return 0;
		}
		return n;
	} else {
		return 0;
	}
}
