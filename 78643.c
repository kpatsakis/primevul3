void FrameLoader::checkLoadComplete(DocumentLoader* documentLoader)
{
    if (documentLoader)
        documentLoader->checkLoadComplete();
    checkLoadComplete();
}
