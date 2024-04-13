static PassRefPtr<CSSValue> valueForReflection(const StyleReflection* reflection, const RenderStyle* style)
{
    if (!reflection)
        return cssValuePool().createIdentifierValue(CSSValueNone);

    RefPtr<CSSPrimitiveValue> offset;
    if (reflection->offset().isPercent())
        offset = cssValuePool().createValue(reflection->offset().percent(), CSSPrimitiveValue::CSS_PERCENTAGE);
    else
        offset = zoomAdjustedPixelValue(reflection->offset().value(), style);

    return CSSReflectValue::create(reflection->direction(), offset.release(), valueForNinePieceImage(reflection->mask()));
}
