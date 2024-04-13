static int snd_pcm_oss_sync(struct snd_pcm_oss_file *pcm_oss_file)
{
	int err = 0;
	unsigned int saved_f_flags;
	struct snd_pcm_substream *substream;
	struct snd_pcm_runtime *runtime;
	snd_pcm_format_t format;
	unsigned long width;
	size_t size;

	substream = pcm_oss_file->streams[SNDRV_PCM_STREAM_PLAYBACK];
	if (substream != NULL) {
		runtime = substream->runtime;
		if (atomic_read(&substream->mmap_count))
			goto __direct;
		err = snd_pcm_oss_make_ready(substream);
		if (err < 0)
			return err;
		atomic_inc(&runtime->oss.rw_ref);
		if (mutex_lock_interruptible(&runtime->oss.params_lock)) {
			atomic_dec(&runtime->oss.rw_ref);
			return -ERESTARTSYS;
		}
		format = snd_pcm_oss_format_from(runtime->oss.format);
		width = snd_pcm_format_physical_width(format);
		if (runtime->oss.buffer_used > 0) {
#ifdef OSS_DEBUG
			pcm_dbg(substream->pcm, "sync: buffer_used\n");
#endif
			size = (8 * (runtime->oss.period_bytes - runtime->oss.buffer_used) + 7) / width;
			snd_pcm_format_set_silence(format,
						   runtime->oss.buffer + runtime->oss.buffer_used,
						   size);
			err = snd_pcm_oss_sync1(substream, runtime->oss.period_bytes);
			if (err < 0)
				goto unlock;
		} else if (runtime->oss.period_ptr > 0) {
#ifdef OSS_DEBUG
			pcm_dbg(substream->pcm, "sync: period_ptr\n");
#endif
			size = runtime->oss.period_bytes - runtime->oss.period_ptr;
			snd_pcm_format_set_silence(format,
						   runtime->oss.buffer,
						   size * 8 / width);
			err = snd_pcm_oss_sync1(substream, size);
			if (err < 0)
				goto unlock;
		}
		/*
		 * The ALSA's period might be a bit large than OSS one.
		 * Fill the remain portion of ALSA period with zeros.
		 */
		size = runtime->control->appl_ptr % runtime->period_size;
		if (size > 0) {
			size = runtime->period_size - size;
			if (runtime->access == SNDRV_PCM_ACCESS_RW_INTERLEAVED)
				snd_pcm_lib_write(substream, NULL, size);
			else if (runtime->access == SNDRV_PCM_ACCESS_RW_NONINTERLEAVED)
				snd_pcm_lib_writev(substream, NULL, size);
		}
unlock:
		mutex_unlock(&runtime->oss.params_lock);
		atomic_dec(&runtime->oss.rw_ref);
		if (err < 0)
			return err;
		/*
		 * finish sync: drain the buffer
		 */
	      __direct:
		saved_f_flags = substream->f_flags;
		substream->f_flags &= ~O_NONBLOCK;
		err = snd_pcm_kernel_ioctl(substream, SNDRV_PCM_IOCTL_DRAIN, NULL);
		substream->f_flags = saved_f_flags;
		if (err < 0)
			return err;
		mutex_lock(&runtime->oss.params_lock);
		runtime->oss.prepare = 1;
		mutex_unlock(&runtime->oss.params_lock);
	}

	substream = pcm_oss_file->streams[SNDRV_PCM_STREAM_CAPTURE];
	if (substream != NULL) {
		err = snd_pcm_oss_make_ready(substream);
		if (err < 0)
			return err;
		runtime = substream->runtime;
		err = snd_pcm_kernel_ioctl(substream, SNDRV_PCM_IOCTL_DROP, NULL);
		if (err < 0)
			return err;
		mutex_lock(&runtime->oss.params_lock);
		runtime->oss.buffer_used = 0;
		runtime->oss.prepare = 1;
		mutex_unlock(&runtime->oss.params_lock);
	}
	return 0;
}