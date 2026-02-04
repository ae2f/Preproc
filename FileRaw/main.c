#include "./.main.h"
#include <stdio.h>

#if STRGEN

char BackSlash[] = "\\\\";

int main(int argc, const char** argv) {
        int Ch;
	unsigned Number = 0;
	unsigned LBL = 0;

OVER:
	if(argc > 1) {
		fputs("#define ", stdout);
		fputs(argv[1], stdout);
		printf("_%u \\\n", LBL);
	} else {
		printf("#define A_%u \\\n", LBL);
	}

	fputc('"', stdout);
	while((Ch =  fgetc(stdin)) != EOF) {
		switch(Ch) {
                        case '\0': case '\n': case '\r': /** Line Changing */
				fputs("\\n\" \\\n\"", stdout);
				Number += sizeof("\\n\" \\\n\"") - 1;
				break;
			case '\\': case '\'': case '\"':
				BackSlash[1] = (char)Ch;
				fputs(BackSlash, stdout);
				Number += 2;
				break;
			case '\t':
				BackSlash[1] = 't';
				fputs(BackSlash, stdout);
				Number += 2;
				break;
			default:
				fputc(Ch, stdout);
				++Number;
				break;
		}

		if(Number >= 500) {
			++LBL;
			Number = 0;
			puts("\"");
			goto OVER;
		}
	}

	puts("\"");

	if(argc > 1) {
		fputs("#define ", stdout);
		fputs(argv[1], stdout);
		puts(" \\");
	} else {
		puts("#define A \\");
	}

	for(Number = 0; Number < LBL + 1; ++Number) {
		if(argc > 1) {
			fputs(argv[1], stdout);
			printf("_%u \\\n", Number);
		} else {
			printf("A_%u \\\n", Number);
		}
	}

	puts("\"\"\n");

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
