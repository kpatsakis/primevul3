static PassRefPtr<CSSValue> getDurationValue(const AnimationList* animList)
{
    RefPtr<CSSValueList> list = CSSValueList::createCommaSeparated();
    if (animList) {
        for (size_t i = 0; i < animList->size(); ++i)
            list->append(cssValuePool().createValue(animList->animation(i)->duration(), CSSPrimitiveValue::CSS_S));
    } else {
        list->append(cssValuePool().createValue(Animation::initialAnimationDuration(), CSSPrimitiveValue::CSS_S));
    }
    return list.release();
}
