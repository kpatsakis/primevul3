void XSLStyleSheet::loadChildSheet(const String& href)
{
    OwnPtr<XSLImportRule> childRule = XSLImportRule::create(this, href);
    XSLImportRule* c = childRule.get();
    m_children.append(childRule.release());
    c->loadSheet();
}
