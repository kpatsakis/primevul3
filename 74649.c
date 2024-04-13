bool FrameLoader::shouldTreatURLAsSameAsCurrent(const KURL& url) const
{
    if (!history()->currentItem())
        return false;
    return url == history()->currentItem()->url() || url == history()->currentItem()->originalURL();
}
