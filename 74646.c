bool FrameLoader::shouldReload(const KURL& currentURL, const KURL& destinationURL)
{
#if ENABLE(WML)
    if (frameContainsWMLContent(m_frame))
        return true;
#endif

    if (!destinationURL.hasFragmentIdentifier())
        return true;
    return !equalIgnoringFragmentIdentifier(currentURL, destinationURL);
}
