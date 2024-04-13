void FrameLoader::commitIconURLToIconDatabase(const KURL& icon)
{
    ASSERT(iconDatabase());
    LOG(IconDatabase, "Committing iconURL %s to database for pageURLs %s and %s", icon.string().ascii().data(), m_URL.string().ascii().data(), originalRequestURL().string().ascii().data());
    iconDatabase()->setIconURLForPageURL(icon.string(), m_URL.string());
    iconDatabase()->setIconURLForPageURL(icon.string(), originalRequestURL().string());
}
