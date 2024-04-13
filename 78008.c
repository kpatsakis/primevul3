  DestructionObserver(WebContentsImpl* owner, WebContents* watched_contents)
      : WebContentsObserver(watched_contents),
        owner_(owner) {
  }
