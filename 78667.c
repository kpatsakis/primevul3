void FrameLoader::handleFallbackContent()
{
    HTMLFrameOwnerElement* owner = m_frame->ownerElement();
    if (!owner || !owner->hasTagName(objectTag))
        return;
    toHTMLObjectElement(owner)->renderFallbackContent();
}
