void NotifyCacheOnIO(
    scoped_refptr<net::URLRequestContextGetter> request_context,
    const GURL& url,
    const std::string& http_method) {
  request_context->GetURLRequestContext()->http_transaction_factory()->
      GetCache()->OnExternalCacheHit(url, http_method);
}
