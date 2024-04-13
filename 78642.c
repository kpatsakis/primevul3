void FrameLoader::checkLoadComplete()
{
    ASSERT(m_client->hasWebView());

    if (Page* page = m_frame->page()) {
        Vector<RefPtr<Frame>, 10> frames;
        for (RefPtr<Frame> frame = page->mainFrame(); frame; frame = frame->tree()->traverseNext())
            frames.append(frame);
        for (size_t i = frames.size(); i; --i)
            frames[i - 1]->loader()->checkLoadCompleteForThisFrame();
    }
}
