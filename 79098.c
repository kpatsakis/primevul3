void WebURLLoaderImpl::Context::Start(
    const WebURLRequest& request,
    ResourceLoaderBridge::SyncLoadResponse* sync_load_response,
    WebKitPlatformSupportImpl* platform) {
  DCHECK(!bridge_.get());

  request_ = request;  // Save the request.

  GURL url = request.url();
  if (url.SchemeIs("data") && CanHandleDataURL(url)) {
    if (sync_load_response) {
      sync_load_response->url = url;
      std::string data;
      GetInfoFromDataURL(sync_load_response->url, sync_load_response,
                         &sync_load_response->data,
                         &sync_load_response->error_code);
    } else {
      AddRef();  // Balanced in OnCompletedRequest
      base::MessageLoop::current()->PostTask(
          FROM_HERE, base::Bind(&Context::HandleDataURL, this));
    }
    return;
  }

  GURL referrer_url(
      request.httpHeaderField(WebString::fromUTF8("Referer")).utf8());
  const std::string& method = request.httpMethod().utf8();

  int load_flags = net::LOAD_NORMAL;
  switch (request.cachePolicy()) {
    case WebURLRequest::ReloadIgnoringCacheData:
      load_flags |= net::LOAD_VALIDATE_CACHE;
      break;
    case WebURLRequest::ReturnCacheDataElseLoad:
      load_flags |= net::LOAD_PREFERRING_CACHE;
      break;
    case WebURLRequest::ReturnCacheDataDontLoad:
      load_flags |= net::LOAD_ONLY_FROM_CACHE;
      break;
    case WebURLRequest::UseProtocolCachePolicy:
      break;
  }

  if (request.reportUploadProgress())
    load_flags |= net::LOAD_ENABLE_UPLOAD_PROGRESS;
  if (request.reportLoadTiming())
    load_flags |= net::LOAD_ENABLE_LOAD_TIMING;
  if (request.reportRawHeaders())
    load_flags |= net::LOAD_REPORT_RAW_HEADERS;

  if (!request.allowCookies() || !request.allowStoredCredentials()) {
    load_flags |= net::LOAD_DO_NOT_SAVE_COOKIES;
    load_flags |= net::LOAD_DO_NOT_SEND_COOKIES;
  }

  if (!request.allowStoredCredentials())
    load_flags |= net::LOAD_DO_NOT_SEND_AUTH_DATA;

  HeaderFlattener flattener(load_flags);
  request.visitHTTPHeaderFields(&flattener);


  ResourceLoaderBridge::RequestInfo request_info;
  request_info.method = method;
  request_info.url = url;
  request_info.first_party_for_cookies = request.firstPartyForCookies();
  request_info.referrer = referrer_url;
  request_info.headers = flattener.GetBuffer();
  request_info.load_flags = load_flags;
  request_info.requestor_pid = request.requestorProcessID();
  request_info.request_type =
      ResourceType::FromTargetType(request.targetType());
  request_info.priority =
      ConvertWebKitPriorityToNetPriority(request.priority());
  request_info.appcache_host_id = request.appCacheHostID();
  request_info.routing_id = request.requestorID();
  request_info.download_to_file = request.downloadToFile();
  request_info.has_user_gesture = request.hasUserGesture();
  request_info.extra_data = request.extraData();
  if (request.extraData()) {
    referrer_policy_ = static_cast<WebURLRequestExtraDataImpl*>(
        request.extraData())->referrer_policy();
    request_info.referrer_policy = referrer_policy_;
  }
  bridge_.reset(platform->CreateResourceLoader(request_info));

  if (!request.httpBody().isNull()) {
    DCHECK(method != "GET" && method != "HEAD");
    const WebHTTPBody& httpBody = request.httpBody();
    size_t i = 0;
    WebHTTPBody::Element element;
    scoped_refptr<ResourceRequestBody> request_body = new ResourceRequestBody;
    while (httpBody.elementAt(i++, element)) {
      switch (element.type) {
        case WebHTTPBody::Element::TypeData:
          if (!element.data.isEmpty()) {
            request_body->AppendBytes(
                element.data.data(), static_cast<int>(element.data.size()));
          }
          break;
        case WebHTTPBody::Element::TypeFile:
          if (element.fileLength == -1) {
            request_body->AppendFileRange(
                webkit_base::WebStringToFilePath(element.filePath),
                0, kuint64max, base::Time());
          } else {
            request_body->AppendFileRange(
                webkit_base::WebStringToFilePath(element.filePath),
                static_cast<uint64>(element.fileStart),
                static_cast<uint64>(element.fileLength),
                base::Time::FromDoubleT(element.modificationTime));
          }
          break;
        case WebHTTPBody::Element::TypeURL: {
          GURL url = GURL(element.url);
          DCHECK(url.SchemeIsFileSystem());
          request_body->AppendFileSystemFileRange(
              url,
              static_cast<uint64>(element.fileStart),
              static_cast<uint64>(element.fileLength),
              base::Time::FromDoubleT(element.modificationTime));
          break;
        }
        case WebHTTPBody::Element::TypeBlob:
          request_body->AppendBlob(GURL(element.blobURL));
          break;
        default:
          NOTREACHED();
      }
    }
    request_body->set_identifier(request.httpBody().identifier());
    bridge_->SetRequestBody(request_body);
  }

  if (sync_load_response) {
    bridge_->SyncLoad(sync_load_response);
    return;
  }

  if (bridge_->Start(this)) {
    AddRef();  // Balanced in OnCompletedRequest
  } else {
    bridge_.reset();
  }
}
