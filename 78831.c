static bool checkSelectorForClassChange(const SpaceSplitString& oldClasses, const SpaceSplitString& newClasses, const Checker& checker)
{
    unsigned oldSize = oldClasses.size();
    if (!oldSize)
        return checkSelectorForClassChange(newClasses, checker);
    BitVector remainingClassBits;
    remainingClassBits.ensureSize(oldSize);
    unsigned newSize = newClasses.size();
    for (unsigned i = 0; i < newSize; ++i) {
        for (unsigned j = 0; j < oldSize; ++j) {
            if (newClasses[i] == oldClasses[j]) {
                remainingClassBits.quickSet(j);
                continue;
            }
        }
        if (checker.hasSelectorForClass(newClasses[i]))
            return true;
    }
    for (unsigned i = 0; i < oldSize; ++i) {
        if (remainingClassBits.quickGet(i))
            continue;
        if (checker.hasSelectorForClass(oldClasses[i]))
            return true;
    }
    return false;
}
