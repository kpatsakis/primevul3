bool RenderView::isSelectTrailingWhitespaceEnabled() {
#if defined(OS_WIN)
  return true;
#else
  return false;
#endif
}
