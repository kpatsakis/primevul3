static int mac80211_hwsim_get_survey(struct ieee80211_hw *hw, int idx,
				     struct survey_info *survey)
{
	struct mac80211_hwsim_data *hwsim = hw->priv;

	if (idx < 0 || idx >= ARRAY_SIZE(hwsim->survey_data))
		return -ENOENT;

	mutex_lock(&hwsim->mutex);
	survey->channel = hwsim->survey_data[idx].channel;
	if (!survey->channel) {
		mutex_unlock(&hwsim->mutex);
		return -ENOENT;
	}

	/*
	 * Magically conjured dummy values --- this is only ok for simulated hardware.
	 *
	 * A real driver which cannot determine real values noise MUST NOT
	 * report any, especially not a magically conjured ones :-)
	 */
	survey->filled = SURVEY_INFO_NOISE_DBM |
			 SURVEY_INFO_TIME |
			 SURVEY_INFO_TIME_BUSY;
	survey->noise = -92;
	survey->time =
		jiffies_to_msecs(hwsim->survey_data[idx].end -
				 hwsim->survey_data[idx].start);
	/* report 12.5% of channel time is used */
	survey->time_busy = survey->time/8;
	mutex_unlock(&hwsim->mutex);

	return 0;
}
