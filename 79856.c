GURL SavePackage::GetUrlToBeSaved() {
  NavigationEntry* active_entry =
      web_contents()->GetController().GetActiveEntry();
  return active_entry->GetURL();
}
