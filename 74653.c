void FrameLoader::startIconLoader()
{
    if (!isLoadingMainFrame())
        return;

    if (!iconDatabase() || !iconDatabase()->isEnabled())
        return;
    
    KURL url(iconURL());
    String urlString(url.string());
    if (urlString.isEmpty())
        return;

    if (loadType() != FrameLoadTypeReload && loadType() != FrameLoadTypeReloadFromOrigin) {
        IconLoadDecision decision = iconDatabase()->loadDecisionForIconURL(urlString, m_documentLoader.get());
        if (decision == IconLoadNo) {
            LOG(IconDatabase, "FrameLoader::startIconLoader() - Told not to load this icon, committing iconURL %s to database for pageURL mapping", urlString.ascii().data());
            commitIconURLToIconDatabase(url);
            
            if (!iconDatabase()->iconDataKnownForIconURL(urlString)) {
                LOG(IconDatabase, "Told not to load icon %s but icon data is not yet available - registering for notification and requesting load from disk", urlString.ascii().data());
                m_client->registerForIconNotification();
                iconDatabase()->iconForPageURL(m_URL.string(), IntSize(0, 0));
                iconDatabase()->iconForPageURL(originalRequestURL().string(), IntSize(0, 0));
            } else
                m_client->dispatchDidReceiveIcon();
                
            return;
        } 
        
        if (decision == IconLoadUnknown) {
            LOG(IconDatabase, "FrameLoader %p might load icon %s later", this, urlString.ascii().data());
            m_mayLoadIconLater = true;    
            m_client->registerForIconNotification();
            commitIconURLToIconDatabase(url);
            return;
        }
    }

    Settings* settings = m_frame->settings();
    if (settings && !settings->loadsImagesAutomatically())
        return;

    if (!m_iconLoader)
        m_iconLoader.set(IconLoader::create(m_frame).release());
        
    m_iconLoader->startLoading();
}
