const char* EditorClientBlackBerry::interpretKeyEvent(const KeyboardEvent* event)
{
    ASSERT(event->type() == eventNames().keydownEvent || event->type() == eventNames().keypressEvent);

    static HashMap<int, const char*>* keyDownCommandsMap = 0;
    static HashMap<int, const char*>* keyPressCommandsMap = 0;

    if (!keyDownCommandsMap) {
        keyDownCommandsMap = new HashMap<int, const char*>;
        keyPressCommandsMap = new HashMap<int, const char*>;

        for (size_t i = 0; i < WTF_ARRAY_LENGTH(keyDownEntries); ++i)
            keyDownCommandsMap->set(keyDownEntries[i].modifiers << 16 | keyDownEntries[i].virtualKey, keyDownEntries[i].name);

        for (size_t i = 0; i < WTF_ARRAY_LENGTH(keyPressEntries); ++i)
            keyPressCommandsMap->set(keyPressEntries[i].modifiers << 16 | keyPressEntries[i].charCode, keyPressEntries[i].name);
    }

    unsigned modifiers = 0;
    if (event->shiftKey())
        modifiers |= ShiftKey;
    if (event->altKey())
        modifiers |= AltKey;
    if (event->ctrlKey())
        modifiers |= CtrlKey;

    if (event->type() == eventNames().keydownEvent) {
        int mapKey = modifiers << 16 | event->keyCode();
        return mapKey ? keyDownCommandsMap->get(mapKey) : 0;
    }

    int mapKey = modifiers << 16 | event->charCode();
    return mapKey ? keyPressCommandsMap->get(mapKey) : 0;
}
