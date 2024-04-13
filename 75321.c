void PrintWebViewHelper::PrintHeaderAndFooter(
    WebKit::WebCanvas* canvas,
    int page_number,
    int total_pages,
    float webkit_scale_factor,
    const PageSizeMargins& page_layout,
    const DictionaryValue& header_footer_info) {
#if defined(USE_SKIA)
  skia::VectorPlatformDeviceSkia* device =
      static_cast<skia::VectorPlatformDeviceSkia*>(canvas->getTopDevice());
  device->setDrawingArea(SkPDFDevice::kMargin_DrawingArea);

  SkPaint paint;
  paint.setColor(SK_ColorBLACK);
  paint.setTextEncoding(SkPaint::kUTF16_TextEncoding);
  paint.setTextSize(SkDoubleToScalar(printing::kSettingHeaderFooterFontSize));
  paint.setTypeface(SkTypeface::CreateFromName(
      printing::kSettingHeaderFooterFontFamilyName, SkTypeface::kNormal));
#elif defined(OS_MACOSX)
  gfx::ScopedCGContextSaveGState CGContextSaveGState(canvas);
  CGContextSetCharacterSpacing(canvas,
                               printing::kSettingHeaderFooterCharacterSpacing);
  CGContextSetTextDrawingMode(canvas, kCGTextFill);
  CGContextSetGrayFillColor(canvas, kBlackGrayLevel, kOpaqueLevel);
  CGContextSelectFont(canvas, printing::kSettingHeaderFooterFontName,
                      printing::kSettingHeaderFooterFontSize,
                      kCGEncodingFontSpecific);
  ScopedCFTypeRef<CFStringRef> font_name(base::SysUTF8ToCFStringRef(
      printing::kSettingHeaderFooterFontName));
  const CGAffineTransform flip_text =  CGAffineTransformMakeScale(1.0f, -1.0f);
  ScopedCFTypeRef<CTFontRef> ct_font(CTFontCreateWithName(
      font_name,
      printing::kSettingHeaderFooterFontSize / webkit_scale_factor,
      &flip_text));
  const void* keys[] = {kCTFontAttributeName};
  const void* values[] = {ct_font};
  ScopedCFTypeRef<CFDictionaryRef> paint(CFDictionaryCreate(
      NULL, keys, values, sizeof(keys) / sizeof(keys[0]), NULL, NULL));
#endif

  string16 date;
  string16 title;
  if (!header_footer_info.GetString(printing::kSettingHeaderFooterTitle,
                                    &title) ||
      !header_footer_info.GetString(printing::kSettingHeaderFooterDate,
                                    &date)) {
    NOTREACHED();
  }
  string16 header_text = date + title;

  SkRect header_vertical_bounds;
#if defined(USE_SKIA)
  paint.measureText(header_text.c_str(), header_text.length() * sizeof(char16),
                    &header_vertical_bounds, 0);
#elif defined(OS_MACOSX)
  header_vertical_bounds.fTop = CTFontGetAscent(ct_font) * webkit_scale_factor;
  header_vertical_bounds.fBottom = -CTFontGetDescent(ct_font) *
                                   webkit_scale_factor;
#endif
  double text_height = printing::kSettingHeaderFooterInterstice +
                       header_vertical_bounds.height();
  if (text_height <= page_layout.margin_top) {
    PrintHeaderFooterText(date, canvas, paint, webkit_scale_factor, page_layout,
                          printing::LEFT, printing::TOP,
                          header_vertical_bounds.top());
    PrintHeaderFooterText(title, canvas, paint, webkit_scale_factor,
                          page_layout, printing::CENTER, printing::TOP,
                          header_vertical_bounds.top());
  }

  string16 page_of_total_pages = base::IntToString16(page_number) +
                                 UTF8ToUTF16("/") +
                                 base::IntToString16(total_pages);
  string16 url;
  if (!header_footer_info.GetString(printing::kSettingHeaderFooterURL,
                                    &url)) {
    NOTREACHED();
  }
  string16 footer_text = page_of_total_pages + url;

  SkRect footer_vertical_bounds;
#if defined(USE_SKIA)
  paint.measureText(footer_text.c_str(), footer_text.length() * sizeof(char16),
                    &footer_vertical_bounds, 0);
#elif defined(OS_MACOSX)
  footer_vertical_bounds.fTop = header_vertical_bounds.fTop;
  footer_vertical_bounds.fBottom = header_vertical_bounds.fBottom;
#endif
  text_height = printing::kSettingHeaderFooterInterstice +
                footer_vertical_bounds.height();
  if (text_height <= page_layout.margin_bottom) {
    PrintHeaderFooterText(page_of_total_pages, canvas, paint,
                          webkit_scale_factor, page_layout, printing::RIGHT,
                          printing::BOTTOM, footer_vertical_bounds.bottom());
    PrintHeaderFooterText(url, canvas, paint, webkit_scale_factor, page_layout,
                          printing::LEFT, printing::BOTTOM,
                          footer_vertical_bounds.bottom());
  }

#if defined(USE_SKIA)
  device->setDrawingArea(SkPDFDevice::kContent_DrawingArea);
#endif
}
