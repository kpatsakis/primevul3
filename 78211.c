scoped_ptr<base::MessagePump> CreateMessagePumpForUI() {
  return scoped_ptr<base::MessagePump>(new content::NestedMessagePumpAndroid());
}
