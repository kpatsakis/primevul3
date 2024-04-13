static void startElementNsHandler(void* closure, const xmlChar* localname, const xmlChar* prefix, const xmlChar* uri, int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted, const xmlChar** libxmlAttributes)
{
    if (hackAroundLibXMLEntityBug(closure))
        return;

    getTokenizer(closure)->startElementNs(localname, prefix, uri, nb_namespaces, namespaces, nb_attributes, nb_defaulted, libxmlAttributes);
}
