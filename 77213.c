void Clipboard::WriteObjects(const ObjectMap& objects) {
  clipboard_data_ = new TargetMap();

  for (ObjectMap::const_iterator iter = objects.begin();
       iter != objects.end(); ++iter) {
    DispatchObject(static_cast<ObjectType>(iter->first), iter->second);
  }

  SetGtkClipboard();
}
