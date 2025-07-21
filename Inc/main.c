#define ON		1
#define OFF		0

#define DIRLEN			150
#define PATHLEN			150
#define STACKLEN		10
#define	INC_IGNORE_NFOUND	ON

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
	char	m_dir[DIRLEN];
} STACK[STACKLEN + 1];
size_t STACK_IDX = 0;

#define STACK_LAST	STACK[STACK_IDX]
#define STACK_NXT	STACK[STACK_IDX + 1]

int	Ch;
int	i;

char	inc_key[sizeof("#include")] = {0, };
char	inc_path_buff[PATHLEN + 1] = {0, };
char	inc_path_dir_buff[PATHLEN + DIRLEN + 1] = {0, };

typedef unsigned char flag_t;
struct FLAGS_t {
	/** is a key of include */
	flag_t	m_Key : 1;
	flag_t	m_path_nclosed : 1;
	flag_t	m_Key_Idx : 3;
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
#if DEBUG
	puts("/** It starts over. */");
#endif

	while((Ch = fgetc(STACK_LAST.m_inp)) != EOF) {
		if(Ch == '#') {
			fgets(inc_key + 1, sizeof(inc_key) - 1, STACK_LAST.m_inp);

#if DEBUG
			fputs("/* ", stdout);
			fputs(inc_key + 1, stdout);
			fputs(" */\n", stdout);
#endif
			ISKEY();

			if(FLAGS.m_Key) {
				goto STACK_SUB;
			} else {
				fputs(inc_key, stdout);
			}
		} else {
			fputc(Ch, stdout);
		}
	}

	goto STACK_ADD;
STACK_SUB:
#if DEBUG
	puts("/** include detected */");
#endif
	while((Ch = fgetc(STACK_LAST.m_inp)) != EOF && Ch == ' ' || Ch == '\t');
#if DEBUG
	puts("/** blank has done */");
#endif
	switch(Ch) {
		default: 
			return -STATE_OPENING_UNKNOWN;
		case '\"': path_closing = '\"';	break;
		case '<': path_closing = '>';	break;
	}

	path_cursor = inc_path_buff;
	FLAGS.m_path_nclosed = 1;
#if DEBUG
	puts("/** Path cursor */");
#endif
	if(STACK_IDX == STACKLEN) {
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
	if(FLAGS.m_path_nclosed) return -1;

	strcpy(inc_path_dir_buff, STACK_LAST.m_dir);
	strcat(inc_path_dir_buff, inc_path_buff);

#if DEBUG
	fputs("/* Opening: ", stdout);
	fputs(inc_path_dir_buff, stdout);
	fputs(" */\n", stdout);
#endif
	STACK_NXT.m_inp = fopen(inc_path_dir_buff, "r");

#if DEBUG
	printf("/** first fp: %p */\n", STACK_NXT.m_inp);
#endif


	for(i = argc; !STACK_NXT.m_inp && i-- != 1;) {
		strcpy(inc_path_dir_buff, argv[i]);
		strcat(inc_path_dir_buff, inc_path_buff);

#if DEBUG
		fputs("/* Opening: ", stdout);
		fputs(inc_path_dir_buff, stdout);
		fputs(" */\n", stdout);
#endif
		STACK_NXT.m_inp = fopen(inc_path_dir_buff, "r");
	}
	if(!STACK_NXT.m_inp)
#if INC_IGNORE_NFOUND
	{
		puts("/** Failed but ignoring it. */");
		goto KEYGET;
	}
#else
		return -STATE_FOPEN_BAD
#endif

	for(i = strlen(inc_path_dir_buff); i--;) {
		if(inc_path_dir_buff[i] == '/') {
			inc_path_dir_buff[i + 1] = 0;
			break;
		}
	}

#if DEBUG
		fputs(
				"/******************** \n"
				" * Now the path will be: \n * "
				, stdout
				);

		fputs(inc_path_dir_buff, stdout);

		puts("\n */");
#endif

	strcpy(STACK_NXT.m_dir, inc_path_dir_buff);
	++STACK_IDX;
	goto KEYGET;

STACK_ADD:
#if  DEBUG
	puts("/** One eof gracefully */");
#endif
	if(STACK_IDX) {
		fclose(STACK_LAST.m_inp);
		--STACK_IDX;
		goto KEYGET;
	}

	return -STATE_GOOD;
}
