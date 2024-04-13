bool JSArray::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    if (propertyName == exec->propertyNames().length) {
        slot.setValue(jsNumber(length()));
        return true;
    }

    bool isArrayIndex;
    unsigned i = propertyName.toArrayIndex(isArrayIndex);
    if (isArrayIndex)
        return JSArray::getOwnPropertySlot(exec, i, slot);

    return JSObject::getOwnPropertySlot(exec, propertyName, slot);
}
