static void paintTextWithShadows(GraphicsContext* context, const Font& font, const TextRun& textRun, int startOffset, int endOffset, int truncationPoint, const IntPoint& textOrigin,
                                 int x, int y, int w, int h, const ShadowData* shadow, bool stroked, bool vertical)
{
    Color fillColor = context->fillColor();
    ColorSpace fillColorSpace = context->fillColorSpace();
    bool opaque = fillColor.alpha() == 255;
    if (!opaque)
        context->setFillColor(Color::black, fillColorSpace);

    do {
        IntSize extraOffset;
        if (shadow)
            extraOffset = roundedIntSize(InlineTextBox::applyShadowToGraphicsContext(context, shadow, FloatRect(x, y, w, h), stroked, opaque, vertical));
        else if (!opaque)
            context->setFillColor(fillColor, fillColorSpace);

        if (startOffset <= endOffset)
            context->drawText(font, textRun, textOrigin + extraOffset, startOffset, endOffset);
        else {
            if (endOffset > 0)
                context->drawText(font, textRun, textOrigin + extraOffset,  0, endOffset);
            if (startOffset < truncationPoint)
                context->drawText(font, textRun, textOrigin + extraOffset, startOffset, truncationPoint);
        }

        if (!shadow)
            break;

        if (shadow->next() || stroked || !opaque)
            context->restore();
        else
            context->clearShadow();

        shadow = shadow->next();
    } while (shadow || stroked || !opaque);
}
