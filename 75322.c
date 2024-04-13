void PrintHeaderFooterText(
    string16 text,
    WebKit::WebCanvas* canvas,
    HeaderFooterPaint paint,
    float webkit_scale_factor,
    const PageSizeMargins& page_layout,
    printing::HorizontalHeaderFooterPosition horizontal_position,
    printing::VerticalHeaderFooterPosition vertical_position,
    double offset_to_baseline) {
#if defined(USE_SKIA)
  size_t text_byte_length = text.length() * sizeof(char16);
  double text_width_in_points = SkScalarToDouble(paint.measureText(
      text.c_str(), text_byte_length));
  SkPoint point = GetHeaderFooterPosition(webkit_scale_factor, page_layout,
                                          horizontal_position,
                                          vertical_position, offset_to_baseline,
                                          text_width_in_points);
  paint.setTextSize(SkDoubleToScalar(
      paint.getTextSize() / webkit_scale_factor));
  canvas->drawText(text.c_str(), text_byte_length, point.x(), point.y(),
                   paint);
#elif defined(OS_MACOSX)
  ScopedCFTypeRef<CFStringRef> cf_text(base::SysUTF16ToCFStringRef(text));
  ScopedCFTypeRef<CFAttributedStringRef> cf_attr_text(
      CFAttributedStringCreate(NULL, cf_text, paint));
  ScopedCFTypeRef<CTLineRef> line(CTLineCreateWithAttributedString(
      cf_attr_text));
  double text_width_in_points =
      CTLineGetTypographicBounds(line, NULL, NULL, NULL) * webkit_scale_factor;
  SkPoint point = GetHeaderFooterPosition(webkit_scale_factor,
                                          page_layout, horizontal_position,
                                          vertical_position, offset_to_baseline,
                                          text_width_in_points);
  CGContextSetTextPosition(canvas, SkScalarToDouble(point.x()),
                           SkScalarToDouble(point.y()));
  CTLineDraw(line, canvas);
#endif
}
