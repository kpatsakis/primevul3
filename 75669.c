PassRefPtr<CSSPrimitiveValue> CSSComputedStyleDeclaration::currentColorOrValidColor(RenderStyle* style, const Color& color) const
{
    if (!color.isValid())
        return cssValuePool().createColorValue(style->color().rgb());
    return cssValuePool().createColorValue(color.rgb());
}
