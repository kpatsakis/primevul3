  void QuantizeSlice(const Device& d, OpKernelContext* ctx,
                     const ConstVec& input, float input_min_range,
                     float input_max_range, Vec output, float* output_min_range,
                     float* output_max_range) {
    OP_REQUIRES(ctx, !(input_max_range < input_min_range),
                errors::InvalidArgument(
                    "input_max_range must be larger than input_min_range."));
    float min_range = std::min(0.0f, input_min_range);
    const float epsilon = std::max(1.0f, std::max(fabsf(input_min_range),
                                                  fabsf(input_max_range))) *
                          ensure_minimum_range_;
    float max_range =
        std::max(0.0f, std::max(input_max_range, min_range + epsilon));

    if (mode_ == QUANTIZE_MODE_MIN_COMBINED) {
      const float scale_factor =
          (static_cast<double>(std::numeric_limits<T>::max()) -
           static_cast<double>(std::numeric_limits<T>::min())) /
          (max_range - min_range);

      // Quantize:
      // Make input in range of [min_range, max_range], then
      // subtract min_range to be in range of [0, max_range - min_range]
      // Divide by (max_range - min_range) to get to [0, 1.0]
      // Multiply by range of T, after that shift left 1/2 range of T if
      // T is signed.
      // Note that the number is rounded before the cast. Rounding follows the
      // semantic of std::round, which implements "round-half-away-zero",
      // e.g., -5.5 gets rounded to -6, -5.4 goes to -5, 5.4 goes to 5,
      // and 5.5 goes to 6.
      bool is_signed = std::is_signed<T>::value;
      if (is_signed) {
        // The slow path.
        // TODO(xbing,yonghui): Speedup this path as well.
        output.device(d) =
            ((input.cwiseMin(max_range).cwiseMax(min_range) - min_range) *
                 scale_factor -
             half_range_)
                .round()
                .template cast<T>();
      } else {
        // The fast path that avoids unaryExpr
        // According to the micro-benchmark, adding device here doesn't help.
        output.device(d) =
            ((input.cwiseMin(max_range).cwiseMax(min_range) - min_range) *
                 scale_factor +
             0.5f)
                .template cast<T>();
      }
    } else if (mode_ == QUANTIZE_MODE_SCALED) {
      const int min_output_value =
          std::numeric_limits<T>::min() + (narrow_range_ ? 1 : 0);
      const int max_output_value = std::numeric_limits<T>::max();
      const float scale_factor_from_min_side =
          (min_output_value * min_range > 0)
              ? min_output_value / min_range
              : std::numeric_limits<float>::max();
      const float scale_factor_from_max_side =
          (max_output_value * max_range > 0)
              ? max_output_value / max_range
              : std::numeric_limits<float>::max();
      const float scale_factor =
          std::min(scale_factor_from_min_side, scale_factor_from_max_side);
      min_range = min_output_value / scale_factor;
      max_range = max_output_value / scale_factor;
      if (round_mode_ == ROUND_HALF_TO_EVEN) {
        output.device(d) =
            (input.cwiseMin(max_range).cwiseMax(min_range) * scale_factor)
                .unaryExpr(
                    Eigen::internal::scalar_round_half_to_even_op<float>())
                .template cast<T>();
      } else if (round_mode_ == ROUND_HALF_AWAY_FROM_ZERO) {
        output.device(d) =
            (input.cwiseMin(max_range).cwiseMax(min_range) * scale_factor)
                .round()
                .template cast<T>();
      }
    }

    *output_min_range = min_range;
    *output_max_range = max_range;
  }