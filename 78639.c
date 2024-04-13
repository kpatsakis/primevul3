bool FrameLoader::allowPlugins(ReasonForCallingAllowPlugins reason)
{
    Settings* settings = m_frame->settings();
    bool allowed = m_client->allowPlugins(settings && settings->arePluginsEnabled());
    if (!allowed && reason == AboutToInstantiatePlugin)
        m_client->didNotAllowPlugins();
    return allowed;
}
