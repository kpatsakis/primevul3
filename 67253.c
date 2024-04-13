static void free_StringTable(StringTable* stringTable) {
	if (stringTable) {
		free (stringTable->szKey);
		if (stringTable->Children) {
			ut32 childrenST = 0;
			for (; childrenST < stringTable->numOfChildren; childrenST++) {
				free_String (stringTable->Children[childrenST]);
			}
			free (stringTable->Children);
		}
		free (stringTable);
	}
}
