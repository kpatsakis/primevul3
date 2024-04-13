static PassRefPtr<CSSValue> valueForNinePieceImageRepeat(const NinePieceImage& image)
{
    RefPtr<CSSPrimitiveValue> horizontalRepeat;
    RefPtr<CSSPrimitiveValue> verticalRepeat;

    horizontalRepeat = cssValuePool().createIdentifierValue(valueForRepeatRule(image.horizontalRule()));
    if (image.horizontalRule() == image.verticalRule())
        verticalRepeat = horizontalRepeat;
    else
        verticalRepeat = cssValuePool().createIdentifierValue(valueForRepeatRule(image.verticalRule()));
    return cssValuePool().createValue(Pair::create(horizontalRepeat.release(), verticalRepeat.release()));
}
