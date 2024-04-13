void FrameLoader::logCanCachePageDecision()
{
    if (m_creatingInitialEmptyDocument)
        return;
    KURL currentURL = m_documentLoader ? m_documentLoader->url() : KURL();
    if (currentURL.isEmpty())
        return;

    int indentLevel = 0;
    PCLOG("--------\n Determining if page can be cached:");

    bool cannotCache = !logCanCacheFrameDecision(1);
        
    FrameLoadType loadType = this->loadType();
    do {
        if (m_frame->tree()->parent())
            { PCLOG("   -Frame has a parent frame"); cannotCache = true; }
        if (!m_frame->page()) {
            PCLOG("   -There is no Page object");
            cannotCache = true;
            break;
        }
        if (!m_frame->page()->backForwardList()->enabled())
            { PCLOG("   -The back/forward list is disabled"); cannotCache = true; }
        if (!(m_frame->page()->backForwardList()->capacity() > 0))
            { PCLOG("   -The back/forward list has a 0 capacity"); cannotCache = true; }
        if (!m_frame->page()->settings()->usesPageCache())
            { PCLOG("   -Page settings says b/f cache disabled"); cannotCache = true; }
        if (loadType == FrameLoadTypeReload)
            { PCLOG("   -Load type is: Reload"); cannotCache = true; }
        if (loadType == FrameLoadTypeReloadFromOrigin)
            { PCLOG("   -Load type is: Reload from origin"); cannotCache = true; }
        if (loadType == FrameLoadTypeSame)
            { PCLOG("   -Load type is: Same"); cannotCache = true; }
    } while (false);
    
    PCLOG(cannotCache ? " Page CANNOT be cached\n--------" : " Page CAN be cached\n--------");
}
