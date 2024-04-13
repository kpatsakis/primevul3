  void FixResourceIdInChangeList(ChangeList* change_list) {
    std::vector<ResourceEntry>* entries = change_list->mutable_entries();
    std::vector<std::string>* parent_resource_ids =
        change_list->mutable_parent_resource_ids();
    for (size_t i = 0; i < entries->size(); ++i) {
      ResourceEntry* entry = &(*entries)[i];
      if (entry->has_resource_id())
        entry->set_resource_id(FixResourceId(entry->resource_id()));

      (*parent_resource_ids)[i] = FixResourceId((*parent_resource_ids)[i]);
    }
  }
