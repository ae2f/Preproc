#include <stdio.h>
#include <string.h>
#include <assert.h>

#define DIRLEN		150
#define PATHLEN		150

typedef unsigned flag_t;

struct FLAGS_t {
	flag_t	m_Inc : 1;
	/** is a key of include */
	flag_t	m_Key : 1;
	flag_t	m_Key_Idx : 3;
	flag_t	m_path_nclosed : 1;
} FLAGS;

int	Ch;
int	i;

char	inc_key[sizeof("#include")] = {0, };
char	inc_path_buff[PATHLEN + 1] = {0, };
char	inc_path_dir_buff[PATHLEN + DIRLEN + 1] = {0, };

char	path_closing;
char*	path_cursor;

FILE*	file;

#define ISKEY() \
	for( \
			FLAGS.m_Key_Idx = sizeof("include") - 1; \
			FLAGS.m_Key_Idx-- && inc_key[FLAGS.m_Key_Idx + 1] == "include"[FLAGS.m_Key_Idx]; \
			) {\
	} FLAGS.m_Key = FLAGS.m_Key_Idx == 7;

int main(int argc, char** argv) {
	Ch = 0;
	inc_key[0] = '#';

	for(i = argc; i-- != 1;) {
		if(strlen(argv[i]) > DIRLEN)
			return -1;
	}


KEYGET:
	while((Ch = fgetc(stdin)) != EOF) {
		if(Ch == '#') {
			fgets(inc_key + 1, sizeof(inc_key) - 1, stdin);
			fputs("/* ", stdout);
			fputs(inc_key + 1, stdout);
			fputs(" */\n", stdout);
			ISKEY();

			if(FLAGS.m_Key) {
				goto PATH;
			} else {
				fputs(inc_key, stdout);
			}
		} else {
			fputc(Ch, stdout);
		}
	}

	return FLAGS.m_Inc;

PATH:
	puts("/** include detected */");
	FLAGS.m_Inc = 1;

	while((Ch = fgetc(stdin)) != EOF && Ch == ' ' || Ch == '\t') {}
	puts("/** blank has done */");
	switch(Ch) {
		default: 
			puts("/**Error: eof*/");
			return -1;
		case '\"': path_closing = '\"';
		case '<': path_closing = '>';
	}

	path_cursor = inc_path_buff;
	FLAGS.m_path_nclosed = 1;

	puts("/** Path cursor */");
	while(path_cursor - inc_path_buff < PATHLEN) {
		if((Ch = fgetc(stdin)) == EOF) return -1;
		if(Ch == path_closing) {
			*path_cursor = 0; 
			FLAGS.m_path_nclosed = 0; 
			break; 
		}
		*path_cursor = Ch;
		path_cursor++;
	}
	if(FLAGS.m_path_nclosed) return -1;

	fputs("/* Opening: ", stdout);
	fputs(inc_path_buff, stdout);
	fputs("*/\n", stdout);


	file = fopen(inc_path_buff, "r");
	for(i = argc; !file && i-- != 1;) {
		strcpy(inc_path_dir_buff, argv[i]);
		strcat(inc_path_dir_buff, inc_path_buff);
		fputs("/* Opening: ", stdout);
		fputs(inc_path_dir_buff, stdout);
		fputs("*/\n", stdout);
		file = fopen(inc_path_dir_buff, "r");
	}
	if(!file) return -1;

	while((Ch = fgetc(file)) != EOF) fputc(Ch, stdout);
	puts("");
	fclose(file);

	goto KEYGET;
	assert(0);
}
