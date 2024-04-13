bool GetInfoFromDataURL(const GURL& url,
                        ResourceResponseInfo* info,
                        std::string* data,
                        int* error_code) {
  std::string mime_type;
  std::string charset;
  if (net::DataURL::Parse(url, &mime_type, &charset, data)) {
    *error_code = net::OK;
    Time now = Time::Now();
    info->load_timing.request_start = TimeTicks::Now();
    info->load_timing.request_start_time = now;
    info->request_time = now;
    info->response_time = now;
    info->headers = NULL;
    info->mime_type.swap(mime_type);
    info->charset.swap(charset);
    info->security_info.clear();
    info->content_length = data->length();
    info->encoded_data_length = 0;

    return true;
  }

  *error_code = net::ERR_INVALID_URL;
  return false;
}
