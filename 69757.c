CJSON_PUBLIC(char *) cJSON_Print(const cJSON *item)
{
    return (char*)print(item, true, &global_hooks);
}
