static void _ewk_view_smart_add_console_message(Ewk_View_Smart_Data* smartData, const char* message, unsigned int lineNumber, const char* sourceID)
{
    INF("console message: %s @%d: %s\n", sourceID, lineNumber, message);
}
