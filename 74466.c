static inline void handleElementNamespaces(Element* newElement, const xmlChar** libxmlNamespaces, int nb_namespaces, ExceptionCode& ec)
{
    xmlSAX2Namespace* namespaces = reinterpret_cast<xmlSAX2Namespace*>(libxmlNamespaces);
    for(int i = 0; i < nb_namespaces; i++) {
        String namespaceQName = "xmlns";
        String namespaceURI = toString(namespaces[i].uri);
        if (namespaces[i].prefix)
            namespaceQName = "xmlns:" + toString(namespaces[i].prefix);
        newElement->setAttributeNS("http://www.w3.org/2000/xmlns/", namespaceQName, namespaceURI, ec);
        if (ec) // exception setting attributes
            return;
    }
}
