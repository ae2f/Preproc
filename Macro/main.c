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

#if CMT_REQUIRED
#define dbg_puts(s) fputs(s, stdout)
#else
#define dbg_puts(s)
#endif

#define SZBOOK ((size_t)(sizeof(BOOK) - 1))

char SEE[sizeof(BOOK)] = {
        0,
};

char PARAM[SZPARAM + 2 + (CMT_REQUIRED) * 4] = {
        0,
};
char TPARAMS[SZTPARAM + 1] = {
        0,
};

char ENDL[] = " \\\n";

#define STATE_OK 0
#define STATE_OUTFAILED 1
#define STATE_UNEXPECTED 2
#define STATE_OVERRUN 3

#define casenewlines                                                           \
        case '\n':                                                                   \
case '\r'

#define caseblanks                                                             \
        case ' ':                                                                    \
case '\t':                                                                   \
casenewlines

#define isAlph(c)                                                              \
        ((((c) >= 'A') && ((c) <= 'Z')) || (((c) >= 'a') && ((c) <= 'z')))
#define isNumber(c) (((c) >= '0') && ((c) <= '9'))
#define isVarName(c) ((isAlph(c) || isNumber(c)) || (c) == '_')
#define isNewLine(c) ((c) == '\n' || (c) == '\r')

#define isTrue(u, m) ((u) & (m))
#define setTrue(u, m) ((u) | (m))

#define setFalse(u, m) ((u) & ~((unsigned)m))
int main(void) {
	/**  register char prmtype = 0; */

	int c;
	int l;
	int tprm = 0;
	int prm = 0;
	int stack = 0;

	enum {
		PRM_CMT = 1,
		PRM_SPACE_FOUND = 2,
		PRM_FIRST = 4,
		PRM_IS_FN_ARG = 8,
		PRM_IS_BYPASSING_ARG = 16
	};

	unsigned prm_bool = 0;


	puts("#undef\t__ae2f_MACRO_GENERATED\n"
			"#define\t__ae2f_MACRO_GENERATED\t1");
	while ((c = fgetc(stdin)) != EOF) {

		goto __START;
__REST:
		if (c != (BOOK)[0]) {
			l = fputc(c, stdout);
			if (l < 0)
				return 1;
		}
		continue;

__START:
		if (c == '\"') goto STRING;

		if (c == '#') {
			const char INCLUDE[sizeof("#include")] = "#include";
			char SEE_INC[STRLEN(INCLUDE)];
			size_t i = 1;
			SEE_INC[0] = '#';
			fputc('#', stdout);
			for (; i < (size_t)STRLEN(INCLUDE); i++) {
				SEE_INC[i] = (char)fgetc(stdin);
				fputc(SEE_INC[i], stdout);

				if (SEE_INC[i] != INCLUDE[i]) {
					break;
				}
			}

			if (i == STRLEN(INCLUDE)) {
				while ((c = fgetc(stdin)) != EOF && !isNewLine(c)) {
					fputc(c, stdout);
				}

				puts("\n#undef __ae2f_MACRO_GENERATED\n"
						"#define __ae2f_MACRO_GENERATED 1");
			}
		}

		else if (c == (BOOK)[0]) {
			size_t i = 1;
			SEE[0] = (BOOK)[0];
			for (; i < SZBOOK; i++) {
				SEE[i] = (char)fgetc(stdin);

				if (SEE[i] != (BOOK)[i]) {
					SEE[i + 1] = 0;
					break;
				}
			}

			if (i < SZBOOK) {
				if (fputs(SEE, stdout) < 0)
					return 1;

				continue;
			}

			l = fputs("#define _", stdout);

			if (l < 0)
				return STATE_OUTFAILED;

			tprm = 0;
			prm = 0;
			stack = 0; /** init */
			/** prmtype = 1; */

			while (1) {
				switch ((c = fgetc(stdin))) {
					case EOF:
						return STATE_UNEXPECTED;

					case ')':
						if(!(stack--))
							goto TPARAMED;
						break;

					case '(':
						stack++;
						break;


					default: {
							 TPARAMS[tprm] = (char)c;
							 if (tprm == SZTPARAM)
								 return STATE_OVERRUN;
							 tprm++;
						 }

						 goto CBLANK_0;

CBLANK_0:
caseblanks:
					break;
				}
			} /** tparam */

TPARAMED:
			while (1) {
				switch (c = fgetc(stdin)) {
					case EOF:
						return 1;
					default:
						if (c == '{') {
							prm_bool = setTrue(prm_bool, PRM_IS_BYPASSING_ARG);
							c = '(';
						}

						l = fputc(c, stdout);
						if (l < 0)
							return STATE_OUTFAILED;

						if (c == '(')
							goto FNED;
						goto CBLANK_1;
CBLANK_1:
caseblanks:
						break;
				}
			} /** fn */

FNED:

			dbg_puts(" \\\n\t/** tparam */ \\\n\t\t");
			TPARAMS[tprm] = 0;
			if (fputs(TPARAMS, stdout) < 0)
				return STATE_OUTFAILED;

			dbg_puts(" \\\n \\\n\t/** param */ \\\n\t\t");
			prm = 0;
			prm_bool |= PRM_CMT | PRM_FIRST;
			stack = 0;
			prm_bool = setFalse(prm_bool, PRM_IS_FN_ARG);

			if(isTrue(prm_bool, PRM_IS_BYPASSING_ARG)) {
				c = ')';
				fputs("/*", stdout);
				goto LBL_BYPASS_SCOPE;
			}

			while (1) {

				if (isTrue(prm_bool, PRM_CMT)) {
					if (fputs("/* ", stdout) < 0)
						return STATE_OUTFAILED;
					prm_bool = setFalse(prm_bool, PRM_CMT);
				}

				switch (c = fgetc(stdin)) {
					case EOF:
						return 1;

caseblanks:
						fputc(' ', stdout);
PRM_CASE_BLANK:
						prm_bool = setTrue(prm_bool, PRM_SPACE_FOUND);
						break;

					default:
						if (!isVarName(c)) {
							PARAM[prm] = 0;
							fputs(PARAM, stdout);
							fputc(c, stdout);
							prm = 0;
							goto PRM_CASE_BLANK;
						}

						if (isTrue(prm_bool, PRM_FIRST)) {
							prm_bool = setFalse(prm_bool, PRM_FIRST);
							if (fputc(',', stdout) < 0)
								return STATE_OUTFAILED;
						}

						if (isTrue(prm_bool, PRM_SPACE_FOUND)) {
							PARAM[prm] = 0;
							fputs(PARAM, stdout);
							prm = 0;
							prm_bool = setFalse(prm_bool, PRM_SPACE_FOUND);
						}

						PARAM[prm] = (char) c;
						if (prm == SZPARAM)
							return STATE_OVERRUN;
						prm++;
						break;

					case '(':
						if(isTrue(prm_bool, PRM_IS_FN_ARG)) {
							while((fgetc(stdin)) != ')');
						} else {
							stack++;
							prm_bool = setTrue(prm_bool, PRM_IS_FN_ARG);
						}

						break;

					case ',':
						prm_bool = setFalse(prm_bool, PRM_IS_FN_ARG);
						goto LBL_BYPASS_SCOPE;
					case ')':
LBL_BYPASS_SCOPE:
						if(c == ')' && stack--) {
							break;
						}

						PARAM[prm] = (char) c;
						PARAM[prm + 1] = 0;

						if (fputs(" */ ", stdout) < 0)
							return STATE_OUTFAILED;

						prm_bool = setTrue(prm_bool,PRM_CMT);

#if CMT_REQUIRED
						if (c == ')') {
							PARAM[prm] = ' ';
							PARAM[prm + 1] = '\\';
							PARAM[prm + 2] = '\n';
							PARAM[prm + 3] = (char)c;
							PARAM[prm + 4] = 0;
							l = fputs(PARAM, stdout);
						} else
#endif
							l = fputs(PARAM, stdout);

						if (l < 0)
							return STATE_OUTFAILED;

						prm = 0;

						if (',' == c) {
							dbg_puts(" \\\n\t\t");
							goto PRM_CASE_BLANK;
						} else {
							goto PRMED;
						}
				}
			}
PRMED:

			stack = 0;
			if(isTrue(prm_bool, PRM_IS_BYPASSING_ARG)) {
				prm_bool = setFalse(prm_bool, PRM_IS_BYPASSING_ARG);
				c = '{';
				goto LBL_UP_STACK;
			}

			while (1) {
				switch (c = fgetc(stdin)) {
					case EOF:
						return 1;

casenewlines:
						ENDL[2] = (char) c;
						l = fputs(ENDL, stdout);
						if (l < 0)
							return STATE_OUTFAILED;

						break;

					case '$':
						c = '#';
						goto LBL_CASENEWLINE_DEFAULT;
					default:
LBL_CASENEWLINE_DEFAULT:

						l = fputc(c, stdout);
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
		}

		else {
			goto __REST;
		}
	}

	puts(
			"\n#undef\t__ae2f_MACRO_GENERATED\n"
			"\n#define\t__ae2f_MACRO_GENERATED\t0\n"
			);

	return 0;

STRING:
	fputc('\"', stdout);
	while((c = fgetc(stdin)) != EOF) {
		switch(c) {
			case '\"':
				goto __REST;

			case '\\':
				fputc(c, stdout);
				if((c = fgetc(stdin)) == EOF) break;
				fputc(c, stdout);
				break;
			default:
				fputc(c, stdout);
				break;
		}
	}

	return -1;
}
