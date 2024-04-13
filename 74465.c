static inline void handleElementAttributes(Element* newElement, const xmlChar** libxmlAttributes, int nb_attributes, ExceptionCode& ec)
{
    xmlSAX2Attributes* attributes = reinterpret_cast<xmlSAX2Attributes*>(libxmlAttributes);
    for(int i = 0; i < nb_attributes; i++) {
        String attrLocalName = toString(attributes[i].localname);
        int valueLength = (int) (attributes[i].end - attributes[i].value);
        String attrValue = toString(attributes[i].value, valueLength);
        String attrPrefix = toString(attributes[i].prefix);
        String attrURI = attrPrefix.isEmpty() ? String() : toString(attributes[i].uri);
        String attrQName = attrPrefix.isEmpty() ? attrLocalName : attrPrefix + ":" + attrLocalName;
        
        newElement->setAttributeNS(attrURI, attrQName, attrValue, ec);
        if (ec) // exception setting attributes
            return;
    }
}
