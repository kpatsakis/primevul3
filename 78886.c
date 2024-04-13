KURL Element::getURLAttribute(const QualifiedName& name) const
{
#if !ASSERT_DISABLED
    if (elementData()) {
        if (const Attribute* attribute = getAttributeItem(name))
            ASSERT(isURLAttribute(*attribute));
    }
#endif
    return document()->completeURL(stripLeadingAndTrailingHTMLSpaces(getAttribute(name)));
}
