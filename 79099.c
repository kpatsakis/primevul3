WebURLLoaderImpl::WebURLLoaderImpl(WebKitPlatformSupportImpl* platform)
    : context_(new Context(this)),
      platform_(platform) {
}
