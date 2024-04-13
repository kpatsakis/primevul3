evdns_log_(int severity, const char *fmt, ...)
{
	va_list args;
	va_start(args,fmt);
	if (evdns_log_fn) {
		char buf[512];
		int is_warn = (severity == EVDNS_LOG_WARN);
		evutil_vsnprintf(buf, sizeof(buf), fmt, args);
		evdns_log_fn(is_warn, buf);
	} else {
		event_logv_(severity, NULL, fmt, args);
	}
	va_end(args);
}
