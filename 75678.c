PassRefPtr<CSSValueList> CSSComputedStyleDeclaration::getBackgroundShorthandValue() const
{
    static const CSSPropertyID propertiesBeforeSlashSeperator[5] = { CSSPropertyBackgroundColor, CSSPropertyBackgroundImage,
                                                                     CSSPropertyBackgroundRepeat, CSSPropertyBackgroundAttachment,  
                                                                     CSSPropertyBackgroundPosition };
    static const CSSPropertyID propertiesAfterSlashSeperator[3] = { CSSPropertyBackgroundSize, CSSPropertyBackgroundOrigin, 
                                                                    CSSPropertyBackgroundClip };

    RefPtr<CSSValueList> list = CSSValueList::createSlashSeparated();
    list->append(getCSSPropertyValuesForShorthandProperties(StylePropertyShorthand(propertiesBeforeSlashSeperator, WTF_ARRAY_LENGTH(propertiesBeforeSlashSeperator))));
    list->append(getCSSPropertyValuesForShorthandProperties(StylePropertyShorthand(propertiesAfterSlashSeperator, WTF_ARRAY_LENGTH(propertiesAfterSlashSeperator))));
    return list.release();
}
