void HTMLTextAreaElement::collectStyleForPresentationAttribute(const QualifiedName& name, const AtomicString& value, MutableStylePropertySet* style)
{
    if (name == wrapAttr) {
        if (shouldWrapText()) {
            addPropertyToPresentationAttributeStyle(style, CSSPropertyWhiteSpace, CSSValuePreWrap);
            addPropertyToPresentationAttributeStyle(style, CSSPropertyWordWrap, CSSValueBreakWord);
        } else {
            addPropertyToPresentationAttributeStyle(style, CSSPropertyWhiteSpace, CSSValuePre);
            addPropertyToPresentationAttributeStyle(style, CSSPropertyWordWrap, CSSValueNormal);
        }
    } else
        HTMLTextFormControlElement::collectStyleForPresentationAttribute(name, value, style);
}
