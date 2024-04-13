void FrameLoader::loadArchive(PassRefPtr<Archive> prpArchive)
{
    RefPtr<Archive> archive = prpArchive;
    
    ArchiveResource* mainResource = archive->mainResource();
    ASSERT(mainResource);
    if (!mainResource)
        return;
        
    SubstituteData substituteData(mainResource->data(), mainResource->mimeType(), mainResource->textEncoding(), KURL());
    
    ResourceRequest request(mainResource->url());
#if PLATFORM(MAC)
    request.applyWebArchiveHackForMail();
#endif

    RefPtr<DocumentLoader> documentLoader = m_client->createDocumentLoader(request, substituteData);
    documentLoader->addAllArchiveResources(archive.get());
    load(documentLoader.get());
}
