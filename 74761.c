WebKit::WebPlugin* RenderView::GetWebPluginFromPluginDocument() {
  return webview()->mainFrame()->document().to<WebPluginDocument>().plugin();
}
