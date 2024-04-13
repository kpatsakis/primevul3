 void ColorChooserWin::OnColorChosen(SkColor color) {
   if (web_contents_)
     web_contents_->DidChooseColorInColorChooser(color);
}
