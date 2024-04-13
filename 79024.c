const AtomicString& Element::webkitRegionOverset() const
{
    document()->updateLayoutIgnorePendingStylesheets();

    DEFINE_STATIC_LOCAL(AtomicString, undefinedState, ("undefined", AtomicString::ConstructFromLiteral));
    if (!RuntimeEnabledFeatures::cssRegionsEnabled() || !renderRegion())
        return undefinedState;

    switch (renderRegion()->regionOversetState()) {
    case RegionFit: {
        DEFINE_STATIC_LOCAL(AtomicString, fitState, ("fit", AtomicString::ConstructFromLiteral));
        return fitState;
    }
    case RegionEmpty: {
        DEFINE_STATIC_LOCAL(AtomicString, emptyState, ("empty", AtomicString::ConstructFromLiteral));
        return emptyState;
    }
    case RegionOverset: {
        DEFINE_STATIC_LOCAL(AtomicString, overflowState, ("overset", AtomicString::ConstructFromLiteral));
        return overflowState;
    }
    case RegionUndefined:
        return undefinedState;
    }

    ASSERT_NOT_REACHED();
    return undefinedState;
}
