void Cache::setDisabled(bool disabled)
{
    m_disabled = disabled;
    if (!m_disabled)
        return;

    for (;;) {
        CachedResourceMap::iterator i = m_resources.begin();
        if (i == m_resources.end())
            break;
        remove(i->second);
    }
}
