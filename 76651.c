  virtual bool IsHandledURL(const GURL& url) {
    return schemes_.find(url.scheme()) != schemes_.end();
  }
