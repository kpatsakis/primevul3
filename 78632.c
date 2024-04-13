content::ColorChooser* ShowColorChooser(content::WebContents* web_contents,
                                        SkColor initial_color) {
  return ColorChooserWin::Open(web_contents, initial_color);
}
