bool FrameLoader::representationExistsForURLScheme(const String& URLScheme)
{
    return m_client->representationExistsForURLScheme(URLScheme);
}
