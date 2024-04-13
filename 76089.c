static Eina_Bool _ewk_view_editor_command(Ewk_View_Private_Data* priv, const char* command, const char* value = 0)
{
    return priv->page->focusController()->focusedOrMainFrame()->editor()->command(WTF::String::fromUTF8(command)).execute(WTF::String::fromUTF8(value));
}
