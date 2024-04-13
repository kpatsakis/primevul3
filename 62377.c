write_or_buffer(struct reqelem * req, const unsigned char * data, int len)
{
	if(write_buffer(req) < 0)
		return -1;
	if(req->output_buffer && req->output_buffer_len > 0) {
		return add_to_buffer(req, data, len);
	} else {
		int n = write(req->socket, data, len);
		if(n == len)
			return len;
		if(n < 0) {
			if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN) {
				n = add_to_buffer(req, data, len);
				if(n < 0) return n;
			} else {
				return n;
			}
		} else {
			n = add_to_buffer(req, data + n, len - n);
			if(n < 0) return n;
		}
	}
	return len;
}
