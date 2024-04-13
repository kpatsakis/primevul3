static xmlEntityPtr sharedXHTMLEntity()
{
    static xmlEntity entity;
    if (!entity.type) {
        entity.type = XML_ENTITY_DECL;
        entity.orig = sharedXHTMLEntityResult;
        entity.content = sharedXHTMLEntityResult;
        entity.etype = XML_INTERNAL_PREDEFINED_ENTITY;
    }
    return &entity;
}
