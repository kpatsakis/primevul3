void Examples::RandomShuffle() {
  std::iota(sampled_index_.begin(), sampled_index_.end(), 0);

  std::random_device rd;
  std::mt19937 rng(rd());
  std::shuffle(sampled_index_.begin(), sampled_index_.end(), rng);
}