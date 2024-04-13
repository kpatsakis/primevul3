bool FrameLoader::isHostedByObjectElement() const
{
    HTMLFrameOwnerElement* owner = m_frame->ownerElement();
    return owner && owner->hasTagName(objectTag);
}
