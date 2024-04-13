static void attributesStartElementNsHandler(void* closure, const xmlChar* xmlLocalName, const xmlChar* /*xmlPrefix*/,
                                            const xmlChar* /*xmlURI*/, int /*nb_namespaces*/, const xmlChar** /*namespaces*/,
                                            int nb_attributes, int /*nb_defaulted*/, const xmlChar** libxmlAttributes)
{
    if (strcmp(reinterpret_cast<const char*>(xmlLocalName), "attrs") != 0)
        return;
    
    xmlParserCtxtPtr ctxt = static_cast<xmlParserCtxtPtr>(closure);
    AttributeParseState* state = static_cast<AttributeParseState*>(ctxt->_private);
    
    state->gotAttributes = true;
    
    xmlSAX2Attributes* attributes = reinterpret_cast<xmlSAX2Attributes*>(libxmlAttributes);
    for(int i = 0; i < nb_attributes; i++) {
        String attrLocalName = toString(attributes[i].localname);
        int valueLength = (int) (attributes[i].end - attributes[i].value);
        String attrValue = toString(attributes[i].value, valueLength);
        String attrPrefix = toString(attributes[i].prefix);
        String attrQName = attrPrefix.isEmpty() ? attrLocalName : attrPrefix + ":" + attrLocalName;
        
        state->attributes.set(attrQName, attrValue);
    }
}
