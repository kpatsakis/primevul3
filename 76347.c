void updateGraphicsContext(GraphicsContext* context, const Color& fillColor, const Color& strokeColor, float strokeThickness, ColorSpace colorSpace)
{
    int mode = context->textDrawingMode();
    if (strokeThickness > 0) {
        int newMode = mode | cTextStroke;
        if (mode != newMode) {
            context->setTextDrawingMode(newMode);
            mode = newMode;
        }
    }
    
    if (mode & cTextFill && (fillColor != context->fillColor() || colorSpace != context->fillColorSpace()))
        context->setFillColor(fillColor, colorSpace);

    if (mode & cTextStroke) {
        if (strokeColor != context->strokeColor())
            context->setStrokeColor(strokeColor, colorSpace);
        if (strokeThickness != context->strokeThickness())
            context->setStrokeThickness(strokeThickness);
    }
}
