void RecordPercentageMatchHistogram(const base::string16& old_text,
                                    const base::string16& new_text,
                                    bool url_replacement_active,
                                    content::PageTransition transition,
                                    int omnibox_width) {
  size_t avg_length = (old_text.length() + new_text.length()) / 2;

  int percent = 0;
  if (!old_text.empty() && !new_text.empty()) {
    size_t shorter_length = std::min(old_text.length(), new_text.length());
    base::string16::const_iterator end(old_text.begin() + shorter_length);
    base::string16::const_iterator mismatch(
        std::mismatch(old_text.begin(), end, new_text.begin()).first);
    size_t matching_characters = mismatch - old_text.begin();
    percent = static_cast<float>(matching_characters) / avg_length * 100;
  }

  std::string histogram_name;
  if (url_replacement_active) {
    if (transition == content::PAGE_TRANSITION_TYPED) {
      histogram_name = "InstantExtended.PercentageMatchV2_QuerytoURL";
      UMA_HISTOGRAM_PERCENTAGE(histogram_name, percent);
    } else {
      histogram_name = "InstantExtended.PercentageMatchV2_QuerytoQuery";
      UMA_HISTOGRAM_PERCENTAGE(histogram_name, percent);
    }
  } else {
    if (transition == content::PAGE_TRANSITION_TYPED) {
      histogram_name = "InstantExtended.PercentageMatchV2_URLtoURL";
      UMA_HISTOGRAM_PERCENTAGE(histogram_name, percent);
    } else {
      histogram_name = "InstantExtended.PercentageMatchV2_URLtoQuery";
      UMA_HISTOGRAM_PERCENTAGE(histogram_name, percent);
    }
  }

  std::string suffix = "large";
  for (size_t i = 0; i < arraysize(kPercentageMatchHistogramWidthBuckets);
       ++i) {
    if (omnibox_width < kPercentageMatchHistogramWidthBuckets[i]) {
      suffix = base::IntToString(kPercentageMatchHistogramWidthBuckets[i]);
      break;
    }
  }

  base::HistogramBase* counter = base::LinearHistogram::FactoryGet(
      histogram_name + "_" + suffix, 1, 101, 102,
      base::Histogram::kUmaTargetedHistogramFlag);
  counter->Add(percent);
}
