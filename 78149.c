void WebContentsImpl::OnWebContentsDestroyed(WebContentsImpl* web_contents) {
  RemoveDestructionObserver(web_contents);

  if (web_contents == opener_) {
    opener_ = NULL;
    return;
  }
  for (PendingContents::iterator iter = pending_contents_.begin();
       iter != pending_contents_.end();
       ++iter) {
    if (iter->second != web_contents)
      continue;
    pending_contents_.erase(iter);
    return;
  }
  NOTREACHED();
}
