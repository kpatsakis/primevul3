void FrameLoader::prepareForHistoryNavigation()
{
    RefPtr<HistoryItem> currentItem = history()->currentItem();
    if (!currentItem) {
        insertDummyHistoryItem();
        ASSERT(stateMachine()->isDisplayingInitialEmptyDocument());
        stateMachine()->advanceTo(FrameLoaderStateMachine::CommittedFirstRealLoad);
    }
}
