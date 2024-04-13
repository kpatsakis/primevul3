static xmlEntityPtr getEntityHandler(void* closure, const xmlChar* name)
{
    xmlParserCtxtPtr ctxt = static_cast<xmlParserCtxtPtr>(closure);
    xmlEntityPtr ent = xmlGetPredefinedEntity(name);
    if (ent) {
        ent->etype = XML_INTERNAL_PREDEFINED_ENTITY;
        return ent;
    }

    ent = xmlGetDocEntity(ctxt->myDoc, name);
    if (!ent && (getTokenizer(closure)->isXHTMLDocument()
#if ENABLE(WML)
                 || getTokenizer(closure)->isWMLDocument()
#endif
       )) {
        ent = getXHTMLEntity(name);
        if (ent)
            ent->etype = XML_INTERNAL_GENERAL_ENTITY;
    }
    
    return ent;
}
