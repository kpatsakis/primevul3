void FrameLoader::insertDummyHistoryItem()
{
    RefPtr<HistoryItem> currentItem = HistoryItem::create();
    history()->setCurrentItem(currentItem.get());
    frame()->page()->backForward().setCurrentItem(currentItem.get());
}
