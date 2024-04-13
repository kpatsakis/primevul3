void DocumentWriter::setEncoding(const String& name, bool userChosen)
{
    m_frame->loader()->willSetEncoding();
    m_encoding = name;
     m_encodingWasChosenByUser = userChosen;
 }
