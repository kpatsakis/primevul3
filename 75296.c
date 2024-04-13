printing::MarginType GetMarginsForPdf(WebFrame* frame, const WebNode& node) {
  if (frame->isPrintScalingDisabledForPlugin(node))
    return printing::NO_MARGINS;
  else
    return printing::PRINTABLE_AREA_MARGINS;
}
