void PPB_URLLoader_Impl::SaveResponse(const WebURLResponse& response) {
  scoped_refptr<PPB_URLResponseInfo_Impl> response_info(
      new PPB_URLResponseInfo_Impl(pp_instance()));
  if (response_info->Initialize(response))
    response_info_ = response_info;
}
