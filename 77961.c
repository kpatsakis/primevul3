  InterstitialObserver(content::WebContents* web_contents,
                       const base::Closure& attach_callback,
                       const base::Closure& detach_callback)
      : WebContentsObserver(web_contents),
        attach_callback_(attach_callback),
        detach_callback_(detach_callback) {
  }
