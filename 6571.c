static void PrintSplitUsage()
{
	u32 i=0;
	gf_sys_format_help(helpout, help_flags, "  \n"
		"# File splitting\n"
		"MP4Box can split input files by size, duration or extract a given part of the file to new IsoMedia file(s).\n"
		"This requires that at most one track in the input file has non random-access points (typically one video track at most).\n"
		"Splitting will ignore all MPEG-4 Systems tracks and hint tracks, but will try to split private media tracks.\n"
		"The input file must have enough random access points in order to be split. If this is not the case, you will have to re-encode the content.\n"
		"You can add media to a file and split it in the same pass. In this case, the destination file (the one which would be obtained without splitting) will not be stored.\n"
		"  \n"
		"Time ranges are specified as follows:\n"
		"- `S-E`: `S` start and `E` end times, formatted as `HH:MM:SS.ms`, `MM:SS.ms` or time in seconds (int, double, fraction)\n"
		"- `S:E`: `S` start time and `E` end times in seconds (int, double, fraction)\n"
		"- `S:end` or `S:end-N`: `S` start time in seconds (int, double), `N` number of seconds (int, double) before the end\n"
		"  \n"
		"MP4Box splitting runs a filter session using the `reframer` filter as follows:\n"
		"- `splitrange` option of the reframer is always set\n"
		"- source is demuxed with `alltk` option set\n"
		"- start and end ranges are passed to `xs` and `xe` options of the reframer\n"
		"- for `-splitz`, options `xadjust` and `xround=after` are enforced\n"
		"- for `-splitg`, options `xadjust` and `xround=before` are enforced\n"
		"- for `-splitf`, option `xround=seek` is enforced and `propbe_ref`set if not specified at prompt\n"
		"- for `-splitx`, option `xround=closest` and `propbe_ref` are enforced if not specified at prompt\n"
		"  \n"
		"The output file(s) storage mode can be specified using -flat, -newfs, -inter and -frag\n"
		"  \n"
	);

	i=0;
	while (m4b_split_args[i].name) {
		GF_GPACArg *arg = (GF_GPACArg *) &m4b_split_args[i];
		i++;
		gf_sys_print_arg(helpout, help_flags, arg, "mp4box-split");
	}

}