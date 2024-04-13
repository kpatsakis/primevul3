AudioRendererMixerManager* RenderThreadImpl::GetAudioRendererMixerManager() {
  if (!audio_renderer_mixer_manager_) {
    audio_renderer_mixer_manager_.reset(new AudioRendererMixerManager(
        GetAudioHardwareConfig()));
  }

  return audio_renderer_mixer_manager_.get();
}
