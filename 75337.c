void PrintWebViewHelper::UpdatePrintableSizeInPrintParameters(
    WebFrame* frame,
    const WebNode& node,
    PrepareFrameAndViewForPrint* prepare,
    PrintMsg_Print_Params* params) {
  if (PrintingNodeOrPdfFrame(frame, node))
    return;
  PageSizeMargins page_layout_in_points;
  PrintWebViewHelper::GetPageSizeAndMarginsInPoints(frame, 0, *params,
                                                    &page_layout_in_points);
  int dpi = GetDPI(params);
  params->printable_size = gfx::Size(
      static_cast<int>(ConvertUnitDouble(
          page_layout_in_points.content_width,
          printing::kPointsPerInch, dpi)),
      static_cast<int>(ConvertUnitDouble(
          page_layout_in_points.content_height,
          printing::kPointsPerInch, dpi)));

  double page_width_in_points =
      page_layout_in_points.content_width +
      page_layout_in_points.margin_left +
      page_layout_in_points.margin_right;
  double page_height_in_points =
      page_layout_in_points.content_height +
      page_layout_in_points.margin_top +
      page_layout_in_points.margin_bottom;

  params->page_size = gfx::Size(
      static_cast<int>(ConvertUnitDouble(
          page_width_in_points, printing::kPointsPerInch, dpi)),
      static_cast<int>(ConvertUnitDouble(
          page_height_in_points, printing::kPointsPerInch, dpi)));

  params->margin_top = static_cast<int>(ConvertUnitDouble(
      page_layout_in_points.margin_top, printing::kPointsPerInch, dpi));
  params->margin_left = static_cast<int>(ConvertUnitDouble(
      page_layout_in_points.margin_left, printing::kPointsPerInch, dpi));

  prepare->UpdatePrintParams(*params);
}
