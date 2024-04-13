void GraphicsContext::drawLineForTextChecking(const FloatPoint& pt, float width, TextCheckingLineStyle style)
{
    if (paintingDisabled())
        return;

    static SkBitmap* misspellBitmap = 0;
    if (!misspellBitmap) {
        const int rowPixels = 32;  // Must be multiple of 4 for pattern below.
        const int colPixels = 2;
        misspellBitmap = new SkBitmap;
        misspellBitmap->setConfig(SkBitmap::kARGB_8888_Config,
                                   rowPixels, colPixels);
        misspellBitmap->allocPixels();

        misspellBitmap->eraseARGB(0, 0, 0, 0);
        const uint32_t lineColor = 0xFFFF0000;  // Opaque red.
        const uint32_t antiColor = 0x60600000;  // Semitransparent red.

        uint32_t* row1 = misspellBitmap->getAddr32(0, 0);
        uint32_t* row2 = misspellBitmap->getAddr32(0, 1);
        for (int x = 0; x < rowPixels; x++) {
            switch (x % 4) {
            case 0:
                row1[x] = lineColor;
                break;
            case 1:
                row1[x] = antiColor;
                row2[x] = antiColor;
                break;
            case 2:
                row2[x] = lineColor;
                break;
            case 3:
                row1[x] = antiColor;
                row2[x] = antiColor;
                break;
            }
        }
    }

    SkScalar originX = WebCoreFloatToSkScalar(pt.x());
    SkScalar originY = WebCoreFloatToSkScalar(pt.y()) + 1;

    SkShader* shader = SkShader::CreateBitmapShader(
        *misspellBitmap, SkShader::kRepeat_TileMode,
        SkShader::kRepeat_TileMode);
    SkMatrix matrix;
    matrix.reset();
    matrix.postTranslate(originX, originY);
    shader->setLocalMatrix(matrix);

    SkPaint paint;
    paint.setShader(shader);
    shader->unref();

    SkRect rect;
    rect.set(originX,
             originY,
             originX + WebCoreFloatToSkScalar(width),
             originY + SkIntToScalar(misspellBitmap->height()));
    platformContext()->canvas()->drawRect(rect, paint);
}
