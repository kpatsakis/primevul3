void BookmarkManagerView::PaintBackground(gfx::Canvas* canvas) {
  canvas->drawColor(kBackgroundColorBottom, SkPorterDuff::kSrc_Mode);

  SkPaint paint;
  paint.setShader(skia::CreateGradientShader(0, kBackgroundGradientHeight,
      kBackgroundColorTop,
      kBackgroundColorBottom))->safeUnref();
  canvas->FillRectInt(0, 0, width(), kBackgroundGradientHeight, paint);
}
