void Element::reportMemoryUsage(MemoryObjectInfo* memoryObjectInfo) const
{
    MemoryClassInfo info(memoryObjectInfo, this, WebCoreMemoryTypes::DOM);
    ContainerNode::reportMemoryUsage(memoryObjectInfo);
    info.addMember(m_tagName, "tagName");
    info.addMember(m_elementData, "elementData");
}
