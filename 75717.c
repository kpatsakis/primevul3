static PassRefPtr<CSSValue> valueForGridTrackList(const Vector<Length>& trackLengths, const RenderStyle* style)
{
    ASSERT(trackLengths.size());

    if (trackLengths.size() == 1 && trackLengths[0].isUndefined())
        return cssValuePool().createIdentifierValue(CSSValueNone);

    RefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();
    for (size_t i = 0; i < trackLengths.size(); ++i)
        list->append(valueForGridTrackBreadth(trackLengths[i], style));
    return list.release();
}
