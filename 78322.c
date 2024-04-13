bool OmniboxEditModel::query_in_progress() const {
  return !autocomplete_controller()->done();
}
