void JSArray::put(ExecState* exec, const Identifier& propertyName, JSValue value, PutPropertySlot& slot)
{
    bool isArrayIndex;
    unsigned i = propertyName.toArrayIndex(isArrayIndex);
    if (isArrayIndex) {
        put(exec, i, value);
        return;
    }

    if (propertyName == exec->propertyNames().length) {
        unsigned newLength = value.toUInt32(exec);
        if (value.toNumber(exec) != static_cast<double>(newLength)) {
            throwError(exec, createRangeError(exec, "Invalid array length"));
            return;
        }
        setLength(newLength);
        return;
    }

    JSObject::put(exec, propertyName, value, slot);
}
