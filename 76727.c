Window* ConsumerToWindow(ui::GestureConsumer* consumer) {
  return consumer && !consumer->ignores_events() ?
      static_cast<Window*>(consumer) : NULL;
}
