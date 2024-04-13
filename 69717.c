CJSON_PUBLIC(void) cJSON_AddItemToObjectCS(cJSON *object, const char *string, cJSON *item)
{
    add_item_to_object(object, string, item, &global_hooks, true);
}
