String FrameLoader::referrer() const
{
    return documentLoader()->request().httpReferrer();
}
