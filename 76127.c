static Eina_Bool _ewk_view_smart_run_javascript_prompt(Ewk_View_Smart_Data* smartData, Evas_Object* frame, const char* message, const char* defaultValue, char** value)
{
    *value = strdup("test");
    Eina_Bool result = true;
    INF("javascript prompt:\n"
        "\t      message: %s\n"
        "\tdefault value: %s\n"
        "\tgiving answer: %s\n"
        "\t       button: %s", message, defaultValue, *value, result ? "ok" : "cancel");

    return result;
}
