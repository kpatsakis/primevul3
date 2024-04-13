  explicit MockWebContentsObserver(WebContents* web_contents)
      : WebContentsObserver(web_contents),
        got_user_gesture_(false) {
  }
