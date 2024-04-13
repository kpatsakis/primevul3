PrintWebViewHelper::PrintPreviewContext::metafile() const {
  DCHECK(IsRendering());
  return metafile_.get();
}
