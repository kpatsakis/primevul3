static bool checkSelectorForClassChange(const SpaceSplitString& changedClasses, const Checker& checker)
{
    unsigned changedSize = changedClasses.size();
    for (unsigned i = 0; i < changedSize; ++i) {
        if (checker.hasSelectorForClass(changedClasses[i]))
            return true;
    }
    return false;
}
