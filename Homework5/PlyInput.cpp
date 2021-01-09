#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Geometry.h"

char* get_words(FILE *fp, int &nwords, char *word[])
{
#define LINE_SIZE	4096
	static char line[LINE_SIZE];
	char *ret = fgets(line, LINE_SIZE, fp);
	nwords = 0;

	if (ret != NULL) {
		char *pch = line;
		while (*pch == ' ')
			pch++;

		char token = *pch;
		while (token != '\n') {
			word[nwords] = pch;

			while (*pch != ' ' && *pch != '\n')
				pch++;
			token = *pch;
			*pch++ = '\0';
			nwords++;
		}
	}

	return ret;
}

bool LoadSimplePlyFile(const char *fileName, 
					   FPoint3 *&vertices, 
					   int &nVertices, 
					   int *&indices, 
					   int &nIndices)
{
	FILE *fp;
	if ( (fp = fopen(fileName, "r")) == NULL )
		return false;

	int nWords = 0;
	char *word[16];

	// read header
	get_words(fp, nWords, word);
	if ( strcmp(word[0], "ply") != 0 ) {
		fclose(fp);
		return false;
	}

	nVertices = 0;
	int nFaces = 0;
	bool flagRead = true;
	while (true) {
		if (flagRead)
			get_words(fp, nWords, word);
		else
			flagRead = true;

		if ( strcmp(word[0], "end_header") == 0 )
			break;

		if ( strcmp(word[0], "element") == 0 ) {
			if ( strcmp(word[1], "vertex") == 0 ) {
				nVertices = atoi(word[2]);
				// count property
				int nProps = 0;
				while (true) {
					get_words(fp, nWords, word);

					if ( strcmp(word[0], "property") != 0 ) {
						flagRead = false;
						break;
					}
					nProps++;
				}
				if (nProps != 3) {
					fclose(fp);
					return false;
				}
			} else if ( strcmp(word[1], "face") == 0 )
				nFaces = atoi(word[2]);
		}
	}

	// read the vertices
	vertices = new FPoint3[nVertices];
	FPoint3 vertex;
	for (int i = 0; i < nVertices; i++) {
		get_words(fp, nWords, word);

		vertex.x = atof(word[0]);
		vertex.y = atof(word[1]);
		vertex.z = atof(word[2]);
		vertices[i] = vertex;
	}

	// read the vertex indices
	indices = new int[nFaces * 3];
	int count = 0;
	for ( i = 0; i < nFaces; i++) {
		get_words(fp, nWords, word);

		if ( strcmp(word[0], "3") != 0 ) {
			fclose(fp);
			return false;
		}
		for (int j = 1; j <= 3; j++) {
			int ind = atoi(word[j]);
			indices[count++] = ind;
		}
	}

	fclose(fp);

	nIndices = nFaces * 3;

	return true;
}