void FrameLoader::loadURLIntoChildFrame(const KURL& url, const String& referer, Frame* childFrame)
{
    ASSERT(childFrame);

    HistoryItem* parentItem = history()->currentItem();
    FrameLoadType loadType = this->loadType();
    FrameLoadType childLoadType = FrameLoadTypeRedirectWithLockedBackForwardList;

    KURL workingURL = url;
    
    if (parentItem && parentItem->children().size() && isBackForwardLoadType(loadType)) {
        HistoryItem* childItem = parentItem->childItemWithTarget(childFrame->tree()->name());
        if (childItem) {
            workingURL = KURL(ParsedURLString, childItem->originalURLString());
            childLoadType = loadType;
            childFrame->loader()->history()->setProvisionalItem(childItem);
        }
    }

    RefPtr<Archive> subframeArchive = activeDocumentLoader()->popArchiveForSubframe(childFrame->tree()->name());
    
    if (subframeArchive)
        childFrame->loader()->loadArchive(subframeArchive.release());
    else
        childFrame->loader()->loadURL(workingURL, referer, String(), false, childLoadType, 0, 0);
}
