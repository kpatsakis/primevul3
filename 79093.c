bool WebURLLoaderImpl::Context::OnReceivedRedirect(
    const GURL& new_url,
    const ResourceResponseInfo& info,
    bool* has_new_first_party_for_cookies,
    GURL* new_first_party_for_cookies) {
  if (!client_)
    return false;

  WebURLResponse response;
  response.initialize();
  PopulateURLResponse(request_.url(), info, &response);

  WebURLRequest new_request(new_url);
  new_request.setFirstPartyForCookies(request_.firstPartyForCookies());
  new_request.setDownloadToFile(request_.downloadToFile());

  WebString referrer_string = WebString::fromUTF8("Referer");
  WebString referrer = WebSecurityPolicy::generateReferrerHeader(
      referrer_policy_,
      new_url,
      request_.httpHeaderField(referrer_string));
  if (!referrer.isEmpty())
    new_request.setHTTPHeaderField(referrer_string, referrer);

  if (response.httpStatusCode() == 307)
    new_request.setHTTPMethod(request_.httpMethod());

  client_->willSendRequest(loader_, new_request, response);
  request_ = new_request;
  *has_new_first_party_for_cookies = true;
  *new_first_party_for_cookies = request_.firstPartyForCookies();

  if (new_url == GURL(new_request.url()))
    return true;

  DCHECK(!new_request.url().isValid());
  return false;
}
