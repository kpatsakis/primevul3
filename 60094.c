assert_circuit_mux_okay(channel_t *chan)
{
  tor_assert(chan);
  tor_assert(chan->cmux);

  circuitmux_assert_okay(chan->cmux);
}
