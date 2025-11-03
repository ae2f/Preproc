#include "./main.auto.h"
#include <stdio.h>

#if STRGEN

char BackSlash[3] = "\\\\";

int main() {
        register int Ch;

	fputc('"', stdout);
	while((Ch =  fgetc(stdin)) != EOF) {
		switch(Ch) {
                        case '\0': case '\n': case '\r': /** Line Changing */
				fputs("\\n\"\\\n\"", stdout);
                                
				break;
			case '\\': case '\'': case '\"':
				BackSlash[1] = Ch;
				fputs(BackSlash, stdout);
				break;
			case '\t':
				BackSlash[1] = 't';
				fputs(BackSlash, stdout);
				break;
			default:
				fputc(Ch, stdout);
				break;
		}
	}

	fputc('"', stdout);
	return 0;
}
#else

char	Buff[BINGEN_BUFFSZ];
size_t	BuffLen, BuffI;

int main() {
	BuffLen = fread(Buff, 1, 1, stdin);
	if(ferror(stdout)) {
		return -1;
	}
	if(!BuffLen || feof(stdin)) {
		printf("{}");
		return 0;
	}

	printf("{%d", Buff[0]);

RECURSE:

	BuffLen = fread(Buff, 1, BINGEN_BUFFSZ, stdin);
	if(ferror(stdin)) {
		return -1;
	}

	if(BuffLen) {
		for(BuffI = 0; BuffI < BuffLen; BuffI++) {
			printf(",%d", Buff[BuffI]);
		}

		goto RECURSE;
	}

	putc('}', stdout);
	return 0;
}

#endif
