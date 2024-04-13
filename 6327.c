Status Examples::SampleAdaptiveProbabilities(
    const int num_loss_partitions, const Regularizations& regularization,
    const ModelWeights& model_weights,
    const TTypes<float>::Matrix example_state_data,
    const std::unique_ptr<DualLossUpdater>& loss_updater,
    const int num_weight_vectors) {
  if (num_weight_vectors != 1) {
    return errors::InvalidArgument(
        "Adaptive SDCA only works with binary SDCA, "
        "where num_weight_vectors should be 1.");
  }
  // Compute the probabilities
  for (int example_id = 0; example_id < num_examples(); ++example_id) {
    const Example& example = examples_[example_id];
    const double example_weight = example.example_weight();
    float label = example.example_label();
    const Status conversion_status = loss_updater->ConvertLabel(&label);
    const ExampleStatistics example_statistics =
        example.ComputeWxAndWeightedExampleNorm(num_loss_partitions,
                                                model_weights, regularization,
                                                num_weight_vectors);
    const double kappa = example_state_data(example_id, 0) +
                         loss_updater->PrimalLossDerivative(
                             example_statistics.wx[0], label, 1.0);
    probabilities_[example_id] = example_weight *
                                 sqrt(examples_[example_id].squared_norm_ +
                                      regularization.symmetric_l2() *
                                          loss_updater->SmoothnessConstant()) *
                                 std::abs(kappa);
  }

  // Sample the index
  random::DistributionSampler sampler(probabilities_);
  GuardedPhiloxRandom generator;
  generator.Init(0, 0);
  auto local_gen = generator.ReserveSamples32(num_examples());
  random::SimplePhilox random(&local_gen);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, 1);

  // We use a decay of 10: the probability of an example is divided by 10
  // once that example is picked. A good approximation of that is to only
  // keep a picked example with probability (1 / 10) ^ k where k is the
  // number of times we already picked that example. We add a num_retries
  // to avoid taking too long to sample. We then fill the sampled_index with
  // unseen examples sorted by probabilities.
  int id = 0;
  int num_retries = 0;
  while (id < num_examples() && num_retries < num_examples()) {
    int picked_id = sampler.Sample(&random);
    if (dis(gen) > MathUtil::IPow(0.1, sampled_count_[picked_id])) {
      num_retries++;
      continue;
    }
    sampled_count_[picked_id]++;
    sampled_index_[id++] = picked_id;
  }

  std::vector<std::pair<int, float>> examples_not_seen;
  examples_not_seen.reserve(num_examples());
  for (int i = 0; i < num_examples(); ++i) {
    if (sampled_count_[i] == 0)
      examples_not_seen.emplace_back(sampled_index_[i], probabilities_[i]);
  }
  std::sort(
      examples_not_seen.begin(), examples_not_seen.end(),
      [](const std::pair<int, float>& lhs, const std::pair<int, float>& rhs) {
        return lhs.second > rhs.second;
      });
  for (int i = id; i < num_examples(); ++i) {
    sampled_count_[i] = examples_not_seen[i - id].first;
  }
  return Status::OK();
}