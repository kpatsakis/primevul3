void AddHistogramSample(void* hist, int sample) {
  base::Histogram* histogram = static_cast<base::Histogram*>(hist);
  histogram->Add(sample);
}
