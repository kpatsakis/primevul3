bool NavigationController::IsInitialNavigation() {
  return last_document_loaded_.is_null();
}
