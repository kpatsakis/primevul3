void PrintWebViewHelper::GetPageSizeAndMarginsInPoints(
    WebFrame* frame,
    int page_index,
    const PrintMsg_Print_Params& default_params,
    PageSizeMargins* page_layout_in_points) {
  int dpi = GetDPI(&default_params);

  WebSize page_size_in_pixels(
      ConvertUnit(default_params.page_size.width(),
                  dpi, printing::kPixelsPerInch),
      ConvertUnit(default_params.page_size.height(),
                  dpi, printing::kPixelsPerInch));
  int margin_top_in_pixels = ConvertUnit(
      default_params.margin_top,
      dpi, printing::kPixelsPerInch);
  int margin_right_in_pixels = ConvertUnit(
      default_params.page_size.width() -
      default_params.printable_size.width() - default_params.margin_left,
      dpi, printing::kPixelsPerInch);
  int margin_bottom_in_pixels = ConvertUnit(
      default_params.page_size.height() -
      default_params.printable_size.height() - default_params.margin_top,
      dpi, printing::kPixelsPerInch);
  int margin_left_in_pixels = ConvertUnit(
      default_params.margin_left,
      dpi, printing::kPixelsPerInch);

  if (frame) {
    frame->pageSizeAndMarginsInPixels(page_index,
                                      page_size_in_pixels,
                                      margin_top_in_pixels,
                                      margin_right_in_pixels,
                                      margin_bottom_in_pixels,
                                      margin_left_in_pixels);
  }

  page_layout_in_points->content_width =
      ConvertPixelsToPoint(page_size_in_pixels.width -
                           margin_left_in_pixels -
                           margin_right_in_pixels);
  page_layout_in_points->content_height =
      ConvertPixelsToPoint(page_size_in_pixels.height -
                           margin_top_in_pixels -
                           margin_bottom_in_pixels);

  if (page_layout_in_points->content_width < 1.0 ||
      page_layout_in_points->content_height < 1.0) {
    CHECK(frame != NULL);
    GetPageSizeAndMarginsInPoints(NULL, page_index, default_params,
                                  page_layout_in_points);
    return;
  }

    page_layout_in_points->margin_top =
        ConvertPixelsToPointDouble(margin_top_in_pixels);
    page_layout_in_points->margin_right =
        ConvertPixelsToPointDouble(margin_right_in_pixels);
    page_layout_in_points->margin_bottom =
        ConvertPixelsToPointDouble(margin_bottom_in_pixels);
    page_layout_in_points->margin_left =
        ConvertPixelsToPointDouble(margin_left_in_pixels);
}
