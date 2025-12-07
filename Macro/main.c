#include "./main.auto.h"
#include <stdio.h>
#include <stddef.h>

int main(void);

#define BOOK (ae2f_MAC_KEYWORD "(")

#if CMT_REQUIRED
#define dbg_puts(s) fputs(s, stdout)
#else
#define dbg_puts(s)
#endif

#define SZBOOK ((size_t)(sizeof(BOOK) - 1))

char SEE[sizeof(BOOK)] = {
        0,
};

#define prm_cmt prmbool.m_cmt
#define prm_space_found prmbool.m_space_found
#define prm_first prmbool.m_first

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

int main(void) {
	/**  register char prmtype = 0; */

	register int c;
	int l;
	register int tprm = 0;
	register int prm = 0;
	register int stack = 0;

	register struct __prmbool {
		unsigned m_cmt : 1;
		unsigned m_space_found : 1;
		unsigned m_first : 1;
		unsigned m_is_fn_arg : 1;
	} prmbool = {0, 0, 0, 0 };


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
			const char _INCLUDE[sizeof("#include") - 1] = "#include";
			char _SEE_INC[sizeof(_INCLUDE)];
			size_t i = 1;
			_SEE_INC[0] = '#';
			fputc('#', stdout);
			for (; i < (size_t)sizeof(_INCLUDE); i++) {
				_SEE_INC[i] = (char)fgetc(stdin);
				fputc(_SEE_INC[i], stdout);

				if (_SEE_INC[i] != _INCLUDE[i]) {
					break;
				}
			}

			if (i == sizeof(_INCLUDE)) {
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
							 TPARAMS[tprm] = c;
							 if (tprm == SZTPARAM)
								 return STATE_OVERRUN;
							 tprm++;
						 }

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
						l = fputc(c, stdout);
						if (l < 0)
							return STATE_OUTFAILED;
						if (c == '(')
							goto FNED;
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
			prm_cmt = 1;
			prm_first = 1;
			stack = 0;
			prmbool.m_is_fn_arg = 0;

			while (1) {

				if (prm_cmt) {
					if (fputs("/* ", stdout) < 0)
						return STATE_OUTFAILED;
					prm_cmt = 0;
				}

				switch (c = fgetc(stdin)) {
					case EOF:
						return 1;

caseblanks:
						fputc(' ', stdout);
PRM_CASE_BLANK:
						prm_space_found = 1;
						break;

					default:
						if (!isVarName(c)) {
							PARAM[prm] = 0;
							fputs(PARAM, stdout);
							fputc(c, stdout);
							prm = 0;
							goto PRM_CASE_BLANK;
						}

						if (prm_first) {
							prm_first = 0;
							if (fputc(',', stdout) < 0)
								return STATE_OUTFAILED;
						}

						if (prm_space_found) {
							PARAM[prm] = 0;
							fputs(PARAM, stdout);
							prm = 0;
							prm_space_found = 0;
						}

						PARAM[prm] = c;
						if (prm == SZPARAM)
							return STATE_OVERRUN;
						prm++;
						break;

					case '(':
						if(prmbool.m_is_fn_arg) {
							while((fgetc(stdin)) != ')');
						} else {
							stack++;
							prmbool.m_is_fn_arg = 1;
						}

						break;

					case ',':
						prmbool.m_is_fn_arg = 0;
					case ')':
						if(c == ')' && stack--) {
							break;
						}

						PARAM[prm] = c;
						PARAM[prm + 1] = 0;

						if (fputs(" */ ", stdout) < 0)
							return STATE_OUTFAILED;

						prm_cmt = 1;

#if CMT_REQUIRED
						if (c == ')') {
							PARAM[prm] = ' ';
							PARAM[prm + 1] = '\\';
							PARAM[prm + 2] = '\n';
							PARAM[prm + 3] = c;
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

			while (1) {
				switch (c = fgetc(stdin)) {
					case EOF:
						return 1;

casenewlines:
						ENDL[2] = c;
						l = fputs(ENDL, stdout);
						if (l < 0)
							return STATE_OUTFAILED;

						break;

					default:
						l = fputc(c, stdout);
						if (l < 0)
							return STATE_OUTFAILED;
						break;

					case '{':
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
				c = fgetc(stdin);

			default:
				fputc(c, stdout);
				break;
		}
	}

	return -1;
}
