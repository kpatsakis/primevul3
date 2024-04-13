CJSON_PUBLIC(void) cJSON_ReplaceItemInObject(cJSON *object, const char *string, cJSON *newitem)
{
    replace_item_in_object(object, string, newitem, false);
}
