channel_unlink_all_circuits(channel_t *chan)
{
  tor_assert(chan);
  tor_assert(chan->cmux);

  circuitmux_detach_all_circuits(chan->cmux);
  chan->num_n_circuits = 0;
  chan->num_p_circuits = 0;
}
