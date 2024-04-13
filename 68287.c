UNCURL_EXPORT void uncurl_set_option(struct uncurl_conn *ucc, int32_t opt, int32_t val)
{
	switch (opt) {
		case UNCURL_OPT_MAX_HEADER:
			ucc->opts.max_header = (uint32_t) val; break;
		case UNCURL_OPT_MAX_BODY:
			ucc->opts.max_body = (uint32_t) val; break;

		case UNCURL_NOPT_READ_TIMEOUT:
			ucc->nopts.read_timeout = val; break;
		case UNCURL_NOPT_CONNECT_TIMEOUT:
			ucc->nopts.connect_timeout = val; break;
		case UNCURL_NOPT_ACCEPT_TIMEOUT:
			ucc->nopts.accept_timeout = val; break;
		case UNCURL_NOPT_READ_BUF:
			ucc->nopts.read_buf = val; break;
		case UNCURL_NOPT_WRITE_BUF:
			ucc->nopts.write_buf = val; break;
		case UNCURL_NOPT_KEEPALIVE:
			ucc->nopts.keepalive = val; break;
		case UNCURL_NOPT_TCP_NODELAY:
			ucc->nopts.tcp_nodelay = val; break;
		case UNCURL_NOPT_REUSEADDR:
			ucc->nopts.reuseaddr = val; break;

		case UNCURL_TOPT_VERIFY_HOST:
			ucc->topts.verify_host = val; break;
	}
}
