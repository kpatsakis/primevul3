void ChangeListLoader::RemoveObserver(ChangeListLoaderObserver* observer) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  observers_.RemoveObserver(observer);
}
