 bool FrameLoader::isLoading() const
 {
     DocumentLoader* docLoader = activeDocumentLoader();
    if (!docLoader)
        return false;
    return docLoader->isLoading();
}
