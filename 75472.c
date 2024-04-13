void DocumentWriter::end()
{
    m_frame->loader()->didEndDocument();
    endIfNotLoadingMainResource();
}
