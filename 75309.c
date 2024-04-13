bool PrintWebViewHelper::PrintPreviewContext::IsModifiable() const {
  return !PrintingNodeOrPdfFrame(frame(), node());
}
