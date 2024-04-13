void base_audio_entry_dump(GF_AudioSampleEntryBox *p, FILE * trace)
{
	fprintf(trace, " DataReferenceIndex=\"%d\" SampleRate=\"%d\"", p->dataReferenceIndex, p->samplerate_hi);
	fprintf(trace, " Channels=\"%d\" BitsPerSample=\"%d\"", p->channel_count, p->bitspersample);
}
