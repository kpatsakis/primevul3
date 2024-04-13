void ChromotingInstance::SendPerfStats() {
  if (!client_.get()) {
    return;
  }

  plugin_task_runner_->PostDelayedTask(
      FROM_HERE, base::Bind(&ChromotingInstance::SendPerfStats, AsWeakPtr()),
      base::TimeDelta::FromMilliseconds(kPerfStatsIntervalMs));

  scoped_ptr<base::DictionaryValue> data(new base::DictionaryValue());
  ChromotingStats* stats = client_->GetStats();
  data->SetDouble("videoBandwidth", stats->video_bandwidth()->Rate());
  data->SetDouble("videoFrameRate", stats->video_frame_rate()->Rate());
  data->SetDouble("captureLatency", stats->video_capture_ms()->Average());
  data->SetDouble("encodeLatency", stats->video_encode_ms()->Average());
  data->SetDouble("decodeLatency", stats->video_decode_ms()->Average());
  data->SetDouble("renderLatency", stats->video_paint_ms()->Average());
  data->SetDouble("roundtripLatency", stats->round_trip_ms()->Average());
  PostChromotingMessage("onPerfStats", data.Pass());
}
