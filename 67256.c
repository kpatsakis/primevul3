static void free_VarFileInfo(VarFileInfo* varFileInfo) {
	if (varFileInfo) {
		free (varFileInfo->szKey);
		if (varFileInfo->Children) {
			ut32 children = 0;
			for (; children < varFileInfo->numOfChildren; children++) {
				free_Var (varFileInfo->Children[children]);
			}
			free (varFileInfo->Children);
		}
		free (varFileInfo);
	}
}
