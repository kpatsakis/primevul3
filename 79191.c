content::ColorChooser* DevToolsWindow::OpenColorChooser(
    content::WebContents* web_contents,
    SkColor initial_color) {
  return chrome::ShowColorChooser(web_contents, initial_color);
}
