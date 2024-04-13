void FrameLoader::checkIfRunInsecureContent(SecurityOrigin* context, const KURL& url)
{
    if (!isMixedContent(context, url))
        return;

    String message = String::format("The page at %s ran insecure content from %s.\n",
        m_URL.string().utf8().data(), url.string().utf8().data());
    m_frame->domWindow()->console()->addMessage(HTMLMessageSource, LogMessageType, WarningMessageLevel, message, 1, String());

    m_client->didRunInsecureContent(context);
}
