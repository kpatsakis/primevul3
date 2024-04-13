  void InjectBeforeUnloadListener(content::WebContents* web_contents) {
    ASSERT_TRUE(content::ExecuteScript(web_contents->GetRenderViewHost(),
        "window.addEventListener('beforeunload',"
        "function(event) { event.returnValue = 'Foo'; });"));
  }
