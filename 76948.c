static gchar* attributeSetToString(AtkAttributeSet* attributeSet)
{
    GString* str = g_string_new(0);
    for (GSList* attributes = attributeSet; attributes; attributes = attributes->next) {
        AtkAttribute* attribute = static_cast<AtkAttribute*>(attributes->data);
        GOwnPtr<gchar> attributeData(g_strconcat(attribute->name, ":", attribute->value, NULL));
        g_string_append(str, attributeData.get());
        if (attributes->next)
            g_string_append(str, ", ");
    }

    return g_string_free(str, FALSE);
}
