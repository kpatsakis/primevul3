static PassRefPtr<CSSValue> getBorderRadiusCornerValue(LengthSize radius, const RenderStyle* style, RenderView* renderView)
{
    if (radius.width() == radius.height()) {
        if (radius.width().type() == Percent)
            return cssValuePool().createValue(radius.width().percent(), CSSPrimitiveValue::CSS_PERCENTAGE);
        return zoomAdjustedPixelValue(valueForLength(radius.width(), 0, renderView), style);
    }
    return getBorderRadiusCornerValues(radius, style, renderView);
}
