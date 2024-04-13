void PrintWebViewHelper::PrintPage(WebKit::WebFrame* frame) {
  DCHECK(frame);

  if (prerender::PrerenderHelper::IsPrerendering(render_view())) {
    Send(new ChromeViewHostMsg_CancelPrerenderForPrinting(routing_id()));
    return;
  }

  if (IsScriptInitiatedPrintTooFrequent(frame))
    return;
  IncrementScriptedPrintCount();

  if (is_preview_enabled_) {
    print_preview_context_.InitWithFrame(frame);
    RequestPrintPreview();
  } else {
    Print(frame, WebNode());
  }
}
