  RenderViewSizeObserver(content::WebContents* web_contents,
                         BrowserWindow* browser_window)
      : WebContentsObserver(web_contents),
        browser_window_(browser_window) {
  }
