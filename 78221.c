PassRefPtr<Element> ApplyBlockElementCommand::createBlockElement() const
{
    RefPtr<Element> element = createHTMLElement(document(), m_tagName);
    if (m_inlineStyle.length())
        element->setAttribute(styleAttr, m_inlineStyle);
    return element.release();
}
