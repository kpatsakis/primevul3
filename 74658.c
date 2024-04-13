void FrameLoader::stopLoadingSubframes()
{
    for (RefPtr<Frame> child = m_frame->tree()->firstChild(); child; child = child->tree()->nextSibling())
        child->loader()->stopAllLoaders();
}
