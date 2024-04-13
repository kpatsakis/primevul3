void HTMLFormElement::removeImgElement(HTMLImageElement* e)
{
    ASSERT(m_imageElements.find(e) != kNotFound);
    removeFromPastNamesMap(*e);
    removeFromVector(m_imageElements, e);
}
