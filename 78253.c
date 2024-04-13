void HTMLFormElement::registerImgElement(HTMLImageElement* e)
{
    ASSERT(m_imageElements.find(e) == kNotFound);
    m_imageElements.append(e);
}
