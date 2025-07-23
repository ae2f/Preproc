#include "./main.auto.h"

#if defined(NDEBUG) && NDEBUG
#define DEBUG	0
#else
#define DEBUG	1
#endif

#include <stdio.h>
#include <string.h>
#include <assert.h>

enum STATE_t {
	STATE_GOOD,
	STATE_EOF,
	STATE_OPENING_UNKNOWN,
	STATE_STACK_SMASHED,
	STATE_ARGC_UNEXPECTED,
	STATE_FOPEN_BAD
};

struct STACK_T {
	FILE*	m_inp;
	char	m_dir[DIRLEN + 1];
	char	m_current[DIRLEN + PATHLEN + 1];
} STACK[STACKLEN + 1];
size_t STACK_IDX = 0;
size_t STACK_IDX_BUFF;

#define STACK_LAST	STACK[STACK_IDX]
#define STACK_NXT	STACK[STACK_IDX + 1]

int	Ch;
int	i;

char	inc_key[sizeof("#include")] = {0, };
char	inc_path_buff[PATHLEN + 1] = {0, };

typedef unsigned char flag_t;
struct FLAGS_t {
	/** is a key of include */
	flag_t	m_Key : 1;
	flag_t	m_path_nclosed : 1;
	flag_t	m_Key_Idx : 3;
	flag_t	m_ManyCmtOutMaybe : 1;
} FLAGS;

char	path_closing;
char*	path_cursor;

#define ISKEY() \
	for( \
			FLAGS.m_Key_Idx = sizeof("include") - 1; \
			FLAGS.m_Key_Idx-- && inc_key[FLAGS.m_Key_Idx + 1] == "include"[FLAGS.m_Key_Idx]; \
			) {\
	} FLAGS.m_Key = FLAGS.m_Key_Idx == 7;

int main(int argc, char** argv) {
	assert(argc > 0);
	if(argc < 1) {
		return -STATE_ARGC_UNEXPECTED;
	}

	Ch = 0;
	inc_key[0] = '#';
	STACK_LAST.m_dir[0] = 0;
	STACK_LAST.m_inp = stdin;

	for(i = argc; i-- != 1;) {
		if(strlen(argv[i]) > DIRLEN)
			return -1;
	}

	assert(STACK_LAST.m_inp);

KEYGET:
	while((Ch = fgetc(STACK_LAST.m_inp)) != EOF) {
		switch(Ch) {
			case '#':
				while((Ch = fgetc(STACK_LAST.m_inp)) != EOF && Ch == ' ' || Ch == '\t');
				if(Ch == EOF) return -STATE_EOF;
				inc_key[1] = Ch;
				fgets(inc_key + 2, sizeof(inc_key) - 2, STACK_LAST.m_inp);

				ISKEY();

				if(FLAGS.m_Key) {
					goto STACK_SUB;
				} else {
					fputs(inc_key, stdout);
				}
				break;

			case '/':
				switch(fgetc(STACK_LAST.m_inp)) {
					case '/': 
#if DEBUG
						fputs("//", STACK_LAST.m_inp);
#endif

						while((Ch = fgetc(STACK_LAST.m_inp)) != EOF && Ch != '\n' && Ch != '\r') {
#if DEBUG
							fputc(Ch, stdout);
#endif
						}

						if(Ch == EOF) return -STATE_EOF;
						puts("");
						goto KEYGET;

					case '*':
#if DEBUG
						fputs("/*", stdout);
#endif

						while((Ch = fgetc(STACK_LAST.m_inp)) != EOF) {
							if(Ch == '*') {
								if((Ch = fgetc(STACK_LAST.m_inp)) == '/') {
#if DEBUG
									fputs("*/", stdout);
#endif
									goto KEYGET;
								}

#if DEBUG
								fputc('*', stdout);
								fputc(Ch, stdout);
#endif
							}

#if DEBUG
							fputc(Ch, stdout);
#endif
						}

						goto KEYGET;

					default:
						fputc(Ch, stdout);
						goto KEYGET;
				}

			default:
				fputc(Ch, stdout);
		}
	}

	goto STACK_ADD;
STACK_SUB:
	while((Ch = fgetc(STACK_LAST.m_inp)) != EOF && Ch == ' ' || Ch == '\t');
	switch(Ch) {
		case EOF: 
#if DEBUG
			puts("/** Unknown EOF bug has appeared. */");
#endif
			return -STATE_EOF;
		case '\"': path_closing = '\"';	break;
		case '<': path_closing = '>';	break;
		default:
			  fputc(Ch, stdout);
			  goto KEYGET;
	}

#if DEBUG
	puts("/** include detected */");
#endif

	path_cursor = inc_path_buff;
	FLAGS.m_path_nclosed = 1;
#if DEBUG
	puts("/** Path cursor */");
#endif
	if(STACK_IDX == STACKLEN) {
#if DEBUG
		puts("/** Stack has smashed. shame. */");
		fputs("/*", stdout);

		while((Ch = fgetc(STACK_LAST.m_inp)) != EOF && Ch != '\n' && Ch != '\r') {
#if DEBUG
			fputc(Ch, stdout);
#endif
		}

		puts("*/");
#endif

#if INC_IGNORE_SMASH



		goto KEYGET;
#endif
		return -STATE_STACK_SMASHED;
	}

	while(path_cursor - inc_path_buff < PATHLEN) {
		if((Ch = fgetc(STACK_LAST.m_inp)) == EOF) 
			return -STATE_EOF;
		if(Ch == path_closing) {
			*path_cursor = 0; 
			FLAGS.m_path_nclosed = 0; 
			break; 
		}
		*path_cursor = Ch;
		path_cursor++;
	}
	if(FLAGS.m_path_nclosed) return -STATE_OPENING_UNKNOWN;

	strcpy(STACK_NXT.m_current, STACK_LAST.m_dir);
	strcat(STACK_NXT.m_current, inc_path_buff);

#if DEBUG
	fputs("/* Opening: ", stdout);
	fputs(STACK_NXT.m_current, stdout);
	fputs(" */\n", stdout);
#endif
	STACK_NXT.m_inp = fopen(STACK_NXT.m_current, "r");

#if DEBUG
	printf("/** first fp: %p */\n", STACK_NXT.m_inp);
#endif


	for(i = argc; !STACK_NXT.m_inp && i-- != 1;) {
		strcpy( STACK_NXT.m_current, argv[i]);
		strcat(STACK_NXT.m_current, inc_path_buff);

#if DEBUG
		fputs("/* Opening: ", stdout);
		fputs(STACK_NXT.m_current, stdout);
		fputs(" */\n", stdout);
#endif
		STACK_NXT.m_inp = fopen(STACK_NXT.m_current, "r");
	}
	if(!STACK_NXT.m_inp)
#if INC_IGNORE_NFOUND
	{
		puts("/** Failed but ignoring it. */");
		goto KEYGET;
	}
#else
	{ return -STATE_FOPEN_BAD; }
#endif

	for(i = strlen(STACK_NXT.m_current); i--;) {
		if(STACK_NXT.m_current[i] == '/') {
			Ch = STACK_NXT.m_current[i + 1];
			STACK_NXT.m_current[i + 1] = 0;
			strcpy(STACK_NXT.m_dir, STACK_NXT.m_current);
			STACK_NXT.m_current[i + 1] = Ch;

#if DEBUG
			printf("/** Current %s */\n", STACK_NXT.m_current);
			printf("/** Dir %s */\n", STACK_NXT.m_dir);
#endif
			break;
		}
	}


#if INC_REPT_CHECK
#if DEBUG
	puts("/** Found something. Now let's validate. */");
#endif

	for(STACK_IDX_BUFF = STACK_IDX; STACK_IDX_BUFF--;) {
		if(!strcmp(STACK[STACK_IDX_BUFF].m_current, STACK_NXT.m_current)) {
#if DEBUG
			puts("/** It's repeating himself for some reason. */");
			goto KEYGET;
#endif
		}
	}

#if DEBUG
	puts("/** It's not repeating*/");
#endif
#endif


#if DEBUG
	fputs(
			"/******************** \n"
			" * Now the path will be: \n * "
			, stdout
	     );

	fputs(STACK_NXT.m_current, stdout);

	puts("\n */");
#endif
	++STACK_IDX;
	goto KEYGET;

STACK_ADD:
	fputc('\n', stdout);

	if(STACK_IDX) {
		fclose(STACK_LAST.m_inp);
		--STACK_IDX;

#if  DEBUG
		puts("/** One eof gracefully */");
#endif

		goto KEYGET;
	}

#if DEBUG
	puts("/** All done. */");
#endif

	return -STATE_GOOD;
}
