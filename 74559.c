void FrameLoader::end()
{
    m_isLoadingMainResource = false;
    endIfNotLoadingMainResource();
}
