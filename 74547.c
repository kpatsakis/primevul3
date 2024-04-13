Frame* FrameLoader::createWindow(FrameLoader* frameLoaderForFrameLookup, const FrameLoadRequest& request, const WindowFeatures& features, bool& created)
{ 
    ASSERT(!features.dialog || request.frameName().isEmpty());

    if (!request.frameName().isEmpty() && request.frameName() != "_blank") {
        Frame* frame = frameLoaderForFrameLookup->frame()->tree()->find(request.frameName());
        if (frame && shouldAllowNavigation(frame)) {
            if (!request.resourceRequest().url().isEmpty())
                frame->loader()->loadFrameRequest(request, false, false, 0, 0, SendReferrer);
            if (Page* page = frame->page())
                page->chrome()->focus();
            created = false;
            return frame;
        }
    }
    
    FrameLoadRequest requestWithReferrer = request;
    requestWithReferrer.resourceRequest().setHTTPReferrer(m_outgoingReferrer);
    addHTTPOriginIfNeeded(requestWithReferrer.resourceRequest(), outgoingOrigin());

    Page* oldPage = m_frame->page();
    if (!oldPage)
        return 0;
        
    Page* page = oldPage->chrome()->createWindow(m_frame, requestWithReferrer, features);
    if (!page)
        return 0;

    Frame* frame = page->mainFrame();
    if (request.frameName() != "_blank")
        frame->tree()->setName(request.frameName());

    page->chrome()->setToolbarsVisible(features.toolBarVisible || features.locationBarVisible);
    page->chrome()->setStatusbarVisible(features.statusBarVisible);
    page->chrome()->setScrollbarsVisible(features.scrollbarsVisible);
    page->chrome()->setMenubarVisible(features.menuBarVisible);
    page->chrome()->setResizable(features.resizable);


    FloatRect windowRect = page->chrome()->windowRect();
    FloatSize pageSize = page->chrome()->pageRect().size();
    if (features.xSet)
        windowRect.setX(features.x);
    if (features.ySet)
        windowRect.setY(features.y);
    if (features.widthSet)
        windowRect.setWidth(features.width + (windowRect.width() - pageSize.width()));
    if (features.heightSet)
        windowRect.setHeight(features.height + (windowRect.height() - pageSize.height()));
    page->chrome()->setWindowRect(windowRect);

    page->chrome()->show();

    created = true;
    return frame;
}
