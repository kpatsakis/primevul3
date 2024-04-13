void DevToolsWindow::RunFileChooser(content::WebContents* web_contents,
                                    const content::FileChooserParams& params) {
  FileSelectHelper::RunFileChooser(web_contents, params);
}
