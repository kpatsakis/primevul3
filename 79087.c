void WebURLLoaderImpl::Context::HandleDataURL() {
  ResourceResponseInfo info;
  int error_code;
  std::string data;

  if (GetInfoFromDataURL(request_.url(), &info, &data, &error_code)) {
    OnReceivedResponse(info);
    if (!data.empty())
      OnReceivedData(data.data(), data.size(), 0);
  }

  OnCompletedRequest(error_code, false, info.security_info,
                     base::TimeTicks::Now());
}
