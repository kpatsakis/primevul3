static PassRefPtr<CSSValue> renderUnicodeBidiFlagsToCSSValue(EUnicodeBidi unicodeBidi)
{
    switch (unicodeBidi) {
    case UBNormal:
        return cssValuePool().createIdentifierValue(CSSValueNormal);
    case Embed:
        return cssValuePool().createIdentifierValue(CSSValueEmbed);
    case Plaintext:
        return cssValuePool().createIdentifierValue(CSSValueWebkitPlaintext);
    case Override:
        return cssValuePool().createIdentifierValue(CSSValueBidiOverride);
    case Isolate:
        return cssValuePool().createIdentifierValue(CSSValueWebkitIsolate);
    case OverrideIsolate:
    {
        RefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();
        list->append(cssValuePool().createIdentifierValue(CSSValueBidiOverride));
        list->append(cssValuePool().createIdentifierValue(CSSValueWebkitIsolate));
        return list;
    }
    }
    ASSERT_NOT_REACHED();
    return 0;
}
