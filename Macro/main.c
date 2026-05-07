#define	_CRT_SECURE_NO_WARNINGS	1

#include <.main.h>

#ifndef	_MAIN_H	/** This section is for .main.h when it is not grepped by ast or somehting */
#define	ae2f_MAC_KEYWORD	"ae2f_MAC"
#define	CMT_REQUIRED		1
#define	SZPARAM			10
#define	SZTPARAM		10
#endif

#include <stdio.h>
#include <stddef.h>


int main(void);

#define BOOK (ae2f_MAC_KEYWORD "(")
#define	STRLEN(a)	((int)(sizeof(a) - 1))

#if CMT_REQUIRED	/* comment required */
#define dbg_puts(s) fputs(s, stdout)
#else
#define dbg_puts(s)
#endif

#define SZBOOK ((size_t)(sizeof(BOOK) - 1))

char SEE_BUF[sizeof(BOOK)] = {
        0,
};

char PARAMS_BUF[SZPARAM + 2 + (CMT_REQUIRED) * 4] = {
        0,
};
char TPARAMS_BUF[SZTPARAM + 1] = {
        0,
};

char ENDL[] = " \\\n";

#define STATE_OK 0
#define STATE_OUTFAILED 1
#define STATE_UNEXPECTED 2
#define STATE_OVERRUN 3

#define casenewlines	\
        case '\n':	\
	case '\r'

#define caseblanks	\
        case ' ':	\
	case '\t':	\
casenewlines

#define isAlph(c)                                                              \
        ((((c) >= 'A') && ((c) <= 'Z')) || (((c) >= 'a') && ((c) <= 'z')))
#define isNumber(c) (((c) >= '0') && ((c) <= '9'))
#define isVarName(c) ((isAlph(c) || isNumber(c)) || (c) == '_')	/* TODO: first character shouldn't be number*/
#define isNewLine(c) ((c) == '\n' || (c) == '\r')

#define isTrue(u, m) ((u) & (m))
#define setTrue(u, m) ((u) |= (m))
#define setFalse(u, m) ((u) &= ~((unsigned)m))

int main(void) {
	/**  register char prms_lentype = 0; */

	int ch;
	int l;
	int tprms_len = 0;
	int prms_len = 0;
	int stack = 0;

	enum {
		FIELD_CMT = 1,
		FIELD_SPACE_FOUND = 2,
		FIELD_FIRST = 4,
		FIELD_IS_FN_ARG = 8,
		FIELD_IS_BYPASSING_ARG = 16
	};

	unsigned field = 0;

	puts("#undef\t__ae2f_MACRO_GENERATED\n"
			"#define\t__ae2f_MACRO_GENERATED\t1");
	while ((ch = fgetc(stdin)) != EOF) {

		goto __START;
__REST:
		if (ch != (BOOK)[0]) {
			l = fputc(ch, stdout);
			if (l < 0)
				return STATE_OUTFAILED;
		}
		continue;

__START:
		if (ch == '\"') goto STRING;

		if (ch == '#') {
			const char INCLUDE[sizeof("#include")] = "#include";
			char SEE_INCLUDE[STRLEN(INCLUDE)];
			size_t i = 1;
			SEE_INCLUDE[0] = '#';

			l = fputc(SEE_INCLUDE[0], stdout);
			if (l < 0)	return STATE_OUTFAILED;

			for (; i < (size_t)STRLEN(INCLUDE); ++i) {
				SEE_INCLUDE[i] = (char)fgetc(stdin);
				l = fputc(SEE_INCLUDE[i], stdout);
				if (l < 0)	return STATE_OUTFAILED;

				if (SEE_INCLUDE[i] != INCLUDE[i]) {
					break;
				}
			}

			if (i == STRLEN(INCLUDE)) {
				while ((ch = fgetc(stdin)) != EOF && !isNewLine(ch)) {
					fputc(ch, stdout);
				}

				l = puts("\n#undef __ae2f_MACRO_GENERATED\n"
					"#define __ae2f_MACRO_GENERATED 1");
				if (l < 0)	return STATE_OUTFAILED;
				
			}
		} else if (ch == (BOOK)[0]) {
			size_t i = 1;
			SEE_BUF[0] = (BOOK)[0];
			for (; i < SZBOOK; i++) {
				SEE_BUF[i] = (char)fgetc(stdin);

				if (SEE_BUF[i] != (BOOK)[i]) {
					SEE_BUF[i + 1] = 0;
					break;
				}
			}

			if (i < SZBOOK) {
				l = fputs(SEE_BUF, stdout);
				if (l < 0)	return STATE_OUTFAILED;

				continue;
			}

			l = fputs("#define _", stdout);
			if (l < 0)	return STATE_OUTFAILED;

			tprms_len = 0;
			prms_len = 0;
			stack = 0; /** init */
			/** prms_lentype = 1; */

			while (1) {
				switch ((ch = fgetc(stdin))) {
					case EOF:
						return STATE_UNEXPECTED;

					case ')':
						if(!(stack--))	goto TPARAMS_END;
						break;

					case '(':
						stack++;
						break;

					default: {
						TPARAMS_BUF[tprms_len] = (char)ch;
						if (tprms_len == SZTPARAM)	return STATE_OVERRUN;
						tprms_len++;
					}
					goto FALLTROUGH_0;
caseblanks:
FALLTROUGH_0:
						break;
				}
			} /** tparam */

TPARAMS_END:
			while (1) {
				switch (ch = fgetc(stdin)) {
					case EOF:
						return STATE_OUTFAILED;
					default:
						if (ch == '{') {
							setTrue(field, FIELD_IS_BYPASSING_ARG);
							ch = '(';
						}

						l = fputc(ch, stdout);
						if (l < 0)	return STATE_OUTFAILED;

						if (ch == '(')	goto FNNAME_END;

						goto FALLTROUGH_1;	
caseblanks:
FALLTROUGH_1:
						break;
				}
			} /** fn */

FNNAME_END:
			dbg_puts(" \\\n\t/** tparam */ \\\n\t\t");
			TPARAMS_BUF[tprms_len] = 0;

			l = fputs(TPARAMS_BUF, stdout);
			if (l < 0)	return STATE_OUTFAILED;

			dbg_puts(" \\\n \\\n\t/** param */ \\\n\t\t");
			prms_len = 0;
			setTrue(field, FIELD_CMT | FIELD_FIRST);
			stack = 0;
			setFalse(field, FIELD_IS_FN_ARG);

			if(isTrue(field, FIELD_IS_BYPASSING_ARG)) {
				ch = ')';
				fputs("/*", stdout);
				goto LBL_BYPASS_SCOPE;
			}

			while (1) {

				if (isTrue(field, FIELD_CMT)) {
					if (fputs("/* ", stdout) < 0)
						return STATE_OUTFAILED;
					setFalse(field, FIELD_CMT);
				}

				switch (ch = fgetc(stdin)) {
					case EOF:
						return 1;

caseblanks:
						fputc(' ', stdout);
PRM_CASE_BLANK:
						setTrue(field, FIELD_SPACE_FOUND);
						break;

					default:
						if (!isVarName(ch)) {
							PARAMS_BUF[prms_len] = 0;
							fputs(PARAMS_BUF, stdout);
							fputc(ch, stdout);
							prms_len = 0;
							goto PRM_CASE_BLANK;
						}

						if (isTrue(field, FIELD_FIRST)) {
							setFalse(field, FIELD_FIRST);
							if (fputc(',', stdout) < 0)
								return STATE_OUTFAILED;
						}

						if (isTrue(field, FIELD_SPACE_FOUND)) {
							PARAMS_BUF[prms_len] = 0;
							fputs(PARAMS_BUF, stdout);
							prms_len = 0;
							setFalse(field, FIELD_SPACE_FOUND);
						}

						PARAMS_BUF[prms_len] = (char) ch;
						if (prms_len == SZPARAM)
							return STATE_OVERRUN;
						prms_len++;
						break;

					case '(':
						if(isTrue(field, FIELD_IS_FN_ARG)) {
							while((fgetc(stdin)) != ')');
						} else {
							stack++;
							setTrue(field, FIELD_IS_FN_ARG);
						}

						break;

					case ',':
						setFalse(field, FIELD_IS_FN_ARG);
						goto LBL_BYPASS_SCOPE;
					case ')':
LBL_BYPASS_SCOPE:
						if(ch == ')' && stack--) {
							break;
						}

						PARAMS_BUF[prms_len] = (char) ch;
						PARAMS_BUF[prms_len + 1] = 0;

						if (fputs(" */ ", stdout) < 0)
							return STATE_OUTFAILED;

						setTrue(field,FIELD_CMT);

#if CMT_REQUIRED
						if (ch == ')') {
							PARAMS_BUF[prms_len] = ' ';
							PARAMS_BUF[prms_len + 1] = '\\';
							PARAMS_BUF[prms_len + 2] = '\n';
							PARAMS_BUF[prms_len + 3] = (char)ch;
							PARAMS_BUF[prms_len + 4] = 0;
						} 
#endif
						l = fputs(PARAMS_BUF, stdout);
						if (l < 0)
							return STATE_OUTFAILED;

						prms_len = 0;

						if (',' == ch) {
							dbg_puts(" \\\n\t\t");
							goto PRM_CASE_BLANK;
						} else {
							goto PRM_END;
						}
				}
			}
PRM_END:

			stack = 0;
			if(isTrue(field, FIELD_IS_BYPASSING_ARG)) {
				setFalse(field, FIELD_IS_BYPASSING_ARG);
				ch = '{';
				goto LBL_UP_STACK;
			}

			while (1) {
				switch (ch = fgetc(stdin)) {
					case EOF:
						return 1;

casenewlines:
						ENDL[2] = (char) ch;
						l = fputs(ENDL, stdout);
						if (l < 0)
							return STATE_OUTFAILED;

						break;

					case '$':
						ch = '#';
						goto LBL_CASENEWLINE_DEFAULT;
					default:
LBL_CASENEWLINE_DEFAULT:

						l = fputc(ch, stdout);
						if (l < 0)
							return STATE_OUTFAILED;
						break;

					case '{':
LBL_UP_STACK:
						if (!(++stack)) {
							return STATE_OVERRUN;
						}
						l = fputc('{', stdout);
						if (l < 0)
							return STATE_OUTFAILED;
						break;

					case '}':
						l = fputc('}', stdout);
						if (l < 0)
							return STATE_OUTFAILED;

						if (!(--stack)) {
							goto STACKED;
						}
						break;
				}
			}
			/* stack */
STACKED:;
		} else {
			goto __REST;
		}
	}

	puts("\n#undef\t__ae2f_MACRO_GENERATED\n"
		"\n#define\t__ae2f_MACRO_GENERATED\t0\n");

	return 0;

STRING:
	fputc('\"', stdout);
	while((ch = fgetc(stdin)) != EOF) {
		switch(ch) {
			case '\"':
				goto __REST;

			case '\\':
				fputc(ch, stdout);
				if((ch = fgetc(stdin)) == EOF) break;
				fputc(ch, stdout);
				break;
			default:
				fputc(ch, stdout);
				break;
		}
	}

	return -1;
}
