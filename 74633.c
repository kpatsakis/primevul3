void FrameLoader::setEncoding(const String& name, bool userChosen)
{
    if (!m_workingURL.isEmpty())
        receivedFirstData();
    m_encoding = name;
    m_encodingWasChosenByUser = userChosen;
}
