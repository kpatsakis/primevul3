void CSSStyleSheet::checkLoaded()
{
    if (isLoading())
        return;
    if (parent())
        parent()->checkLoaded();

    RefPtr<CSSStyleSheet> protector(this);
    m_loadCompleted = ownerNode() ? ownerNode()->sheetLoaded() : true;
}
