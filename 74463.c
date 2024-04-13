static xmlEntityPtr getXHTMLEntity(const xmlChar* name)
{
    UChar c = decodeNamedEntity(reinterpret_cast<const char*>(name));
    if (!c)
        return 0;

    CString value = String(&c, 1).utf8();
    ASSERT(value.length() < 5);
    xmlEntityPtr entity = sharedXHTMLEntity();
    entity->length = value.length();
    entity->name = name;
    memcpy(sharedXHTMLEntityResult, value.data(), entity->length + 1);

    return entity;
}
