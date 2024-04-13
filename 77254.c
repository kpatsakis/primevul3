void OmniboxViewWin::DrawSlashForInsecureScheme(HDC hdc,
                                                const CRect& client_rect,
                                                const CRect& paint_clip_rect) {
  DCHECK(insecure_scheme_component_.is_nonempty());

  const int font_top = client_rect.top + font_y_adjustment_;
  const SkScalar kStrokeWidthPixels = SkIntToScalar(2);
  const int kAdditionalSpaceOutsideFont =
      static_cast<int>(ceil(kStrokeWidthPixels * 1.5f));
  const CRect scheme_rect(PosFromChar(insecure_scheme_component_.begin).x,
                          font_top + font_.GetBaseline() - font_x_height_ -
                              kAdditionalSpaceOutsideFont,
                          PosFromChar(insecure_scheme_component_.end()).x,
                          font_top + font_.GetBaseline() +
                              kAdditionalSpaceOutsideFont);

  CRect canvas_clip_rect, canvas_paint_clip_rect;
  canvas_clip_rect.IntersectRect(scheme_rect, client_rect);
  canvas_paint_clip_rect.IntersectRect(canvas_clip_rect, paint_clip_rect);
  if (canvas_paint_clip_rect.IsRectNull())
    return;  // We don't need to paint any of this region, so just bail early.
  canvas_clip_rect.OffsetRect(-scheme_rect.left, -scheme_rect.top);
  canvas_paint_clip_rect.OffsetRect(-scheme_rect.left, -scheme_rect.top);

  SkPaint paint;
  paint.setAntiAlias(true);
  paint.setStrokeWidth(kStrokeWidthPixels);
  paint.setStrokeCap(SkPaint::kRound_Cap);

  gfx::CanvasSkia canvas(gfx::Size(scheme_rect.Width(), scheme_rect.Height()),
                         false);
  SkCanvas* sk_canvas = canvas.sk_canvas();
  sk_canvas->getDevice()->accessBitmap(true).eraseARGB(0, 0, 0, 0);

  const SkScalar kEndCapRadiusPixels = kStrokeWidthPixels / SkIntToScalar(2);
  const SkPoint start_point = {
      kEndCapRadiusPixels,
      SkIntToScalar(scheme_rect.Height()) - kEndCapRadiusPixels };
  const SkPoint end_point = {
      SkIntToScalar(scheme_rect.Width()) - kEndCapRadiusPixels,
      kEndCapRadiusPixels };

  CHARRANGE sel;
  GetSel(sel);
  const SkRect selection_rect = {
      SkIntToScalar(PosFromChar(sel.cpMin).x - scheme_rect.left),
      SkIntToScalar(0),
      SkIntToScalar(PosFromChar(sel.cpMax).x - scheme_rect.left),
      SkIntToScalar(scheme_rect.Height()) };

  sk_canvas->save();
  if (selection_rect.isEmpty() ||
      sk_canvas->clipRect(selection_rect, SkRegion::kDifference_Op)) {
    paint.setColor(LocationBarView::GetColor(security_level_,
                                             LocationBarView::SECURITY_TEXT));
    sk_canvas->drawLine(start_point.fX, start_point.fY,
                        end_point.fX, end_point.fY, paint);
  }
  sk_canvas->restore();

  if (!selection_rect.isEmpty() && sk_canvas->clipRect(selection_rect)) {
    paint.setColor(LocationBarView::GetColor(security_level_,
                                             LocationBarView::SELECTED_TEXT));
    sk_canvas->drawLine(start_point.fX, start_point.fY,
                        end_point.fX, end_point.fY, paint);
  }

  skia::DrawToNativeContext(sk_canvas, hdc,
      scheme_rect.left + canvas_paint_clip_rect.left - canvas_clip_rect.left,
      std::max(scheme_rect.top, client_rect.top) + canvas_paint_clip_rect.top -
          canvas_clip_rect.top, &canvas_paint_clip_rect);
}
