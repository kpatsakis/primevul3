ITextDocument* OmniboxViewWin::GetTextObjectModel() const {
  if (!text_object_model_) {
    base::win::ScopedComPtr<IRichEditOle, NULL> ole_interface;
    ole_interface.Attach(GetOleInterface());
    if (ole_interface) {
      ole_interface.QueryInterface(
          __uuidof(ITextDocument),
          reinterpret_cast<void**>(&text_object_model_));
    }
  }
  return text_object_model_;
}
