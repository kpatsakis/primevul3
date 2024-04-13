void PPB_URLLoader_Impl::didFail(WebURLLoader* loader,
                                 const WebURLError& error) {
  int32_t pp_error = PP_ERROR_FAILED;
  if (error.domain.equals(WebString::fromUTF8(net::kErrorDomain))) {
    switch (error.reason) {
      case net::ERR_ABORTED:
        pp_error = PP_ERROR_ABORTED;
        break;
      case net::ERR_ACCESS_DENIED:
      case net::ERR_NETWORK_ACCESS_DENIED:
        pp_error = PP_ERROR_NOACCESS;
        break;
    }
  } else {
    pp_error = PP_ERROR_NOACCESS;
  }

  FinishLoading(pp_error);
}
