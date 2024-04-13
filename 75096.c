bool NotificationService::HasKey(const NotificationSourceMap& map,
                                 const NotificationSource& source) {
  return map.find(source.map_key()) != map.end();
}
