void ElementData::reportMemoryUsage(MemoryObjectInfo* memoryObjectInfo) const
{
    size_t actualSize = m_isUnique ? sizeof(ElementData) : sizeForShareableElementDataWithAttributeCount(m_arraySize);
    MemoryClassInfo info(memoryObjectInfo, this, WebCoreMemoryTypes::DOM, actualSize);
    info.addMember(m_inlineStyle, "inlineStyle");
    info.addMember(m_classNames, "classNames");
    info.addMember(m_idForStyleResolution, "idForStyleResolution");
    if (m_isUnique) {
        const UniqueElementData* uniqueThis = static_cast<const UniqueElementData*>(this);
        info.addMember(uniqueThis->m_presentationAttributeStyle, "presentationAttributeStyle");
        info.addMember(uniqueThis->m_attributeVector, "attributeVector");
    }
    for (unsigned i = 0, len = length(); i < len; i++)
        info.addMember(*attributeItem(i), "*attributeItem");
}
