void EditorClientBlackBerry::requestCheckingOfString(PassRefPtr<TextCheckingRequest> testCheckingRequest)
{
    m_webPagePrivate->m_inputHandler->requestCheckingOfString(textCheckingRequest);
}
