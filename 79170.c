content::JavaScriptDialogManager* DevToolsWindow::GetJavaScriptDialogManager() {
  content::WebContents* inspected_web_contents = GetInspectedWebContents();
  return (inspected_web_contents && inspected_web_contents->GetDelegate()) ?
      inspected_web_contents->GetDelegate()->GetJavaScriptDialogManager() :
      content::WebContentsDelegate::GetJavaScriptDialogManager();
}
