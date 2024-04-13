void FrameLoader::finishedLoading()
{
    RefPtr<Frame> protect(m_frame);

    RefPtr<DocumentLoader> dl = activeDocumentLoader();
    dl->finishedLoading();
    if (!dl->mainDocumentError().isNull() || !dl->frameLoader())
        return;
    dl->setPrimaryLoadComplete(true);
    m_client->dispatchDidLoadMainResource(dl.get());
    checkLoadComplete();
}
