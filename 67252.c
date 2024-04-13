static void free_StringFileInfo(StringFileInfo* stringFileInfo) {
	if (stringFileInfo) {
		free (stringFileInfo->szKey);
		if (stringFileInfo->Children) {
			ut32 childrenSFI = 0;
			for (; childrenSFI < stringFileInfo->numOfChildren; childrenSFI++) {
				free_StringTable (stringFileInfo->Children[childrenSFI]);
			}
			free (stringFileInfo->Children);
		}
		free (stringFileInfo);
	}
}
