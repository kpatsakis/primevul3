static void alterCurrentValue(PlatformUIElement element, int factor)
{
    if (!element || !ATK_IS_VALUE(element))
        return;

    GValue currentValue = G_VALUE_INIT;
    atk_value_get_current_value(ATK_VALUE(element), &currentValue);

    GValue increment = G_VALUE_INIT;
    atk_value_get_minimum_increment(ATK_VALUE(element), &increment);

    GValue newValue = G_VALUE_INIT;
    g_value_init(&newValue, G_TYPE_DOUBLE);

    g_value_set_float(&newValue, g_value_get_float(&currentValue) + factor * g_value_get_float(&increment));
    atk_value_set_current_value(ATK_VALUE(element), &newValue);

    g_value_unset(&newValue);
    g_value_unset(&increment);
     g_value_unset(&currentValue);
 }
