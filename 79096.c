void PopulateURLResponse(
    const GURL& url,
    const ResourceResponseInfo& info,
    WebURLResponse* response) {
  response->setURL(url);
  response->setResponseTime(info.response_time.ToDoubleT());
  response->setMIMEType(WebString::fromUTF8(info.mime_type));
  response->setTextEncodingName(WebString::fromUTF8(info.charset));
  response->setExpectedContentLength(info.content_length);
  response->setSecurityInfo(info.security_info);
  response->setAppCacheID(info.appcache_id);
  response->setAppCacheManifestURL(info.appcache_manifest_url);
  response->setWasCached(!info.load_timing.request_start_time.is_null() &&
      info.response_time < info.load_timing.request_start_time);
  response->setRemoteIPAddress(
      WebString::fromUTF8(info.socket_address.host()));
  response->setRemotePort(info.socket_address.port());
  response->setConnectionID(info.load_timing.socket_log_id);
  response->setConnectionReused(info.load_timing.socket_reused);
  response->setDownloadFilePath(
      webkit_base::FilePathToWebString(info.download_file_path));
  WebURLResponseExtraDataImpl* extra_data =
      new WebURLResponseExtraDataImpl(info.npn_negotiated_protocol);
  response->setExtraData(extra_data);
  extra_data->set_was_fetched_via_spdy(info.was_fetched_via_spdy);
  extra_data->set_was_npn_negotiated(info.was_npn_negotiated);
  extra_data->set_was_alternate_protocol_available(
      info.was_alternate_protocol_available);
  extra_data->set_was_fetched_via_proxy(info.was_fetched_via_proxy);

  if (!info.load_timing.receive_headers_end.is_null()) {
    WebURLLoadTiming timing;
    PopulateURLLoadTiming(info.load_timing, &timing);
    response->setLoadTiming(timing);
  }

  if (info.devtools_info) {
    WebHTTPLoadInfo load_info;

    load_info.setHTTPStatusCode(info.devtools_info->http_status_code);
    load_info.setHTTPStatusText(WebString::fromUTF8(
        info.devtools_info->http_status_text));
    load_info.setEncodedDataLength(info.encoded_data_length);

    load_info.setRequestHeadersText(WebString::fromUTF8(
        info.devtools_info->request_headers_text));
    load_info.setResponseHeadersText(WebString::fromUTF8(
        info.devtools_info->response_headers_text));
    const HeadersVector& request_headers = info.devtools_info->request_headers;
    for (HeadersVector::const_iterator it = request_headers.begin();
         it != request_headers.end(); ++it) {
      load_info.addRequestHeader(WebString::fromUTF8(it->first),
          WebString::fromUTF8(it->second));
    }
    const HeadersVector& response_headers =
        info.devtools_info->response_headers;
    for (HeadersVector::const_iterator it = response_headers.begin();
         it != response_headers.end(); ++it) {
      load_info.addResponseHeader(WebString::fromUTF8(it->first),
          WebString::fromUTF8(it->second));
    }
    response->setHTTPLoadInfo(load_info);
  }

  const net::HttpResponseHeaders* headers = info.headers;
  if (!headers)
    return;

  WebURLResponse::HTTPVersion version = WebURLResponse::Unknown;
  if (headers->GetHttpVersion() == net::HttpVersion(0, 9))
    version = WebURLResponse::HTTP_0_9;
  else if (headers->GetHttpVersion() == net::HttpVersion(1, 0))
    version = WebURLResponse::HTTP_1_0;
  else if (headers->GetHttpVersion() == net::HttpVersion(1, 1))
    version = WebURLResponse::HTTP_1_1;
  response->setHTTPVersion(version);
  response->setHTTPStatusCode(headers->response_code());
  response->setHTTPStatusText(WebString::fromUTF8(headers->GetStatusText()));

  std::string value;
  headers->EnumerateHeader(NULL, "content-disposition", &value);
  response->setSuggestedFileName(
      net::GetSuggestedFilename(url,
                                value,
                                std::string(),  // referrer_charset
                                std::string(),  // suggested_name
                                std::string(),  // mime_type
                                std::string()));  // default_name

  Time time_val;
  if (headers->GetLastModifiedValue(&time_val))
    response->setLastModifiedDate(time_val.ToDoubleT());

  void* iter = NULL;
  std::string name;
  while (headers->EnumerateHeaderLines(&iter, &name, &value)) {
    response->addHTTPHeaderField(WebString::fromUTF8(name),
                                 WebString::fromUTF8(value));
  }
}
