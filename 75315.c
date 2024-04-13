void PrintWebViewHelper::OnPrintPages() {
  WebFrame* frame;
  if (GetPrintFrame(&frame))
    Print(frame, WebNode());
}
