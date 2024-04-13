void ImageLoader::reportMemoryUsage(MemoryObjectInfo* memoryObjectInfo) const
{
    MemoryClassInfo info(memoryObjectInfo, this, WebCoreMemoryTypes::Image);
    info.addMember(m_element, "element");
    info.addMember(m_image.get(), "image", WTF::RetainingPointer);
    info.addMember(m_derefElementTimer, "derefElementTimer");
    info.addMember(m_failedLoadURL, "failedLoadURL");
}
