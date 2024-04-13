void PopulateURLLoadTiming(const net::LoadTimingInfo& load_timing,
                           WebURLLoadTiming* url_timing) {
  DCHECK(!load_timing.request_start.is_null());

  const TimeTicks kNullTicks;
  url_timing->initialize();
  url_timing->setRequestTime(
      (load_timing.request_start - kNullTicks).InSecondsF());
  url_timing->setProxyStart(
      (load_timing.proxy_resolve_start - kNullTicks).InSecondsF());
  url_timing->setProxyEnd(
      (load_timing.proxy_resolve_end - kNullTicks).InSecondsF());
  url_timing->setDNSStart(
      (load_timing.connect_timing.dns_start - kNullTicks).InSecondsF());
  url_timing->setDNSEnd(
      (load_timing.connect_timing.dns_end - kNullTicks).InSecondsF());
  url_timing->setConnectStart(
      (load_timing.connect_timing.connect_start - kNullTicks).InSecondsF());
  url_timing->setConnectEnd(
      (load_timing.connect_timing.connect_end - kNullTicks).InSecondsF());
  url_timing->setSSLStart(
      (load_timing.connect_timing.ssl_start - kNullTicks).InSecondsF());
  url_timing->setSSLEnd(
      (load_timing.connect_timing.ssl_end - kNullTicks).InSecondsF());
  url_timing->setSendStart(
      (load_timing.send_start - kNullTicks).InSecondsF());
  url_timing->setSendEnd(
      (load_timing.send_end - kNullTicks).InSecondsF());
  url_timing->setReceiveHeadersEnd(
      (load_timing.receive_headers_end - kNullTicks).InSecondsF());
}
