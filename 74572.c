void FrameLoader::iconLoadDecisionAvailable()
{
    if (!m_mayLoadIconLater)
        return;
    LOG(IconDatabase, "FrameLoader %p was told a load decision is available for its icon", this);
    startIconLoader();
    m_mayLoadIconLater = false;
}
