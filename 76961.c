static bool checkElementState(PlatformUIElement element, AtkStateType stateType)
{
    if (!ATK_IS_OBJECT(element))
        return false;

    GRefPtr<AtkStateSet> stateSet = adoptGRef(atk_object_ref_state_set(ATK_OBJECT(element)));
    return atk_state_set_contains_state(stateSet.get(), stateType);
}
