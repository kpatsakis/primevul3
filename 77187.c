void ClearData(GtkClipboard* clipboard,
               gpointer user_data) {
  Clipboard::TargetMap* map =
      reinterpret_cast<Clipboard::TargetMap*>(user_data);
  std::set<char*> ptrs;

  for (Clipboard::TargetMap::iterator iter = map->begin();
       iter != map->end(); ++iter) {
    if (iter->first == kMimeTypeBitmap)
      g_object_unref(reinterpret_cast<GdkPixbuf*>(iter->second.first));
    else
      ptrs.insert(iter->second.first);
  }

  for (std::set<char*>::iterator iter = ptrs.begin();
       iter != ptrs.end(); ++iter) {
    delete[] *iter;
  }

  delete map;
}
