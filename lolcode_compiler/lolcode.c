/******************************************************************************
	NEW:

   monitor
	clearmonitor
	unmonitor

 ******************************************************************************/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <time.h>
# include <ctype.h>
# include "errorcodes.h"

# ifndef TRUE
# define TRUE (0==0)
# define FALSE (0!=0)
# endif
# define VERSION "LOLCODE V0.3 - Modified by Kyle Frost"
# define MAGICNUMBER "lolc0003"
# define MAGICNUMBERHEX "LoLC0003"

# define CMD_PUT_VAR      1
# define CMD_PUT_NUM      2
# define CMD_PUT_STR      3
# define CMD_ADD          4
# define CMD_SUB          5
# define CMD_MUL          6
# define CMD_DIV          7
# define CMD_DUP          8
# define CMD_SWAP         9
# define CMD_ROT          10
# define CMD_DROP         11
# define CMD_SMALLER_NUM  12
# define CMD_EQUALS_NUM   13
# define CMD_BIGGER_NUM   14
# define CMD_SMALLER_STR  15
# define CMD_EQUALS_STR   16
# define CMD_BIGGER_STR   17
# define CMD_HAI          18
# define CMD_CAN_HAS      19
# define CMD_CAN_HAZ      20	// unused
# define CMD_VISIBLE_TEXT 21
# define CMD_VISIBLE_VAR  22
# define CMD_KTHXBYE      23
# define CMD_I_HAS_A      24
# define CMD_GOTO         25
# define CMD_UP           26
# define CMD_DOWN         27
# define CMD_IZ           28
# define CMD_IZ_VAR_VAR   29
# define CMD_IZ_VAR_NUM   30
# define CMD_IZ_VAR_STR   31
# define CMD_EEKS         32
# define CMD_LOL_NUM      33
# define CMD_LOL_NUM_VAR  34
# define CMD_LOL_NUM_PRON 35
# define CMD_LOL_STR      36
# define CMD_LOL_STR_VAR  37
# define CMD_LOL_ERRORCODE 38
# define CMD_LOL_STRCAT   39
# define CMD_ENUF         40
# define CMD_GOSUB        41
# define CMD_RETURN       42
# define CMD_TOVARSTACKINT 43
# define CMD_TOVARSTACKSTR 44
# define CMD_TOVARSTACKINTVAR 45
# define CMD_TOVARSTACKSTRVAR 46
# define CMD_TOVARSTACKTYPE 47
# define CMD_TOVARSTACKLOCTYPE 48
# define CMD_TOVARSTACKPSTART 49
# define CMD_TOVARSTACKINTPAR 50
# define CMD_TOVARSTACKSTRPAR 51
# define CMD_TOVARSTACKINTVARPAR 52
# define CMD_TOVARSTACKSTRVARPAR 53
# define CMD_TOVARSTACKTYPEPAR 54
# define CMD_TOVARSTACKNAME 55
# define CMD_TOVARSTACKLOCNAME 56
# define CMD_TOVARSTACKNAMEPAR 57
# define CMD_RESERVE_PARAMS 58
# define CMD_ADD_VARIABLES 59
# define CMD_SUB_VARIABLES 60
# define CMD_MUL_VARIABLES 61
# define CMD_DIV_VARIABLES 62
# define CMD_GIMMEH        63
# define CMD_LOL_NUM_RND   64
# define CMD_ST0R          65
# define CMD_GIEF          66
# define CMD_IMP           67
# define CMD_EAT_MA        68
# define CMD_DBDUMP        69
# define CMD_LOL_INT2STR   70
# define CMD_LOL_STR2INT   71
# define CMD_INYRLOOP_VAR_NUM 72
# define CMD_INYRLOOP_VAR_VAR 73
# define CMD_LOL_STRLEN   74
# define CMD_WTF          75
# define CMD_OMG          76
# define CMD_OIC          77

# define MAXMONITOR  32
char *monitorvar[MAXMONITOR];
int monitorcnt = 0;

# define MAXDEBUGLINE 16384
int debug_line[MAXDEBUGLINE];
int debug_pptr[MAXDEBUGLINE];
char *debug_src[MAXDEBUGLINE];
int debuglcnt = 0;

# define MAXBREAKPOINTS 512
int breakpoints[MAXBREAKPOINTS];
int breakcnt = 0;

# define MAXBREAKIF 128
char *breakifvariable[MAXBREAKIF];
int breakifcondition[MAXBREAKIF];
long breakifvalue[MAXBREAKIF];
int breakifcnt;

# define MAXLINES 4096

char *flptr[MAXLINES];
int lcnt = 0;

char listfile[256];		// the *.lst of the current program
char programname[256];		// the program name with suffix 

char actfunction[256];
int actfunctionpos = 0;

# define MAXBYTES 256000
unsigned char bytes[MAXBYTES];
int bytecnt = 0;
int startbyte = -1;
int pptr = -1;			// the program pointer
# define MAXRETSTACK 1024
long retstack[MAXRETSTACK];
long varretstack[MAXRETSTACK];	// for the automatic
						// variables
long oldvarretstack[MAXRETSTACK];	// for the automatic
						// variables
int retsp = 0;

int errorcnt = 0;

FILE *fptab[32];
int fpcnt = 0;
char fname[32][256];
char listname[256];
time_t tsec;
struct tm *s_tm;

int opt_list = FALSE;
int opt_long_list = FALSE;
int opt_binary = FALSE;
int opt_hex = FALSE;
int opt_dump = FALSE;
int opt_dbg = FALSE;
int opt_trace = FALSE;
int opt_nobanners = FALSE;
int opt_wait_at_end = FALSE;
char password[256] = "";
FILE *fplist;
long linecount = 0;
long pagecount = 0;
int running = FALSE;
int hexmode = FALSE;
int gtim;

# define MAXVARIABLES 4096	// must be a multiple of 2 !!
# define GLOVARIABLE (MAXVARIABLES/2)	// here the global variables start!

char *varname[MAXVARIABLES];
long varcontent[MAXVARIABLES];
char vartype[MAXVARIABLES];

int glovarcnt = 0;		// count for global variables
int varcnt = 0;			// count for local variables, automatic memory
int gvarcnt = 0;		// to reset the variables at the end of a LULZ

// the pointer of the varstack, auto memory
// varptr points always at the end of the used part of the varstack
int varptr = 0;

// the old pointer of the varstack, auto memory
// oldvarptr points always at the beginning of the used part of the
// varstack
int oldvarptr = 0;

// to assign the types to the local variables
// locvarptr is a temporary pointer starting at oldvarptr to set the
// correct
// vartypes for the local variables
int locvarptr = 0;

// to assign the types to the parameter variables for the next called
// function
// it starts at varptr and writes into the part of the varstack used
// by the next called function
int parvarptr = 0;

# define MAXLOOPS 64
char loopname[MAXLOOPS][32];
long looppos[MAXLOOPS];
int loopcnt = 0;

# define MAXRELTAB 256
int reltab[MAXRELTAB];
char *relloop[MAXRELTAB];
int relcnt = 0;

int izkthxreltab[32];
int izkthxrelcnt = 0;

int izlevel = 0;
int nowaireltab[32];
int nowaiizlevel[32];
int nowairelcnt = 0;

# define MAXSTACK 64
int stack[MAXSTACK];
int sp = 0;
char stringstack[256];

char compiledate[32];

# define MAXFUNCTIONS 1024
char functiontab[MAXFUNCTIONS][32];	// the name of the
						// function
char paramtab[MAXFUNCTIONS][128];	// the parameters of the
						// function
int adresstab[MAXFUNCTIONS];	// the address of the
						// function for gosub
long varcnttab[MAXFUNCTIONS];	// the offset for the
						// local variables
// on varstack
int functioncnt = 0;

unsigned char gbuff[32768];
unsigned char gbuff2[32768];

int gerrorcode = 0;		// the global error code
int gerrorpptr = 0;		// the program pointer, where the error
				// occured

typedef struct dbstruct {
	unsigned char *data;
	unsigned char *key;
	unsigned char *category;
	void *next;
} dbstr;

dbstr *dbptr = NULL;

void do_coredump(char *msg);

int do_extra_checks = FALSE;
int debug_done = FALSE;

void do_debug(int pptr);
void do_trace(int p);

int wtf_variable = -1;

# define GO_ON \
	if(do_extra_checks==TRUE){ \
	   if(opt_dbg==TRUE){ \
	      debug_done=FALSE; \
          for(;;){        \
	         do_debug(pptr); \
			 if(debug_done==TRUE) break; \
		  }               \
	   }                  \
	   if(opt_trace==TRUE){ \
	      do_trace(pptr); \
	   }                  \
	}                     \
	switch (bytes[pptr++]) { \
	case CMD_PUT_VAR: goto do_put_var; \
	case CMD_PUT_NUM: goto do_put_num; \
	case CMD_PUT_STR: goto do_put_str; \
	case CMD_ADD: goto do_cmd_add; \
	case CMD_SUB: goto do_cmd_sub; \
	case CMD_MUL: goto do_cmd_mul; \
	case CMD_DIV: goto do_cmd_div; \
	case CMD_DUP: goto do_cmd_dup; \
	case CMD_SWAP: goto do_cmd_swap; \
	case CMD_ROT: goto do_cmd_rot; \
	case CMD_DROP: goto do_cmd_drop; \
	case CMD_SMALLER_NUM: goto do_smaller_num; \
	case CMD_EQUALS_NUM: goto do_equals_num; \
	case CMD_BIGGER_NUM: goto do_bigger_num; \
	case CMD_HAI: goto do_hai; 		 \
	case CMD_CAN_HAS: goto do_can_has; \
	case CMD_CAN_HAZ: goto do_can_haz;	 \
	case CMD_VISIBLE_TEXT: goto do_visible_text; \
	case CMD_VISIBLE_VAR: goto do_visible_var; \
	case CMD_GOTO: goto do_goto; \
	case CMD_KTHXBYE: goto do_kthxbye; \
	case CMD_UP: goto do_up; \
	case CMD_DOWN: goto do_down; \
	case CMD_IZ_VAR_NUM: goto do_iz_var_num; \
	case CMD_IZ_VAR_VAR: goto do_iz_var_var; \
	case CMD_IZ_VAR_STR: goto do_iz_var_str; \
	case CMD_EEKS: goto do_eeks; \
	case CMD_LOL_NUM: goto do_lol_num; \
	case CMD_LOL_NUM_VAR: goto do_lol_num_var; \
	case CMD_LOL_NUM_PRON: goto do_lol_num_pron; \
	case CMD_LOL_NUM_RND: goto do_lol_num_rnd; \
	case CMD_LOL_ERRORCODE: goto do_lol_errorcode; \
	case CMD_LOL_STRCAT: goto do_lol_strcat; \
	case CMD_LOL_STR: goto do_lol_str; \
	case CMD_LOL_STR_VAR: goto do_lol_str_var; \
	case CMD_GOSUB: goto do_gosub; \
	case CMD_RETURN: goto do_return; \
	case CMD_TOVARSTACKINT: goto do_tovarstackint; \
	case CMD_TOVARSTACKSTR: goto do_tovarstackstr; \
	case CMD_TOVARSTACKINTVAR: goto do_tovarstackintvar; \
	case CMD_TOVARSTACKSTRVAR: goto do_tovarstackstrvar; \
	case CMD_TOVARSTACKTYPE: goto do_tovarstacktype; \
	case CMD_TOVARSTACKLOCTYPE: goto do_tovarstackloctype; \
	case CMD_TOVARSTACKNAME: goto do_tovarstackname; \
	case CMD_TOVARSTACKLOCNAME: goto do_tovarstacklocname; \
	case CMD_TOVARSTACKNAMEPAR: goto do_tovarstacknamepar; \
	case CMD_TOVARSTACKPSTART: goto do_tovarstackpstart; \
	case CMD_TOVARSTACKINTPAR: goto do_tovarstackintpar; \
	case CMD_TOVARSTACKSTRPAR: goto do_tovarstackstrpar; \
	case CMD_TOVARSTACKINTVARPAR: goto do_tovarstackintvarpar; \
	case CMD_TOVARSTACKSTRVARPAR: goto do_tovarstackstrvarpar; \
	case CMD_TOVARSTACKTYPEPAR: goto do_tovarstacktypepar; \
	case CMD_RESERVE_PARAMS: goto do_reserve_params; \
	case CMD_ADD_VARIABLES: goto do_add_variables; \
	case CMD_SUB_VARIABLES: goto do_sub_variables; \
	case CMD_MUL_VARIABLES: goto do_mul_variables; \
	case CMD_DIV_VARIABLES: goto do_div_variables; \
	case CMD_GIMMEH: goto do_gimmeh; \
	case CMD_ST0R: goto do_st0r; \
	case CMD_GIEF: goto do_gief; \
	case CMD_IMP: goto do_imp; \
	case CMD_EAT_MA: goto do_eat_ma; \
	case CMD_DBDUMP: goto do_dbdump; \
	case CMD_LOL_INT2STR: goto do_lol_int2str; \
	case CMD_LOL_STR2INT: goto do_lol_str2int; \
	case CMD_INYRLOOP_VAR_NUM: goto do_inyrloop_var_num; \
	case CMD_INYRLOOP_VAR_VAR: goto do_inyrloop_var_var; \
	case CMD_LOL_STRLEN: goto do_lol_strlen; \
	default: \
		do_coredump("Unknown instruction"); \
		exit(80); \
	}

	//case CMD_WTF: goto do_wtf; \
/******************************************************************************/
void ucase(char *s)
{
	int c;
	int t;

	for (t = 0;; t++) {
		c = s[t];
		if (c == 0)
			break;
		s[t] = toupper(c);
	}
}

/******************************************************************************/
int strUcmp(char *s1, char *s2)
{
	char zwi1[32768];
	char zwi2[32768];

	strcpy(zwi1, s1);
	strcpy(zwi2, s2);
	ucase(zwi1);
	ucase(zwi2);

	return (strcmp(zwi1, zwi2));
}

/************************************************************************/
void print_string(unsigned char *s)
{
	int l;
	int t;

	l = s[0] + (256 * s[1]) + (256 * 256 * s[2]) + (256 * 256 * 256 * s[3]);

	for (t = 4; t < (l + 4); t++) {
		printf("%c", s[t]);
	}
	//puts("");
}

/************************************************************************/
unsigned char *make_string(unsigned char *str, long len)
{
	int l;
	unsigned char *newstr;

	//printf("make_string: >%s< len=%ld\n", str, len);
	newstr = malloc(len + 4 + 1);
	if (newstr == NULL) {
		do_coredump("Out of memory.");
		exit(81);
	}

	l = len;

	newstr[0] = l & 255;
	l /= 256;
	newstr[1] = l & 255;
	l /= 256;
	newstr[2] = l & 255;
	l /= 256;
	newstr[3] = l & 255;
	memcpy(newstr + 4, str, len);
	newstr[len + 4] = 0;

	return (newstr);
}

/************************************************************************/
// datastr = the pure data, without length
// str = 4 bytes length + data
unsigned char *extract_string(long *len, unsigned char *str)
{
	int l;
	int c;
	unsigned char *datastr;

	if (str == NULL) {
		*len = 0;
		return (NULL);
	}

	c = str[0];
	l = c;
	c = str[1];
	l = l + (c * 256);
	c = str[2];
	l = l + (c * 256 * 256);
	c = str[3];
	l = l + (c * 256 * 256 * 256);

	datastr = malloc(l + 4);
	if (datastr == NULL) {
		do_coredump("Out of memory.");
		exit(82);
	}
	memcpy(datastr, str + 4, l);
	datastr[l] = 0;

	*len = l;

	return (datastr);

}

/************************************************************************/
long get_string_length(unsigned char *str)
{
	int l;
	int c;

	c = str[0];
	l = c;
	c = str[1];
	l = l + (c * 256);
	c = str[2];
	l = l + (c * 256 * 256);
	c = str[3];
	l = l + (c * 256 * 256 * 256);

	return (l);
}

/************************************************************************/
unsigned char *copy_string(unsigned char *strsrc)
{
	int l;
	int c;
	unsigned char *strdest;

	if (strsrc == NULL)
		return (NULL);

	c = strsrc[0];
	l = c;
	c = strsrc[1];
	l = l + (c * 256);
	c = strsrc[2];
	l = l + (c * 256 * 256);
	c = strsrc[3];
	l = l + (c * 256 * 256 * 256);

	strdest = malloc(l + 4);
	if (strdest == NULL) {
		do_coredump("Out of memory.");
		exit(83);
	}

	memcpy(strdest, strsrc, l + 4);
	return (strdest);
}

/************************************************************************/
int read_one_line(FILE * fp, char *s)
{
	int c;
	int t;
	int z;
	for (t = 0, z = 0; t < 255; t++) {
		c = fgetc(fp);
		if (c == EOF || c == '\n' || c == '\r') {
			break;
		}
		if (c != '\r' && c != '\n') {
			s[z++] = c;
		}
	}
	s[z] = 0;
	return (c);
}

/******************************************************************************/
int is_this_a_digit(char c)
{
	switch (c) {
	case '0':
		return (TRUE);
	case '1':
		return (TRUE);
	case '2':
		return (TRUE);
	case '3':
		return (TRUE);
	case '4':
		return (TRUE);
	case '5':
		return (TRUE);
	case '6':
		return (TRUE);
	case '7':
		return (TRUE);
	case '8':
		return (TRUE);
	case '9':
		return (TRUE);
	}
	return (FALSE);
}

/************************************************************************/
int is_number(char *s)
{
	int t;

	if (s[0] == '+' || s[0] == '-' || is_this_a_digit(s[0]) == TRUE) {
		if (s[0] == '+' || s[0] == '-') {
			if (strlen(s) == 1) {	// thats the forth command + and -
				return (FALSE);
			}
		}
	}
	else {
		return (FALSE);
	}
	for (t = 1;; t++) {
		if (s[t] == 0)
			return (TRUE);
		if (is_this_a_digit(s[t]) == FALSE) {
			return (FALSE);
		}
	}
	return (TRUE);
}

/************************************************************************/
void write_line_to_list(char *s)
{
	// printf("write_line_to_list(%s)\n", s);
	if (opt_list == FALSE) {
		return;
	}

	if (linecount == 0) {
		pagecount++;
		// 12=Form Feed
		fprintf(fplist, "%c\n", 12);
		fprintf(fplist,
			"%- 49s %04d.%02d.%02d %02d:%02d:%02d  Page %04ld\n",
			fname[0], s_tm->tm_year, s_tm->tm_mon, s_tm->tm_mday,
			s_tm->tm_hour, s_tm->tm_min, s_tm->tm_sec, pagecount);
		fprintf(fplist,
			"--------------------------------------------------------------------------------\n");
	}
	linecount++;
	fprintf(fplist, "%s\n", s);
	if (linecount > 77) {
		linecount = 0;
	}
}

/************************************************************************/
/*
 * Retrieves word No. n from string *s and stores it in *ziel. * n starts
 * with 1, not with 0 !!! * Escape-Character is \, strings are delimited
 * by ". 
 */
char *__xpjg_wordptr = NULL;
int __xpjg_wordpos = 0;
void word(char *s, char *ziel, int n)
{
	int anz = 0;
	int t;
	int f = 0;
	int z = 0;
	int start;
	int m = 1;
	int esc = 0;
	int l;

	__xpjg_wordptr = s;

	l = (int)strlen((char *)s);
	for (start = 0; start < l; start++)
		if (s[start] > ' ')
			break;

	for (t = start; t < l; t++) {
		if (s[t] == '\\') {
			if (esc == 0) {
				esc = 1;
				if (anz == n)
					ziel[z++] = '\\';
			}
			else {
				if (anz == n)
					ziel[z++] = '\\';
				esc = 0;
			}
		}
		else {
			if (s[t] == '"' && esc == 0)
				m *= -1;
			if (s[t] == ' ' && m == 1)
				f = 0;
			else
				f++;
			if (f == 1)
				anz++;
			if (anz == n && f > 0) {
				ziel[z++] = s[t];
			}
			if (anz > n && f > 0)
				break;
			esc = 0;
		}
	}			/* next t */
	ziel[z] = 0;
	__xpjg_wordpos = t;
	l = strlen((char *)ziel) - 1;	/* Trim trailing spaces */
	for (;;) {
		if (l < 1)
			break;
		if (ziel[l] != ' ')
			break;
		ziel[l] = 0;
		l--;
	}
}

/********************************************************************/
void next_word(unsigned char *ziel)
{
	int anz = 0;
	int t;
	int f = 0;
	int z = 0;
	int m = 1;
	int esc = 0;
	int l;
	char *s;

	s = __xpjg_wordptr;
	if (s == NULL) {
		ziel[0] = 0;
		return;
	}
	l = strlen((char *)s);
	for (t = __xpjg_wordpos; t < l; t++) {
		if (s[t] == '\\') {
			if (esc == 0) {
				esc = 1;
				ziel[z++] = '\\';
			}
			else {
				ziel[z++] = '\\';
				esc = 0;
			}
		}
		else {
			if (s[t] == '"' && esc == 0)
				m *= -1;
			if (s[t] == ' ' && m == 1)
				f = 0;
			else
				f++;
			if (f == 1)
				anz++;
			if (anz > 1 && f > 0)
				break;
			if (f > 0) {
				ziel[z++] = s[t];
			}
			esc = 0;
		}
	}			/* next t */
	ziel[z] = 0;
	if (z == 0) {
		__xpjg_wordptr = NULL;
		__xpjg_wordpos = 0;
	}
	else {
		__xpjg_wordpos = t;
	}
	l = strlen((char *)ziel) - 1;	/* Trim trailing spaces */
	for (;;) {
		if (l < 1)
			break;
		if (ziel[l] != ' ')
			break;
		ziel[l] = 0;
		l--;
	}
}

/********************************************************************/
/*
 * Counts all words in *s and returns the count. 
 */
int words(unsigned char *s)
{
	int anz = 0;
	int t;
	int f = 0;
	int start;
	int m = 1;
	int esc = 0;
	int l;

	l = (int)strlen((char *)s);
	for (start = 0; start < l; start++)
		if (s[start] > ' ')
			break;

	for (t = start; t < l; t++) {
		if (s[t] == '\\') {
			if (esc == 0) {
				esc = 1;
			}
			else {
				esc = 0;
			}
		}
		else {
			if (s[t] == '"' && esc == 0)
				m *= -1;
			if (s[t] == ' ' && m == 1)
				f = 0;
			else
				f++;
			if (f == 1)
				anz++;
			esc = 0;
		}
	}			/* next t */
	return (anz);
}

/************************************************************************/
void trim(char *s)
{
	int t;

	for (;;) {
		if (s[0] == 0)
			return;
		if (s[strlen(s) - 1] <= ' ') {
			s[strlen(s) - 1] = 0;
		}
		else {
			break;
		}
	}

	for (;;) {
		if (s[0] == 0)
			return;

		if (s[0] <= ' ') {
			for (t = 1;; t++) {
				s[t - 1] = s[t];
				if (s[t] == 0)
					break;
			}
		}
		else {
			break;
		}
	}
}

/******************************************************************************/
int read_signed_number(void)
{

	int c;

	int a;

	c = bytes[pptr++];

	a = c;

	c = bytes[pptr++];

	a = a + (c * 256);

	c = bytes[pptr++];

	a = a + (c * 256 * 256);

	c = bytes[pptr++];

	if (c >= 128) {

		c -= 128;	// remove hi bit
		a = a + (c * 256 * 256 * 256);

		a *= -1;	// result is now negative!!
	}

	else {

		a = a + (c * 256 * 256 * 256);

	}

	return (a);

}

/******************************************************************************/
void write_signed_number(int nbr)
{

	int a;

	int c;

	if (nbr < 0) {

		nbr *= -1;

		a = nbr & 255;

		bytes[bytecnt++] = a;

		nbr /= 256;

		a = nbr & 255;

		bytes[bytecnt++] = a;

		nbr /= 256;

		a = nbr & 255;

		bytes[bytecnt++] = a;

		nbr /= 256;

		a = nbr & 255;

		if (a >= 128) {	// make sure the hi bit is cleared
			a -= 128;

		}

		a += 128;	// now set the hi bit ==> negative value
		bytes[bytecnt++] = a;

	}

	else {

		c = nbr;

		bytes[bytecnt++] = c & 255;

		c = c / 256;

		bytes[bytecnt++] = c & 255;

		c = c / 256;

		bytes[bytecnt++] = c & 255;

		c = c / 256;

		if (c > 128) {	// make sure the hi bit is cleared
			c -= 128;	// no hi bit ==> positive value
		}

		bytes[bytecnt++] = c & 255;

	}

}

/******************************************************************************/
int get_variable_index(char *s, int checkparm)
{
	int t;
	int a;
	char zwi[256];
	char zwi2[256];
	strcpy(zwi, s);
	ucase(zwi);

	if (checkparm == 1) {	// yes, check for parameter variables
		// is this a parameter variable? And does it fit with the actual
		// function?
		if (strchr(zwi, '.') != NULL) {
			sprintf(zwi2, "%s.", actfunction);
			// printf("compare >%s<->%s<\n", zwi, zwi2);
			if (strncmp(zwi, zwi2, strlen(zwi2)) != 0) {	// no,
				// does
				// not fit
				// puts("does not fit!!");
				return (-1);
			}
		}
	}

	for (t = 0; t < varcnt; t++) {

		// printf("compare>%s<->%s<\n", varname[t], zwi);
		if (strcmp(varname[t], zwi) == 0) {

			a = t;

			return (a);

		}

	}

	for (t = 0; t < glovarcnt; t++) {

		// printf("compare>%s<->%s<\n", varname[t], zwi);
		if (strcmp(varname[t + GLOVARIABLE], zwi) == 0) {

			a = t + GLOVARIABLE;

			return (a);

		}

	}

	return (-1);
}

/******************************************************************************/
int add_variable(char *s)
{
	int t;
	char zwi[256];
	int c;
	int vtype;

	if (strcmp(actfunction, "HAI") == 0) {
		fprintf(stderr,
			"ERROR 00021 - No variable definitions allowed in HAI section\n");
		sprintf(zwi,
			"ERROR 00021 - No variable definitions allowed in HAI section.");
		write_line_to_list(zwi);
		errorcnt++;
		return (-1);
	}

	strcpy(zwi, s);
	ucase(zwi);

	c = zwi[strlen(zwi) - 1];	// get the variable type (%, $)
	zwi[strlen(zwi) - 1] = 0;

	switch (c) {
	case '%':
		vtype = '%';
		break;
	case '$':
		vtype = '$';
		break;
	default:
		fprintf(stderr, "ERROR 00007 - Unknown vartype >%c<.\n", c);
		sprintf(zwi, "ERROR 00007 - Unknown vartype >%c<.", c);
		write_line_to_list(zwi);
		errorcnt++;
		return (-1);
		break;
	}
	for (t = 0; t < varcnt; t++) {

		if (varname[t] != NULL) {

			if (strcmp(varname[t], zwi) == 0) {

				return (-1);

			}

		}

	}

	for (t = 0; t < glovarcnt; t++) {

		if (varname[t + GLOVARIABLE] != NULL) {

			if (strcmp(varname[t + GLOVARIABLE], zwi) == 0) {

				return (-1);

			}

		}

	}

	if (varcnt >= (MAXVARIABLES - 1)) {
		fprintf(stderr,
			"ERROR 00005 - Overflow. More than %d variables.\n",
			MAXVARIABLES);
		sprintf(zwi, "ERROR 00005 - Overflow. More than %d variables.",
			MAXVARIABLES);
		write_line_to_list(zwi);
		errorcnt++;
		return (0);
	}

	if (vtype == '%') {
		varcontent[varcnt] = 0;
	}
	if (vtype == '$') {
		// make this more intelligent later!!!
		varcontent[varcnt] = 0;
	}
	vartype[varcnt] = vtype;
	//printf("add_variable: %d >%s< type=>%c<\n", varcnt,
	//      varname[varcnt], vartype[varcnt]);
	if (strlen(actfunction) == 0) {
		//printf("globale variable: glovarcnt=%d\n", glovarcnt);
		vartype[glovarcnt + GLOVARIABLE] = vtype;
		//printf("vartype[%d+%d]=%c\n", glovarcnt, GLOVARIABLE,
		//       vartype[glovarcnt + GLOVARIABLE]);
		varcontent[glovarcnt + GLOVARIABLE] = 0;
		//printf("varcontent[%d+%d]=%d\n", glovarcnt, GLOVARIABLE,
		//       varcontent[glovarcnt + GLOVARIABLE]);

		varname[glovarcnt + GLOVARIABLE] = malloc(strlen(zwi) + 1);

		strcpy(varname[glovarcnt + GLOVARIABLE], zwi);
		//printf("varname[%d+%d]=>%s<\n", glovarcnt, GLOVARIABLE,
		//       varname[glovarcnt + GLOVARIABLE]);
		glovarcnt++;

	}
	else {
		vartype[varcnt] = vtype;

		varcontent[varcnt] = 0;

		varname[varcnt] = malloc(strlen(zwi) + 1);

		strcpy(varname[varcnt], zwi);

		varcnt++;

	}

	return (0);
}

/******************************************************************************/
void do_coredump(char *msg)
{
	int t;
	int i;
	int a;
	int c;
	char zwi[256];
	char zwi2[256];

	if (opt_dump == FALSE) {

		if (opt_wait_at_end == TRUE) {

			puts(" * ERROR * Press ENTER key to terminate..");

			c = fgetc(stdin);

		}
		return;

	}

	fprintf(stderr, "-------- Core Dump ---------\n");
	fprintf(stderr,
		"File name: %- 39s         %04d.%02d.%02d %02d:%02d:%02d\n",
		fname[0], s_tm->tm_year, s_tm->tm_mon, s_tm->tm_mday,
		s_tm->tm_hour, s_tm->tm_min, s_tm->tm_sec);
	fprintf(stderr, "Compile date: %s\n", compiledate);
	fprintf(stderr, "At %08d (Instruction %03d): %s\n", pptr, bytes[pptr],
		msg);
	if (bytes[pptr] == CMD_EEKS) {
		fprintf(stderr, "Instruction EEKS! found, abort on purpose!\n");
	}

	fprintf(stderr, "--Dump of global variables:--\n");

	for (t = 0; t < glovarcnt; t++) {

		if (vartype[t + GLOVARIABLE] == '%') {

			a = varcontent[t + GLOVARIABLE];

			fprintf(stderr, "%04d: %s Integer %d\n", t,
				varname[t + GLOVARIABLE], a);

		}

		if (vartype[t] == '$') {

			a = varcontent[t];

			fprintf(stderr, "%04d: %s String  >%s<\n", t,
				varname[t + GLOVARIABLE], (char *)a);

		}

	}
	fprintf(stderr, "----------------------------\n");

	fprintf(stderr, "---Dump of local variables:--\n");

	for (t = 0; t < varcnt; t++) {

		if (vartype[t] == '%') {

			a = varcontent[t];

			fprintf(stderr, "%04d: %s Integer %d\n", t, varname[t],
				a);

		}

		if (vartype[t] == '$') {

			a = varcontent[t];

			fprintf(stderr, "%04d: %s String  >%s<\n", t,
				varname[t], (char *)a);

		}

	}
	fprintf(stderr, "----------------------------\n");

	fprintf(stderr, "-------- Stack Dump --------\n");
	fprintf(stderr, "sp=%d\n", sp);
	for (t = 0; t < MAXSTACK; t++) {
		fprintf(stderr, "%04d: %- 3d [%02X]\n", t, stack[t], stack[t]);
	}
	fprintf(stderr, "----------------------------\n");
	fprintf(stderr, "---Dump of program memory---\n");
	for (t = 0; t < bytecnt; t += 16) {
		sprintf(zwi,
			"%08d                                                                      ",
			t);
		for (i = 0; i < 16; i++) {
			c = bytes[t + i];
			sprintf(zwi2, "%02X", c);
			if (i < 8) {
				zwi[(i * 3) + 10] = zwi2[0];
				zwi[(i * 3) + 11] = zwi2[1];
			}
			else {
				zwi[(i * 3) + 11] = zwi2[0];
				zwi[(i * 3) + 12] = zwi2[1];
			}
			if (isprint(c)) {
			}
			else {
				c = '.';
			}
			if (i < 8) {
				zwi[60 + i] = c;
			}
			else {
				zwi[61 + i] = c;
			}
		}
		fprintf(stderr, "%s\n", zwi);
	}
	fprintf(stderr, "* * * * * * * * * * * * *\n");
	fprintf(stderr, "* * * T E H   E N D * * *\n");
	fprintf(stderr, "* * * * * * * * * * * * *\n");
	printf(">>>Core dump written, program aborted.\n");
}

/******************************************************************************/
int get_loop_pos(char *s)
{
	char zwi[256];
	int t;

	// printf("get_loop_pos(%s) loopcnt=%d\n", s, loopcnt);
	strcpy(zwi, s);
	ucase(zwi);
	for (t = 0; t < loopcnt; t++) {
		if (strcmp(loopname[t], zwi) == 0) {
			// printf("get_loop_pos(%s) %ld\n", zwi, looppos[t]);
			return (looppos[t]);
		}
	}
	return (-1);
}

/******************************************************************************/
int add_loop(char *s, int bpos)
{
	char zwi[256];

	if (loopcnt >= (MAXLOOPS - 1)) {
		fprintf(stderr, "ERROR 00009 - Too many nested loops >%s<.\n",
			s);
		sprintf(zwi, "ERROR 00009 - Too many nested loops >%s<.", s);
		write_line_to_list(zwi);
		errorcnt++;
		return (0);
	}

	strcpy(zwi, s);
	ucase(zwi);

	strcpy(loopname[loopcnt], zwi);
	looppos[loopcnt] = bpos;	// the position where the loop starts
	loopcnt++;

	return (0);
}

/******************************************************************************/
int check_loop_name(char *s)
{
	char zwi[256];
	int t;

	strcpy(zwi, s);
	trim(zwi);
	if (strlen(zwi) > 32)
		return (FALSE);

	if (isalpha(zwi[0] == FALSE)) {
		return (FALSE);

	}
	for (t = 1; t < (int)(strlen(zwi) - 1); t++) {	// dont test the first
		// byte
		if (zwi[t] == 0)
			break;
		if (isalnum(zwi[t]) == FALSE) {
			if (zwi[t] != '-') {
				if (zwi[t] != '_') {
					return (FALSE);
				}
			}
		}
	}

	for (t = 0; t < loopcnt; t++) {
		if (strcmp(loopname[t], zwi) == 0) {
			return (FALSE);
		}
	}

	return (TRUE);
}

/******************************************************************************/
int check_variable_name(char *s)
{
	char zwi[256];
	int t;

	strcpy(zwi, s);
	trim(zwi);
	if (strlen(zwi) > 32)
		return (FALSE);

	if (isalpha(zwi[0] == FALSE))
		return (FALSE);
	for (t = 1; t < (int)(strlen(zwi) - 2); t++) {	// dont test the first and 
		// last byte(vartype)
		if (zwi[t] == 0)
			break;
		if (isalnum(zwi[t]) == FALSE) {
			if (zwi[t] != '-') {
				if (zwi[t] != '_') {
					return (FALSE);
				}
			}
		}
	}

	return (TRUE);
}

/******************************************************************************/
void do_copybook(char *s)
{
	char zwi[256];
	char zwi2[256];

	word(s, zwi, 3);
	fpcnt++;

	// printf("do_copybook(1) zwi=>%s< fpcnt=%d\n", zwi, fpcnt);

	if (zwi[strlen(zwi) - 1] == '?') {	// remove the optional
		// questionmark
		zwi[strlen(zwi) - 1] = 0;
	}
	strcat(zwi, ".cpy");
	// printf("do_copybook(2) zwi=>%s<\n", zwi);

	strcpy(fname[fpcnt], zwi);
	fptab[fpcnt] = fopen(fname[fpcnt], "rb");
	if (fptab[fpcnt] == NULL) {
		fprintf(stderr, "Couldn't fucking open >%s< for input you Jew, aborting this shit...\n",
			fname[fpcnt]);
		exit(1);
	}
	setvbuf(fptab[fpcnt], NULL, _IOFBF, 32768);
	// printf("do_copybook(3) zwi=>%s< fpcnt=%d\n", zwi, fpcnt);
	write_line_to_list("   ");
	write_line_to_list("   ");
	sprintf(zwi2, "         * * * %s * * *\n", zwi);
	write_line_to_list(zwi2);
	// printf("do_copybook(4) zwi=>%s< fpcnt=%d\n", zwi, fpcnt);
}

/******************************************************************************/
void remove_comment(char *s)
{
	int t;

	if (s[0] == '*') {
		s[0] = 0;
	}

	for (t = 0;; t++) {
		if (s[t] == 0) {
			break;
		}
		if (toupper(s[t]) == 'B' &&
		    toupper(s[t + 1]) == 'T' && toupper(s[t + 2]) == 'W') {
			s[t] = 0;
			break;
		}
	}

}

/************************************************************************/
void write_pron_word(char *s)
{
	int a;
	int value = 0;
	char zwi[256];

	if (is_number(s) == TRUE) {
		bytes[bytecnt++] = CMD_PUT_NUM;
		value = atoi(s);
		bytes[bytecnt++] = value & 255;
		value = value / 256;
		bytes[bytecnt++] = value & 255;
		value = value / 256;
		bytes[bytecnt++] = value & 255;
		value = value / 256;
		bytes[bytecnt++] = value & 255;
		return;
	}

	a = get_variable_index(s, 1);
	if (a != -1) {
		// a += oldvarptr;      // automatic memory!! not at compile time!!!!!
		bytes[bytecnt++] = CMD_PUT_VAR;
		value = a;
		bytes[bytecnt++] = value & 255;
		value = value / 256;
		bytes[bytecnt++] = value & 255;
		return;
	}

	strcpy(zwi, s);
	ucase(zwi);
	if (strcmp(zwi, "+") == 0) {
		bytes[bytecnt++] = CMD_ADD;
		return;
	}

	if (strcmp(zwi, "-") == 0) {
		bytes[bytecnt++] = CMD_SUB;
		return;
	}

	if (strcmp(zwi, "*") == 0) {
		bytes[bytecnt++] = CMD_MUL;
		return;
	}

	if (strcmp(zwi, "/") == 0) {
		bytes[bytecnt++] = CMD_DIV;
		return;
	}

	if (strcmp(zwi, "SWAP") == 0) {
		bytes[bytecnt++] = CMD_SWAP;
		return;
	}

	if (strcmp(zwi, "ROT") == 0) {
		bytes[bytecnt++] = CMD_ROT;
		return;
	}

	if (strcmp(zwi, "DROP") == 0) {
		bytes[bytecnt++] = CMD_DROP;
		return;
	}

	if (strcmp(zwi, "DUP") == 0) {
		bytes[bytecnt++] = CMD_DUP;
		return;
	}
	if (strcmp(zwi, "<") == 0) {
		bytes[bytecnt++] = CMD_SMALLER_NUM;
		return;
	}
	if (strcmp(zwi, "=") == 0) {
		bytes[bytecnt++] = CMD_EQUALS_NUM;
		return;
	}
	if (strcmp(zwi, ">") == 0) {
		bytes[bytecnt++] = CMD_BIGGER_NUM;
		return;
	}

	fprintf(stderr, "ERROR 00017 - Unknown PRON word >%s<.\n", s);
	sprintf(zwi, "ERROR 00017 - Unknown PRON word >%s<.", s);
	write_line_to_list(zwi);
	errorcnt++;
	return;
}

/************************************************************************/
int parse_iz_statement(char *s)
{
	char zwi[256];
	char zwi2[256];
	char zwi3[256];
	char zwi4[256];
	char zwi5[256];
	int a;
	int b;
	int c;
	int t;
	int cmd;
	int operator;

	izlevel++;

	word(s, zwi2, 2);

	// form 1 or form 2 (PRON) ??
	// Form 1 PRON
	if (strcmp(zwi2, "PRON") == 0) {
		cmd = CMD_IZ;
		for (t = 3;; t++) {
			word(s, zwi, t);
			if (strlen(zwi) == 0)
				break;
			if (strcmp(zwi, "BTW") == 0)
				break;
			write_pron_word(zwi);
		}
		bytes[bytecnt++] = CMD_IZ;
		nowaiizlevel[nowairelcnt] = izlevel;
		nowaireltab[nowairelcnt++] = bytecnt;	// relocate it after the
		// NOWAI
		//puts("nowairelcnt++ 333");
		bytes[bytecnt++] = 0;
		bytes[bytecnt++] = 0;
		bytes[bytecnt++] = 0;
		bytes[bytecnt++] = 0;
		return (cmd);
	}

	/*
	 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	 * - 
	 */
	// Form 2
	word(s, zwi3, 3);
	ucase(zwi3);
	word(s, zwi4, 4);
	ucase(zwi4);
	word(s, zwi5, 5);

	a = get_variable_index(zwi2, 1);
	if (a == -1) {
		fprintf(stderr, "ERROR 00006 -1- - Unknown variable >%s<.\n",
			zwi2);
		sprintf(zwi, "ERROR 00006 -1- - Unknown variable >%s<.", zwi2);
		write_line_to_list(zwi);
		errorcnt++;
		return (0);
	}
	// a+=oldvarptr; // automatic memory!! not at compile time !!!
	if (strcmp(zwi3, "BIGGER") == 0 ||
	    strcmp(zwi3, "SMALLER") == 0 || strcmp(zwi3, "EQUAL") == 0) {
		if (strcmp(zwi3, "BIGGER") == 0)
			operator= 97;
		if (strcmp(zwi3, "SMALLER") == 0)
			operator= 98;
		if (strcmp(zwi3, "EQUAL") == 0)
			operator= 99;
	}
	else {
		fprintf(stderr,
			"ERROR 00013 -1- Syntax error in IZ statement.\n");
		sprintf(zwi, "ERROR 00013 -1- Syntax error in IZ statement.");
		write_line_to_list(zwi);
		errorcnt++;
		return (0);
	}
	if (strcmp(zwi4, "THAN") == 0) {
	}
	else {
		fprintf(stderr,
			"ERROR 00013 -2- Syntax error in IZ statement.\n");
		sprintf(zwi, "ERROR 00013 -2- Syntax error in IZ statement.");
		write_line_to_list(zwi);
		errorcnt++;
		return (0);
	}
	if (zwi5[strlen(zwi5) - 1] == '?') {
		zwi5[strlen(zwi5) - 1] = 0;
	}
	if (is_number(zwi5) == TRUE) {
		if (vartype[a] != '%') {
			fprintf(stderr,
				"ERROR 00014 - Vartype mismatch in IZ statement.\n");
			sprintf(zwi,
				"ERROR 00014 - Vartype mismatch in IZ statement.");
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		cmd = CMD_IZ_VAR_NUM;
	}
	else {
		if (zwi5[0] == '"') {
			if (vartype[a] != '$') {
				fprintf(stderr,
					"ERROR 00014 - Vartype mismatch in IZ statement.\n");
				sprintf(zwi,
					"ERROR 00014 - Vartype mismatch in IZ statement.");
				write_line_to_list(zwi);
				errorcnt++;
				return (0);
			}
			cmd = CMD_IZ_VAR_STR;
		}
		else {
			b = get_variable_index(zwi5, 1);
			if (b == -1) {
				fprintf(stderr,
					"ERROR 00006 -2- - Unknown variable >%s<.\n",
					zwi5);
				sprintf(zwi,
					"ERROR 00006 -2- - Unknown variable >%s<.",
					zwi5);
				write_line_to_list(zwi);
				errorcnt++;
				return (0);
			}
			// b+=oldvarptr; // automatic memory!! not at compile time !!!!
			cmd = CMD_IZ_VAR_VAR;
		}
	}
	bytes[bytecnt++] = cmd;	// command
	c = a;			// first variable
	bytes[bytecnt++] = c & 255;
	c = c / 256;
	bytes[bytecnt++] = c & 255;
	bytes[bytecnt++] = operator;	// operator
	if (cmd == CMD_IZ_VAR_VAR) {
		c = b;		// second variable
		bytes[bytecnt++] = c & 255;
		c = c / 256;
		bytes[bytecnt++] = c & 255;
	}
	if (cmd == CMD_IZ_VAR_NUM) {
		c = atoi(zwi5);	// 2nd, the number
		write_signed_number(c);
	}
	if (cmd == CMD_IZ_VAR_STR) {	// 2nd, the string
		for (t = 1;; t++) {
			c = zwi5[t];
			if (c == 0 || c == '"') {
				bytes[bytecnt++] = 0;
				break;
			}
			bytes[bytecnt++] = c;
		}
	}
	// printf("IZ: nowairelcnt=%d\n", nowairelcnt);
	nowaiizlevel[nowairelcnt] = izlevel;
	nowaireltab[nowairelcnt++] = bytecnt;	// relocate it after the
	// KTHX or NOWAI
	bytes[bytecnt++] = 0;
	bytes[bytecnt++] = 0;
	bytes[bytecnt++] = 0;
	bytes[bytecnt++] = 0;

	return (0);
}

/************************************************************************/
void handle_function_parameter(char *s, int pos)
{
	int t, i;
	int a, b;
	int c;
	int l;
	int cmd;
	long value;
	char zwi[256];
	char zwi2[256];
	char zwi3[256];
	char fname[256];
	int lvarptr;
	char vtype;

	lvarptr = varptr;
	word(s, fname, 1);
	bytes[bytecnt++] = CMD_TOVARSTACKPSTART;	// a new parameter list
	// starts
	for (t = 1;; t++) {
		word(paramtab[pos], zwi3, t);
		if (strlen(zwi3) == 0)
			break;
		// printf("zwi3=>%s<\n", zwi3);
		vtype = zwi3[strlen(zwi3) - 1];	// get the parameter var type
		word(s, zwi2, t + 1);	// what to assign to that var?
		// printf("param = >%s<->%s<\n", zwi, zwi2);
		if (zwi2[0] == '"') {	// it's a string constant
			// printf("vtype=>%c<\n", vtype);
			if (vtype != '$') {
				fprintf(stderr,
					"ERROR 00015 -1- Vartype mismatch in LOL statement.\n");
				sprintf(zwi,
					"ERROR 00015 -1- Vartype mismatch in LOL statement.");
				write_line_to_list(zwi);
				errorcnt++;
				return;
			}
			cmd = CMD_LOL_STR;
		}
		else {
			if (is_number(zwi2) == TRUE) {
				if (vtype != '%') {
					fprintf(stderr,
						"ERROR 00015 -2- Vartype mismatch in LOL statement.\n");
					sprintf(zwi,
						"ERROR 00015 -2- Vartype mismatch in LOL statement.");
					write_line_to_list(zwi);
					errorcnt++;
					return;
				}
				cmd = CMD_LOL_NUM;
			}
			else {
				b = get_variable_index(zwi2, 1);
				if (b == -1) {
					fprintf(stderr,
						"ERROR 00006 -3- - Unknown variable >%s<.\n",
						zwi2);
					sprintf(zwi,
						"ERROR 00006 -3- - Unknown variable >%s<.",
						zwi2);
					write_line_to_list(zwi);
					errorcnt++;
					return;
				}
				// b += oldvarptr;      // automatic memory!! not at compilie time !!!!
				if (vartype[a] != vartype[b]) {
					fprintf(stderr,
						"ERROR 00015 -3- Vartype mismatch in LOL statement.\n");
					sprintf(zwi,
						"ERROR 00015 -3- Vartype mismatch in LOL statement.");
					write_line_to_list(zwi);
					errorcnt++;
					return;
				}
				if (vartype[a] == '%') {
					cmd = CMD_LOL_NUM_VAR;
				}
				else {
					cmd = CMD_LOL_STR_VAR;
				}
			}
		}
		// printf("handle_function_parameter, cmd=%d\n", cmd);
		if (cmd == CMD_LOL_STR) {
			// printf("bytecnt=%d cmd=%d\n", bytecnt, CMD_TOVARSTACKTYPE);
			if (opt_dbg == TRUE) {	// for debug add now the variable name
				bytes[bytecnt++] = CMD_TOVARSTACKNAMEPAR;
				for (t = 0;; t++) {
					if (zwi3[t] == 0 || zwi3[t] == '$')
						break;
					bytes[bytecnt++] = zwi3[t];
				}
				bytes[bytecnt++] = 0;
			}
			bytes[bytecnt++] = CMD_TOVARSTACKTYPEPAR;
			bytes[bytecnt++] = '$';
			bytes[bytecnt++] = CMD_TOVARSTACKSTRPAR;	// the memory
			// the string itself
			strcpy(zwi3, zwi2 + 1);	// get rid of the leading and trailing "
			zwi3[strlen(zwi3) - 1] = 0;
			l = strlen(zwi3) + 1;
			bytes[bytecnt++] = l & 255;	// the string length, 4 bytes
			l = l / 256;
			bytes[bytecnt++] = l & 255;
			l = l / 256;
			bytes[bytecnt++] = l & 255;
			l = l / 256;
			bytes[bytecnt++] = l & 255;
			l = l / 256;
			for (i = 0;; i++) {
				c = zwi3[i];
				bytes[bytecnt++] = c;
				if (c == 0)
					break;
			}
		}
		if (cmd == CMD_LOL_NUM) {
			if (opt_dbg == TRUE) {	// for debug add now the variable name
				bytes[bytecnt++] = CMD_TOVARSTACKNAMEPAR;
				for (t = 0;; t++) {
					if (zwi3[t] == 0 || zwi3[t] == '$')
						break;
					bytes[bytecnt++] = zwi3[t];
				}
				bytes[bytecnt++] = 0;
			}
			bytes[bytecnt++] = CMD_TOVARSTACKTYPEPAR;
			bytes[bytecnt++] = '%';
			bytes[bytecnt++] = CMD_TOVARSTACKINTPAR;
			value = atoi(zwi2);	// the value to assign to variable
			write_signed_number(value);
		}
		if (cmd == CMD_LOL_NUM_VAR) {
			if (opt_dbg == TRUE) {	// for debug add now the variable name
				bytes[bytecnt++] = CMD_TOVARSTACKNAMEPAR;
				for (t = 0;; t++) {
					if (zwi3[t] == 0 || zwi3[t] == '$')
						break;
					bytes[bytecnt++] = zwi3[t];
				}
				bytes[bytecnt++] = 0;
			}
			bytes[bytecnt++] = CMD_TOVARSTACKTYPEPAR;
			bytes[bytecnt++] = '%';
			bytes[bytecnt++] = CMD_TOVARSTACKINTVARPAR;	// the variable
			// index
			bytes[bytecnt++] = b & 255;
			b = b / 256;
			bytes[bytecnt++] = b & 255;
		}
		if (cmd == CMD_LOL_STR_VAR) {
			if (opt_dbg == TRUE) {	// for debug add now the variable name
				bytes[bytecnt++] = CMD_TOVARSTACKNAMEPAR;
				for (t = 0;; t++) {
					if (zwi3[t] == 0 || zwi3[t] == '$')
						break;
					bytes[bytecnt++] = zwi3[t];
				}
				bytes[bytecnt++] = 0;
			}
			bytes[bytecnt++] = CMD_TOVARSTACKTYPEPAR;
			bytes[bytecnt++] = '$';
			bytes[bytecnt++] = CMD_TOVARSTACKSTRVARPAR;	// the variable
			// index
			bytes[bytecnt++] = b & 255;
			b = b / 256;
			bytes[bytecnt++] = b & 255;
		}
	}			// next
}

/************************************************************************/
int parse_command(char *s)
{
	char zwi[256];
	char zwi2[256];
	char zwi3[256];
	char zwi4[256];
	char zwi5[256];
	char zwi6[256];
	char zwi7[256];
	char zwi8[256];
	char zwi9[256];
	char zwi10[256];
	char zwi11[256];
	char zwi12[256];
	char zwi13[256];
	char ozwi4[256];
	int a;
	int b;
	int c;
	int t;
	int f;
	int pos;
	int cmd;
	int crlf = FALSE;
	int value;
	char vtype;
	int parmcnt;

	//printf("parse_command(%s) bytecnt=%d\n", s, bytecnt);
	//printf("parse_command:vartype[2048]=>%c<\n", vartype[2048]);
	// for(t=0; t<3; t++){
	// printf("%d) %d\n", t, bytes[t]);
	// }

	if (s[0] == 0 || s[0] == '*')
		return (0);

	word(s, zwi, 1);

	if (strcmp(zwi, "HAI") == 0) {
		strcpy(actfunction, "HAI");
		actfunctionpos = 0;
		startbyte = bytecnt;	// start execution right here
		// printf("parse_command: bytecnt=%d, CMD_HAI=%d\n", bytecnt,
		// CMD_HAI);
		bytes[bytecnt++] = CMD_HAI;
		gvarcnt = varcnt;	// to reset the varstack at the end of the 
		// HAI
		return (CMD_HAI);
	}

	if (strcmp(zwi, "EEKS!") == 0) {
		bytes[bytecnt++] = CMD_EEKS;
		return (CMD_EEKS);
	}

	if (strcmp(zwi, "DBDUMP") == 0) {
		bytes[bytecnt++] = CMD_DBDUMP;
		return (CMD_DBDUMP);
	}

	if (strcmp(zwi, "ST0R") == 0) {
		word(s, zwi2, 2);	// var1
		word(s, zwi3, 3);	// WITH
		word(s, zwi4, 4);	// var2
		word(s, zwi5, 5);	// IN
		word(s, zwi6, 6);	// var3
		ucase(zwi3);
		if (strcmp(zwi3, "WITH") != 0) {
			fprintf(stderr,
				"ERROR 00023 - Syntax error in ST0R statement.\n");
			sprintf(zwi,
				"ERROR 00023 - Syntax error in ST0R statement.");
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		ucase(zwi5);
		if (strcmp(zwi5, "IN") != 0) {
			fprintf(stderr,
				"ERROR 00023 - Syntax error in ST0R statement.\n");
			sprintf(zwi,
				"ERROR 00023 - Syntax error in ST0R statement.");
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		bytes[bytecnt++] = CMD_ST0R;
		if (zwi2[0] == '"') {	// data
			bytes[bytecnt++] = 0;	// it's a string constant
			for (t = 1;; t++) {
				if (zwi2[t] == '"')
					break;
				if (zwi2[t] == 0)
					break;
				bytes[bytecnt++] = zwi2[t];
			}
			bytes[bytecnt++] = 0;
		}
		else {
			a = get_variable_index(zwi2, 1);
			if (a == -1) {
				fprintf(stderr,
					"ERROR 00006 -4- - Unknown variable >%s<.\n",
					zwi2);
				sprintf(zwi,
					"ERROR 00006 -4- - Unknown variable >%s<.",
					zwi2);
				write_line_to_list(zwi);
				errorcnt++;
				return (0);
			}
			bytes[bytecnt++] = 1;	// it's a string variable
			value = a;	// index of var1, data
			bytes[bytecnt++] = value & 255;
			value = value / 256;
			bytes[bytecnt++] = value & 255;
		}
		if (zwi4[0] == '"') {	// key
			bytes[bytecnt++] = 0;	// it's a string constant
			for (t = 1;; t++) {
				if (zwi4[t] == '"')
					break;
				if (zwi4[t] == 0)
					break;
				bytes[bytecnt++] = zwi4[t];
			}
			bytes[bytecnt++] = 0;
		}
		else {
			a = get_variable_index(zwi4, 1);
			if (a == -1) {
				fprintf(stderr,
					"ERROR 00006 -5- - Unknown variable >%s<.\n",
					zwi2);
				sprintf(zwi,
					"ERROR 00006 -5- - Unknown variable >%s<.",
					zwi2);
				write_line_to_list(zwi);
				errorcnt++;
				return (0);
			}
			bytes[bytecnt++] = 1;	// it's a string variable
			value = a;	// index of var2, key
			bytes[bytecnt++] = value & 255;
			value = value / 256;
			bytes[bytecnt++] = value & 255;
		}
		if (zwi6[0] == '"') {	// category
			bytes[bytecnt++] = 0;	// it's a string constant
			for (t = 1;; t++) {
				if (zwi6[t] == '"')
					break;
				if (zwi6[t] == 0)
					break;
				bytes[bytecnt++] = zwi6[t];
			}
			bytes[bytecnt++] = 0;
		}
		else {
			a = get_variable_index(zwi6, 1);
			if (a == -1) {
				fprintf(stderr,
					"ERROR 00006 -6- - Unknown variable >%s<.\n",
					zwi6);
				sprintf(zwi,
					"ERROR 00006 -6- - Unknown variable >%s<.",
					zwi6);
				write_line_to_list(zwi);
				errorcnt++;
				return (0);
			}
			bytes[bytecnt++] = 1;	// it's a string variable
			value = a;	// index of var3, category
			bytes[bytecnt++] = value & 255;
			value = value / 256;
			bytes[bytecnt++] = value & 255;
		}
		return (CMD_ST0R);
	}

	if (strcmp(zwi, "EAT") == 0) {
		word(s, zwi2, 2);	// MA
		word(s, zwi3, 3);	// system var
		ucase(zwi2);
		if (strcmp(zwi2, "MA") != 0) {
			fprintf(stderr,
				"ERROR 00026 - Syntax error in EAT statement.\n");
			sprintf(zwi,
				"ERROR 00026 - Syntax error in EAT statement.");
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		ucase(zwi3);
		f = 0;
		if (strcmp(zwi3, "ERRORCODE") == 0) {
			bytes[bytecnt++] = CMD_EAT_MA;
			bytes[bytecnt++] = 0;	// 0 = ERRORCODE
			f = 1;
		}
		if (f == 0) {
			fprintf(stderr,
				"ERROR 00026 - Syntax error in EAT statement.\n");
			sprintf(zwi,
				"ERROR 00026 - Syntax error in EAT statement.");
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		return (CMD_EAT_MA);
	}
	if (strcmp(zwi, "IMP") == 0) {
		word(s, zwi2, 2);	// var1
		word(s, zwi3, 3);	// AS
		word(s, zwi4, 4);	// var2
		ucase(zwi3);
		if (strcmp(zwi3, "AS") != 0) {
			fprintf(stderr,
				"ERROR 00025 - Syntax error in IMP statement.\n");
			sprintf(zwi,
				"ERROR 00025 - Syntax error in IMP statement.");
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		bytes[bytecnt++] = CMD_IMP;
		if (zwi2[0] == '"') {	// file name
			bytes[bytecnt++] = 0;	// it's a string constant
			for (t = 1;; t++) {
				if (zwi2[t] == '"')
					break;
				if (zwi2[t] == 0)
					break;
				bytes[bytecnt++] = zwi2[t];
			}
			bytes[bytecnt++] = 0;
		}
		else {
			a = get_variable_index(zwi2, 1);
			if (a == -1) {
				fprintf(stderr,
					"ERROR 00006 -7- - Unknown variable >%s<.\n",
					zwi2);
				sprintf(zwi,
					"ERROR 00006 -7- - Unknown variable >%s<.",
					zwi2);
				write_line_to_list(zwi);
				errorcnt++;
				return (0);
			}
			bytes[bytecnt++] = 1;	// it's a string variable
			value = a;	// index of var1, data
			bytes[bytecnt++] = value & 255;
			value = value / 256;
			bytes[bytecnt++] = value & 255;
		}
		if (zwi4[0] == '"') {	// category
			bytes[bytecnt++] = 0;	// it's a string constant
			for (t = 1;; t++) {
				if (zwi4[t] == '"')
					break;
				if (zwi4[t] == 0)
					break;
				bytes[bytecnt++] = zwi4[t];
			}
			bytes[bytecnt++] = 0;
		}
		else {
			a = get_variable_index(zwi4, 1);
			if (a == -1) {
				fprintf(stderr,
					"ERROR 00006 -8- - Unknown variable >%s<.\n",
					zwi2);
				sprintf(zwi,
					"ERROR 00006 -8- - Unknown variable >%s<.",
					zwi2);
				write_line_to_list(zwi);
				errorcnt++;
				return (0);
			}
			bytes[bytecnt++] = 1;	// it's a string variable
			value = a;	// index of var2, key
			bytes[bytecnt++] = value & 255;
			value = value / 256;
			bytes[bytecnt++] = value & 255;
		}
		return (CMD_IMP);
	}

	if (strcmp(zwi, "GIEF") == 0) {
		word(s, zwi2, 2);	// var1
		word(s, zwi3, 3);	// WITH
		word(s, zwi4, 4);	// var2
		word(s, zwi5, 5);	// FROM
		word(s, zwi6, 6);	// var3
		ucase(zwi3);
		if (strcmp(zwi3, "WITH") != 0) {
			fprintf(stderr,
				"ERROR 00024 - Syntax error in GIEF statement.\n");
			sprintf(zwi,
				"ERROR 00024 - Syntax error in GIEF statement.");
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		ucase(zwi5);
		if (strcmp(zwi5, "FROM") != 0) {
			fprintf(stderr,
				"ERROR 00024 - Syntax error in GIEF statement.\n");
			sprintf(zwi,
				"ERROR 00024 - Syntax error in GIEF statement.");
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		a = get_variable_index(zwi2, 1);	// data
		if (a == -1) {
			fprintf(stderr,
				"ERROR 00006 -9- - Unknown variable >%s<.\n",
				zwi2);
			sprintf(zwi, "ERROR 00006 -9- - Unknown variable >%s<.",
				zwi2);
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		bytes[bytecnt++] = CMD_GIEF;
		value = a;	// index of var1, data
		bytes[bytecnt++] = value & 255;
		value = value / 256;
		bytes[bytecnt++] = value & 255;

		if (zwi4[0] == '"') {	// key
			bytes[bytecnt++] = 0;	// it's a string constant
			for (t = 1;; t++) {
				if (zwi4[t] == '"')
					break;
				if (zwi4[t] == 0)
					break;
				bytes[bytecnt++] = zwi4[t];
			}
			bytes[bytecnt++] = 0;
		}
		else {
			a = get_variable_index(zwi4, 1);
			if (a == -1) {
				fprintf(stderr,
					"ERROR 00006 -10-- Unknown variable >%s<.\n",
					zwi4);
				sprintf(zwi,
					"ERROR 00006 -10-- Unknown variable >%s<.",
					zwi4);
				write_line_to_list(zwi);
				errorcnt++;
				return (0);
			}
			bytes[bytecnt++] = 1;	// it's a string variable
			value = a;	// index of var2, key
			bytes[bytecnt++] = value & 255;
			value = value / 256;
			bytes[bytecnt++] = value & 255;
		}

		if (zwi6[0] == '"') {	// category
			bytes[bytecnt++] = 0;	// it's a string constant
			for (t = 1;; t++) {
				if (zwi6[t] == '"')
					break;
				if (zwi6[t] == 0)
					break;
				bytes[bytecnt++] = zwi6[t];
			}
			bytes[bytecnt++] = 0;
		}
		else {
			a = get_variable_index(zwi6, 1);
			if (a == -1) {
				fprintf(stderr,
					"ERROR 00006 -11- - Unknown variable >%s<.\n",
					zwi6);
				sprintf(zwi,
					"ERROR 00006 -11- - Unknown variable >%s<.",
					zwi6);
				write_line_to_list(zwi);
				errorcnt++;
				return (0);
			}
			bytes[bytecnt++] = 1;	// it's a string variable
			value = a;	// index of var3, category
			bytes[bytecnt++] = value & 255;
			value = value / 256;
			bytes[bytecnt++] = value & 255;
		}
		return (CMD_GIEF);
	}

	if (strcmp(zwi, "GIMMEH") == 0) {
		word(s, zwi2, 2);	// variable 1
		a = get_variable_index(zwi2, 1);
		if (a == -1) {
			fprintf(stderr,
				"ERROR 00006 -12- - Unknown variable >%s<.\n",
				zwi2);
			sprintf(zwi,
				"ERROR 00006 -12- - Unknown variable >%s<.",
				zwi2);
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		// a += oldvarptr;      // automatic memory!! not at compile time!!!!
		if (vartype[a] != '$') {
			fprintf(stderr,
				"ERROR 00022 - Vartype mismatch in GIMMEH statement.\n");
			sprintf(zwi,
				"ERROR 00022 - Vartype mismatch in GIMMEH statement.");
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		bytes[bytecnt++] = CMD_GIMMEH;
		value = a;	// index of variable
		bytes[bytecnt++] = value & 255;
		value = value / 256;
		bytes[bytecnt++] = value & 255;
		value = 0;	// The file descriptor, 0 = stdin
		bytes[bytecnt++] = value & 255;
		value = value / 256;
		bytes[bytecnt++] = value & 255;
		return (CMD_GIMMEH);
	}

	if (strcmp(zwi, "LOL") == 0) {
		word(s, zwi2, 2);	// variable 1
		word(s, zwi3, 3);	// R
		ucase(zwi3);
		word(s, zwi4, 4);	// variable/value
		word(s, zwi5, 5);
		strcpy(ozwi4, zwi4);
		ucase(zwi4);
		ucase(zwi5);
		a = get_variable_index(zwi2, 1);
		//printf("zwi2=>%s< a=%d\n", zwi2, a);
		if (a == -1) {
			fprintf(stderr,
				"ERROR 00006 -13- - Unknown variable >%s<.\n",
				zwi2);
			sprintf(zwi,
				"ERROR 00006 -13- - Unknown variable >%s<.",
				zwi2);
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		//a += oldvarptr;       // automatic memory!! not at compile time !!!!
		f = 0;
		if (f == 0 && strcmp(zwi4, "$CAT") == 0) {
			if (vartype[a] != '$') {
				fprintf(stderr,
					"ERROR 00015 -4- Vartype mismatch in LOL statement.\n");
				sprintf(zwi,
					"ERROR 00015 -4- Vartype mismatch in LOL statement.");
				write_line_to_list(zwi);
				errorcnt++;
				return (0);
			}
			if (zwi5[0] == '"') {
				cmd = CMD_LOL_STRCAT;
				f = 1;
			}
			else {
				b = get_variable_index(zwi5, 1);
				if (b == -1) {
					fprintf(stderr,
						"ERROR 00006 -14- - Unknown variable >%s<.\n",
						zwi2);
					sprintf(zwi,
						"ERROR 00006 -14- - Unknown variable >%s<.",
						zwi2);
					write_line_to_list(zwi);
					errorcnt++;
					return (0);
				}
				if (vartype[b] != '$') {
					fprintf(stderr,
						"ERROR 00015 -5- Vartype mismatch in LOL statement.\n");
					sprintf(zwi,
						"ERROR 00015 -5- Vartype mismatch in LOL statement.");
					write_line_to_list(zwi);
					errorcnt++;
					return (0);
				}
				cmd = CMD_LOL_STRCAT;
				f = 1;
			}
		}
		if (f == 0 && strcmp(zwi4, "%2$") == 0) {
			if (vartype[a] != '$') {
				fprintf(stderr,
					"ERROR 00015 -6- Vartype mismatch in LOL statement.\n");
				sprintf(zwi,
					"ERROR 00015 -6- Vartype mismatch in LOL statement.");
				write_line_to_list(zwi);
				errorcnt++;
				return (0);
			}
			if (isdigit(zwi5[0]) != 0) {
				cmd = CMD_LOL_INT2STR;
				f = 1;
			}
			else {
				b = get_variable_index(zwi5, 1);
				printf("zwi5=>%s< b=%d\n", zwi5, b);
				if (b == -1) {
					fprintf(stderr,
						"ERROR 00006 -15- - Unknown variable >%s<.\n",
						zwi5);
					sprintf(zwi,
						"ERROR 00006 -15- - Unknown variable >%s<.",
						zwi5);
					write_line_to_list(zwi);
					errorcnt++;
					return (0);
				}
				if (vartype[b] != '%') {
					fprintf(stderr,
						"ERROR 00015 -7- Vartype mismatch in LOL statement.\n");
					sprintf(zwi,
						"ERROR 00015 -7- Vartype mismatch in LOL statement.");
					write_line_to_list(zwi);
					errorcnt++;
					return (0);
				}
				cmd = CMD_LOL_INT2STR;
				f = 1;
			}
		}
		if (f == 0 && strcmp(zwi4, "$2%") == 0) {
			if (vartype[a] != '%') {
				fprintf(stderr,
					"ERROR 00015 -8- Vartype mismatch in LOL statement.\n");
				sprintf(zwi,
					"ERROR 00015 -8- Vartype mismatch in LOL statement.");
				write_line_to_list(zwi);
				errorcnt++;
				return (0);
			}
			if (isdigit(zwi5[0]) == 0) {
				cmd = CMD_LOL_STR2INT;
				f = 1;
			}
			else {
				b = get_variable_index(zwi5, 1);
				//printf("zwi5=>%s< b=%d\n", zwi5, b);
				if (b == -1) {
					fprintf(stderr,
						"ERROR 00006 -15- - Unknown variable >%s<.\n",
						zwi5);
					sprintf(zwi,
						"ERROR 00006 -15- - Unknown variable >%s<.",
						zwi5);
					write_line_to_list(zwi);
					errorcnt++;
					return (0);
				}
				if (vartype[b] != '$') {
					fprintf(stderr,
						"ERROR 00015 -9- Vartype mismatch in LOL statement.\n");
					sprintf(zwi,
						"ERROR 00015 -9- Vartype mismatch in LOL statement.");
					write_line_to_list(zwi);
					errorcnt++;
					return (0);
				}
				cmd = CMD_LOL_STR2INT;
				f = 1;
			}
		}
		if (f == 0 && strcmp(zwi4, "ERRORCODE") == 0) {
			if (vartype[a] != '%') {
				fprintf(stderr,
					"ERROR 00015 -10- Vartype mismatch in LOL statement.\n");
				sprintf(zwi,
					"ERROR 00015 -10- Vartype mismatch in LOL statement.");
				write_line_to_list(zwi);
				errorcnt++;
				return (0);
			}
			cmd = CMD_LOL_ERRORCODE;
			f = 1;
		}
		if (f == 0 && strcmp(zwi4, "RND") == 0) {
			if (vartype[a] != '%') {
				fprintf(stderr,
					"ERROR 00015 -11- Vartype mismatch in LOL statement.\n");
				sprintf(zwi,
					"ERROR 00015 -11- Vartype mismatch in LOL statement.");
				write_line_to_list(zwi);
				errorcnt++;
				return (0);
			}
			cmd = CMD_LOL_NUM_RND;
			f = 1;
		}
		if (f == 0 && strcmp(zwi4, "$LEN") == 0) {
			if (vartype[a] != '%') {
				fprintf(stderr,
					"ERROR 00015 -12- Vartype mismatch in LOL statement.\n");
				sprintf(zwi,
					"ERROR 00015 -12- Vartype mismatch in LOL statement.");
				write_line_to_list(zwi);
				errorcnt++;
				return (0);
			}
			b = get_variable_index(zwi5, 1);
			if (b == -1) {
				if (zwi5[0] != '"') {	// no variable and no string => error
					fprintf(stderr,
						"ERROR 00015 -13- Vartype mismatch in LOL statement.\n");
					sprintf(zwi,
						"ERROR 00015 -13- Vartype mismatch in LOL statement.");
					write_line_to_list(zwi);
					errorcnt++;
					return (0);
				}
			}
			else {
				if (vartype[b] != '$') {
					fprintf(stderr,
						"ERROR 00015 -14- Vartype mismatch in LOL statement.\n");
					sprintf(zwi,
						"ERROR 00015 -14- Vartype mismatch in LOL statement.");
					write_line_to_list(zwi);
					errorcnt++;
					return (0);
				}
			}
			cmd = CMD_LOL_STRLEN;
			f = 1;
		}
		if (f == 0 && strcmp(zwi4, "PRON") == 0) {
			if (vartype[a] != '%') {
				fprintf(stderr,
					"ERROR 00015 -15- Vartype mismatch in LOL statement.\n");
				sprintf(zwi,
					"ERROR 00015 -15- Vartype mismatch in LOL statement.");
				write_line_to_list(zwi);
				errorcnt++;
				return (0);
			}
			cmd = CMD_LOL_NUM_PRON;
			for (t = 5;; t++) {
				word(s, zwi, t);
				if (strlen(zwi) == 0)
					break;
				if (strcmp(zwi, "BTW") == 0)
					break;
				write_pron_word(zwi);
			}
			f = 1;
		}
		if (f == 0) {
			word(s, zwi5, 5);
			if (strcmp(zwi5, "UP") == 0 ||	// simple arithmetich,
			    // +-*/
			    strcmp(zwi5, "NERF") == 0 ||
			    strcmp(zwi5, "TIEMZ") == 0
			    || strcmp(zwi5, "OVAR") == 0) {
				if (strcmp(zwi3, "R") != 0) {
					fprintf(stderr,
						"ERROR 00016 -1- - Syntax error in LOL statement.\n");
					sprintf(zwi,
						"ERROR 00016 -1- - Syntax error in LOL statement.");
					write_line_to_list(zwi);
					errorcnt++;
					return (0);
				}
				b = get_variable_index(zwi4, 1);
				if (b == -1) {
					fprintf(stderr,
						"ERROR 00006 -16- - Unknown variable >%s<.\n",
						zwi4);
					sprintf(zwi,
						"ERROR 00006 -16- - Unknown variable >%s<.",
						zwi4);
					write_line_to_list(zwi);
					errorcnt++;
					return (0);
				}
				word(s, zwi6, 6);
				c = get_variable_index(zwi6, 1);
				if (b == -1) {
					fprintf(stderr,
						"ERROR 00006 -17- - Unknown variable >%s<.\n",
						zwi6);
					sprintf(zwi,
						"ERROR 00006 -17- - Unknown variable >%s<.",
						zwi6);
					write_line_to_list(zwi);
					errorcnt++;
					return (0);
				}
				// only numerical variables are allowed here!
				if (vartype[a] != '%') {
					fprintf(stderr,
						"ERROR 00015 -16- Vartype mismatch in LOL statement.\n");
					sprintf(zwi,
						"ERROR 00015 -16- Vartype mismatch in LOL statement.");
					write_line_to_list(zwi);
					errorcnt++;
					return (0);
				}
				if (vartype[b] != '%') {
					fprintf(stderr,
						"ERROR 00015 -17- Vartype mismatch in LOL statement.\n");
					sprintf(zwi,
						"ERROR 00015 -17- Vartype mismatch in LOL statement.");
					write_line_to_list(zwi);
					errorcnt++;
					return (0);
				}
				if (vartype[c] != '%') {
					fprintf(stderr,
						"ERROR 00015 -18- Vartype mismatch in LOL statement.\n");
					sprintf(zwi,
						"ERROR 00015 -18- Vartype mismatch in LOL statement.");
					write_line_to_list(zwi);
					errorcnt++;
					return (0);
				}
				if (strcmp(zwi5, "UP") == 0) {
					cmd = CMD_ADD_VARIABLES;
				}
				if (strcmp(zwi5, "NERF") == 0) {
					cmd = CMD_SUB_VARIABLES;
				}
				if (strcmp(zwi5, "TIEMZ") == 0) {
					cmd = CMD_MUL_VARIABLES;
				}
				if (strcmp(zwi5, "OVAR") == 0) {
					cmd = CMD_DIV_VARIABLES;
				}
				bytes[bytecnt++] = cmd;
				value = a;	// index of variable
				bytes[bytecnt++] = value & 255;
				value = value / 256;
				bytes[bytecnt++] = value & 255;
				value = value / 256;
				value = b;	// index of 2nd variable
				bytes[bytecnt++] = value & 255;
				value = value / 256;
				bytes[bytecnt++] = value & 255;
				value = c;	// index of 3rd variable
				bytes[bytecnt++] = value & 255;
				value = value / 256;
				bytes[bytecnt++] = value & 255;
				return (cmd);
			}
			else {
				if (strlen(zwi5) > 0) {
					// printf("zwi2=>%s<\n", zwi2);
					// printf("zwi3=>%s<\n", zwi3);
					// printf("zwi4=>%s<\n", zwi4);
					// printf("zwi5=>%s<\n", zwi5);
					fprintf(stderr,
						"ERROR 00016 -2- - Syntax error in LOL statement.\n");
					sprintf(zwi,
						"ERROR 00016 -2- - Syntax error in LOL statement.");
					write_line_to_list(zwi);
					errorcnt++;
					return (0);
				}
				if (is_number(zwi4) == TRUE) {
					if (vartype[a] != '%') {
						printf("a=%d\n", a);
						fprintf(stderr,
							"ERROR 00015 -19- Vartype mismatch in LOL statement.\n");
						sprintf(zwi,
							"ERROR 00015 -19- Vartype mismatch in LOL statement.");
						write_line_to_list(zwi);
						errorcnt++;
						return (0);
					}
					cmd = CMD_LOL_NUM;
				}
				else {
					if (zwi4[0] == '"') {
						if (vartype[a] != '$') {
							fprintf(stderr,
								"ERROR 00015 -20- Vartype mismatch in LOL statement.\n");
							sprintf(zwi,
								"ERROR 00015 -20- Vartype mismatch in LOL statement.");
							write_line_to_list(zwi);
							errorcnt++;
							return (0);
						}
						cmd = CMD_LOL_STR;
					}
					else {
						b = get_variable_index(zwi4, 1);
						if (b == -1) {
							fprintf(stderr,
								"ERROR 00006 -18- - Unknown variable >%s<.\n",
								zwi4);
							sprintf(zwi,
								"ERROR 00006 -18- - Unknown variable >%s<.",
								zwi4);
							write_line_to_list(zwi);
							errorcnt++;
							return (0);
						}
						//b += oldvarptr;       // automatic memory!! not at compile time !!!!
						if (vartype[a] != vartype[b]) {
							fprintf(stderr,
								"ERROR 00015 -20- Vartype mismatch in LOL statement.\n");
							sprintf(zwi,
								"ERROR 00015 -20- Vartype mismatch in LOL statement.");
							write_line_to_list(zwi);
							errorcnt++;
							return (0);
						}
						if (vartype[a] == '%') {
							cmd = CMD_LOL_NUM_VAR;
						}
						else {
							cmd = CMD_LOL_STR_VAR;
						}
					}
				}
			}
			f = 1;
		}
		if (f == 0) {
			fprintf(stderr,
				"ERROR 00016 -3- - Syntax error in LOL statement.\n");
			sprintf(zwi,
				"ERROR 00016 -3- - Syntax error in LOL statement.");
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		if (strcmp(zwi3, "R") != 0) {
			fprintf(stderr,
				"ERROR 00016 -4- - Syntax error in LOL statement.\n");
			sprintf(zwi,
				"ERROR 00016 -4- - Syntax error in LOL statement.");
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		bytes[bytecnt++] = cmd;
		if (cmd == CMD_LOL_ERRORCODE) {
			value = a;	// index of variable
			bytes[bytecnt++] = value & 255;
			value = value / 256;
			bytes[bytecnt++] = value & 255;
		}
		if (cmd == CMD_LOL_STRCAT) {
			value = a;	// index of variable
			bytes[bytecnt++] = value & 255;
			value = value / 256;
			bytes[bytecnt++] = value & 255;
			b = get_variable_index(zwi5, 1);
			if (b == -1) {	// its a string
				bytes[bytecnt++] = 0;
				for (t = 1;; t++) {
					if (zwi5[t] == '"')
						break;
					if (zwi5[t] == 0)
						break;
					bytes[bytecnt++] = zwi5[t];
				}
				bytes[bytecnt++] = 0;
			}
			else {	// its a string variable
				bytes[bytecnt++] = 1;
				value = b;	// index of variable
				bytes[bytecnt++] = value & 255;
				value = value / 256;
				bytes[bytecnt++] = value & 255;
			}
		}
		if (cmd == CMD_LOL_INT2STR) {
			value = a;	// index of variable
			bytes[bytecnt++] = value & 255;
			value = value / 256;
			bytes[bytecnt++] = value & 255;
			b = get_variable_index(zwi5, 1);
			if (b == -1) {	// its a number
				bytes[bytecnt++] = 0;
				value = atoi(zwi5);	// content of string
				write_signed_number(value);
			}
			else {	// its an integer variable
				bytes[bytecnt++] = 1;
				value = b;	// index of variable
				bytes[bytecnt++] = value & 255;
				value = value / 256;
				bytes[bytecnt++] = value & 255;
			}
		}
		if (cmd == CMD_LOL_STR2INT) {
			value = a;	// index of variable
			bytes[bytecnt++] = value & 255;
			value = value / 256;
			bytes[bytecnt++] = value & 255;
			b = get_variable_index(zwi5, 1);
			if (b == -1) {	// its a STRING
				bytes[bytecnt++] = 0;
				for (t = 1;; t++) {
					if (zwi5[t] == '"') {
						bytes[bytecnt++] = 0;
						break;
					}
					bytes[bytecnt++] = zwi5[t];
					if (zwi5[t] == 0)
						break;
				}
			}
			else {	// its a string variable
				bytes[bytecnt++] = 1;
				value = b;	// index of variable
				bytes[bytecnt++] = value & 255;
				value = value / 256;
				bytes[bytecnt++] = value & 255;
			}
		}
		if (cmd == CMD_LOL_STRLEN) {
			value = a;	// index of variable
			bytes[bytecnt++] = value & 255;
			value = value / 256;
			bytes[bytecnt++] = value & 255;
			b = get_variable_index(zwi5, 1);
			if (b == -1) {	// its a STRING
				bytes[bytecnt++] = 0;
				for (t = 1;; t++) {
					if (zwi5[t] == '"') {
						bytes[bytecnt++] = 0;
						break;
					}
					bytes[bytecnt++] = zwi5[t];
					if (zwi5[t] == 0)
						break;
				}
			}
			else {	// its a string variable
				bytes[bytecnt++] = 1;
				value = b;	// index of variable
				bytes[bytecnt++] = value & 255;
				value = value / 256;
				bytes[bytecnt++] = value & 255;
			}
		}
		if (cmd == CMD_LOL_NUM_RND) {
			value = a;	// index of variable
			bytes[bytecnt++] = value & 255;
			value = value / 256;
			bytes[bytecnt++] = value & 255;
			if (strlen(zwi5) == 0) {	// the limit for rnd()
				a = 0;
			}
			else {
				a = atoi(zwi5);
			}
			value = a;
			bytes[bytecnt++] = value & 255;
			value = value / 256;
			bytes[bytecnt++] = value & 255;
		}
		if (cmd == CMD_LOL_NUM) {
			value = a;	// index of variable
			bytes[bytecnt++] = value & 255;
			value = value / 256;
			bytes[bytecnt++] = value & 255;
			value = value / 256;
			value = atoi(zwi4);	// the value to assign to variable
			bytes[bytecnt++] = value & 255;
			value = value / 256;
			bytes[bytecnt++] = value & 255;
			value = value / 256;
			bytes[bytecnt++] = value & 255;
			value = value / 256;
			bytes[bytecnt++] = value & 255;
		}
		if (cmd == CMD_LOL_NUM_PRON) {
			value = a;	// index of variable
			bytes[bytecnt++] = value & 255;
			value = value / 256;
			bytes[bytecnt++] = value & 255;
			value = value / 256;
		}
		if (cmd == CMD_LOL_NUM_VAR || cmd == CMD_LOL_STR_VAR) {
			value = a;	// index of variable
			bytes[bytecnt++] = value & 255;
			value = value / 256;
			bytes[bytecnt++] = value & 255;
			value = value / 256;
			value = b;	// index of 2nd variable
			bytes[bytecnt++] = value & 255;
			value = value / 256;
			bytes[bytecnt++] = value & 255;
		}
		if (cmd == CMD_LOL_STR) {
			value = a;	// index of variable
			bytes[bytecnt++] = value & 255;
			value = value / 256;
			bytes[bytecnt++] = value & 255;
			value = value / 256;
			for (t = 1;; t++) {
				c = ozwi4[t];	// use the original string, not the upper
				// case one
				if (c == 0 || c == '"') {
					bytes[bytecnt++] = 0;
					break;
				}
				bytes[bytecnt++] = c;
			}
		}
		return (cmd);
	}

	if (strcmp(zwi, "IZ") == 0) {
		cmd = parse_iz_statement(s);
		return (cmd);
	}

	if (strcmp(zwi, "UP") == 0) {
		word(s, zwi2, 2);
		bytes[bytecnt++] = CMD_UP;
		a = get_variable_index(zwi2, 1);
		if (a == -1) {
			fprintf(stderr,
				"ERROR 00006 -19 - Unknown variable >%s<.\n",
				zwi2);
			sprintf(zwi, "ERROR 00006 -19 - Unknown variable >%s<.",
				zwi2);
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		// a += oldvarptr;      // automatic memory!! not at compile time !!!!
		// printf("vartype[%d]=%d\n", a, vartype[a]);
		if (vartype[a] == '%') {
		}
		else {
			fprintf(stderr,
				"ERROR 00012 - Invalid operation on non numeric variable >%s<.\n",
				zwi2);
			sprintf(zwi,
				"ERROR 00012 - Invalid operation on non numeric variable >%s<.",
				zwi2);
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		bytes[bytecnt++] = a & 255;
		a = a / 256;
		bytes[bytecnt++] = a & 255;
		return (CMD_UP);
	}

	if (strcmp(zwi, "DOWN") == 0) {
		word(s, zwi2, 2);
		bytes[bytecnt++] = CMD_DOWN;
		a = get_variable_index(zwi2, 1);
		if (a == -1) {
			fprintf(stderr,
				"ERROR 00006 -20- - Unknown variable >%s<.\n",
				zwi2);
			sprintf(zwi,
				"ERROR 00006 -20- - Unknown variable >%s<.",
				zwi2);
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		// a += oldvarptr;      // automatic memory!! not at compile time !!!
		// printf("vartype[%d]=%d\n", a, vartype[a]);
		if (vartype[a] == '%') {
		}
		else {
			fprintf(stderr,
				"ERROR 00012 - Invalid operation on non numeric variable >%s<.\n",
				zwi2);
			sprintf(zwi,
				"ERROR 00012 - Invalid operation on non numeric variable >%s<.",
				zwi2);
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		bytes[bytecnt++] = a & 255;
		a = a / 256;
		bytes[bytecnt++] = a & 255;
		return (CMD_DOWN);
	}

	if (strcmp(zwi, "WTF") == 0) {
		word(s, zwi2, 2);
		if (zwi2[strlen(zwi2) - 1] == '?') {
			zwi2[strlen(zwi2) - 1] = 0;
		}
		bytes[bytecnt++] = CMD_WTF;
		a = get_variable_index(zwi2, 1);
		if (a == -1) {
			fprintf(stderr,
				"ERROR 00006 -20- - Unknown variable >%s<.\n",
				zwi2);
			sprintf(zwi,
				"ERROR 00006 -20- - Unknown variable >%s<.",
				zwi2);
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		bytes[bytecnt++] = a & 255;
		a = a / 256;
		bytes[bytecnt++] = a & 255;
		wtf_variable = a;
		return (CMD_WTF);
	}

	if (strcmp(zwi, "OIC") == 0) {
		bytes[bytecnt++] = CMD_OIC;
		if (wtf_variable == -1) {
			fprintf(stderr,
				"ERROR 00028 -1- OIC without WTF statement.\n");
			sprintf(zwi,
				"ERROR 00028 -1- OIC without WTF statement.");
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		wtf_variable = -1;
		return (CMD_OIC);
	}

	if (strcmp(zwi, "OMG") == 0) {
		if (wtf_variable == -1) {
			fprintf(stderr,
				"ERROR 00029 -1- OMG without WTF statement.\n");
			sprintf(zwi,
				"ERROR 00029 -1- OMG without WTF statement.");
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		bytes[bytecnt++] = CMD_OMG;
		word(s, zwi2, 2);
		return (CMD_OIC);
	}

	if (strcmp(zwi, "ENUF") == 0) {
		bytes[bytecnt++] = CMD_GOTO;
		reltab[relcnt] = bytecnt;	// relocate it after the LOOP
		relloop[relcnt] = malloc(strlen(loopname[loopcnt - 1]) + 1);
		strcpy(relloop[relcnt], loopname[loopcnt - 1]);
		relcnt++;
		bytes[bytecnt++] = 0;
		bytes[bytecnt++] = 0;
		bytes[bytecnt++] = 0;
		bytes[bytecnt++] = 0;
		return (CMD_GOTO);
	}
	if (strcmp(zwi, "CAN") == 0) {
		word(s, zwi2, 2);
		// printf("zwi2=>%s<\n", zwi2);
		if (strcmp(zwi2, "HAS") == 0) {
			bytes[bytecnt++] = CMD_CAN_HAS;
			return (CMD_CAN_HAS);
		}
		else {
			if (strcmp(zwi2, "HAZ") == 0) {
				do_copybook(s);
				// bytes[bytecnt++]=3; // its just a copy book...
				return (CMD_CAN_HAZ);
			}
			else {
				fprintf(stderr,
					"ERROR 00002 -1- Unknown statement >%s<.\n",
					s);
				sprintf(zwi,
					"ERROR 00002 -1- Unknown statement >%s<.",
					s);
				write_line_to_list(zwi);
				errorcnt++;
				return (0);
			}
		}
	}

	if (strcmp(zwi, "VISIBLE") == 0 || strcmp(zwi, "VISIBLE!") == 0) {
		if (strcmp(zwi, "VISIBLE!") == 0) {
			crlf = 1;
		}
		else {
			crlf = 0;
		}
		word(s, zwi2, 2);
		// printf("CMD_VISIBLE: zwi2=>%s< bytecnt=%d\n", zwi2, bytecnt);
		if (zwi2[0] == '"') {
			bytes[bytecnt++] = CMD_VISIBLE_TEXT;
			bytes[bytecnt++] = crlf;
			for (t = 1;; t++) {
				if (zwi2[t] == '"')
					break;
				if (zwi2[t] == '\\') {
					t++;
				}
				bytes[bytecnt++] = zwi2[t];
			}
			bytes[bytecnt++] = 0;
			return (CMD_VISIBLE_TEXT);
		}
		else {
			bytes[bytecnt++] = CMD_VISIBLE_VAR;
			bytes[bytecnt++] = crlf;
			a = get_variable_index(zwi2, 1);
			// printf("visible: a=%d zwi2=>%s<\n", a, zwi2);
			if (a == -1) {
				fprintf(stderr,
					"ERROR 00006 -21- - Unknown variable >%s<.\n",
					zwi2);
				sprintf(zwi,
					"ERROR 00006 -21- - Unknown variable >%s<.",
					zwi2);
				write_line_to_list(zwi);
				errorcnt++;
				return (0);
			}
			else {
				// a += oldvarptr;      // automatic memory!! not at compile time!!!
				bytes[bytecnt++] = a & 255;
				a = a / 256;
				bytes[bytecnt++] = a & 255;
				return (CMD_VISIBLE_VAR);
			}
		}
	}

	if (strcmp(zwi, "KTHXBB") == 0) {
		bytes[bytecnt++] = CMD_KTHXBYE;
		// KTHXBB doesnt need to reset the var stack
		varcnt = gvarcnt;	// get rid of the local variables of the
		// HAI
		strcpy(actfunction, "");
		return (CMD_KTHXBYE);
	}
	if (strcmp(zwi, "KTHXBYE") == 0) {
		bytes[bytecnt++] = CMD_KTHXBYE;
		// KTHXBYE doesnt need to reset the var stack
		varcnt = gvarcnt;	// get rid of the local variables of the
		// HAI
		strcpy(actfunction, "");
		return (CMD_KTHXBYE);
	}
	if (strcmp(zwi, "BBHF") == 0) {
		bytes[bytecnt++] = CMD_RETURN;
		a = varcnt - gvarcnt;	// the offset the local variables use on
		// the varstack
		// printf("BBHF: offset =%d\n", a);
		varcnttab[actfunctionpos] = a;	// store here the offset for the
		// act function
		// printf("function>%s< %d gvarcnt=%d varcnt=%d\n", actfunction,
		// varcnt-gvarcnt, gvarcnt, varcnt);
		varcnt = gvarcnt;	// get rid of the local variables of the
		// LULZ
		strcpy(actfunction, "");
		return (CMD_RETURN);
	}

	if (s[0] == '@') {	// gosub
		word(s, zwi, 1);	// function name
		for (t = 0;; t++) {	// remove the leading @
			if (zwi[t] == 0)
				break;
			zwi[t] = zwi[t + 1];
		}
		ucase(zwi);
		f = 0;
		for (t = 0; t < functioncnt; t++) {
			if (strcmp(zwi, functiontab[t]) == 0) {
				f = 1;
				pos = t;
				break;
			}
		}
		if (f == 0) {
			fprintf(stderr, "ERROR 00020 - Unknown LULZ >%s<.\n",
				zwi);
			sprintf(zwi, "ERROR 00020 - Unknown LULZ >%s<.", zwi);
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		handle_function_parameter(s, pos);	// assign values to
		// parameters
		bytes[bytecnt++] = CMD_GOSUB;
		a = adresstab[t];
		// printf("gosub: a=%d t=%d\n", a, t);
		bytes[bytecnt++] = a & 255;
		a = a / 256;
		bytes[bytecnt++] = a & 255;
		a = a / 256;
		bytes[bytecnt++] = a & 255;
		a = a / 256;
		bytes[bytecnt++] = a & 255;
		a = varcnttab[pos];	// the offset for the auto varstack
		// printf("GOSUB: offset =%d\n", a);
		bytes[bytecnt++] = a & 255;
		a = a / 256;
		bytes[bytecnt++] = a & 255;
		// now do the CMD_TOVARSTACKINT etc. comands for all parameters
		return (CMD_GOSUB);
	}

	if (strcmp(zwi, "LULZ") == 0) {
		word(s, zwi2, 2);	// function name
		ucase(zwi2);
		strcpy(actfunction, zwi2);
		f = 0;
		for (t = 0; t < functioncnt; t++) {
			if (strcmp(zwi2, functiontab[t]) == 0) {
				f = 1;
				break;
			}
		}
		if (f == 1) {
			fprintf(stderr, "ERROR 00018 - Duplicate LULZ >%s<.\n",
				zwi2);
			sprintf(zwi, "ERROR 00018 - Duplicate LULZ >%s<.",
				zwi2);
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		ucase(zwi2);
		strcpy(functiontab[functioncnt], zwi2);
		strcpy(zwi3, "");
		parmcnt = 0;
		for (t = 3;; t++) {
			word(s, zwi4, t);
			if (strlen(zwi4) == 0)
				break;
			strcat(zwi3, zwi4);
			strcat(zwi3, " ");
			parmcnt++;
		}
		trim(zwi3);
		if (strlen(zwi) > 127) {
			fprintf(stderr,
				"ERROR 00019 - Too many parameters >%s<.\n", s);
			sprintf(zwi, "ERROR 00019 - Too many parameters >%s<.",
				s);
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		gvarcnt = varcnt;	// to reset the varstack at the end of the 
		// LULZ
		strcpy(paramtab[functioncnt], zwi3);
		adresstab[functioncnt] = bytecnt;
		for (t = 3;; t++) {
			word(s, zwi4, t);
			if (strlen(zwi4) == 0)
				break;
			// parameter variable look like functionname.variablename
			sprintf(zwi3, "%s.%s", zwi2, zwi4);
			ucase(zwi3);
			// printf("add variable >%s<\n", zwi3);
			add_variable(zwi3);
		}
		actfunctionpos = functioncnt;	// the pos of the function in the
		// functiontab
		functioncnt++;
		bytes[bytecnt++] = CMD_RESERVE_PARAMS;
		bytes[bytecnt++] = parmcnt;
		return (0);
	}

	word(s, zwi2, 2);
	word(s, zwi3, 3);
	if (strcmp(zwi, "I") == 0 &&
	    strcmp(zwi2, "HAS") == 0 && strcmp(zwi3, "A") == 0) {
		word(s, zwi4, 4);
		vtype = zwi4[strlen(zwi4) - 1];
		// printf("vtype=>%c<\n", vtype);
		if (check_variable_name(zwi4) == FALSE) {
			fprintf(stderr,
				"ERROR 00004 - invalid variable name >%s<.\n",
				zwi4);
			sprintf(zwi,
				"ERROR 00004 - invalid variable name >%s<.",
				zwi4);
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		add_variable(zwi4);
		if (opt_dbg == TRUE) {	// for debug add now the variable name
			bytes[bytecnt++] = CMD_TOVARSTACKLOCNAME;
			for (t = 0;; t++) {
				if (zwi4[t] == 0 || zwi4[t] == '$'
				    || zwi4[t] == '%')
					break;
				bytes[bytecnt++] = zwi4[t];
			}
			bytes[bytecnt++] = 0;
		}
		bytes[bytecnt++] = CMD_TOVARSTACKLOCTYPE;
		bytes[bytecnt++] = vtype;

		return (CMD_I_HAS_A);
	}
	/*
	 * - - - - - - - - - - - - - - - - - - - - - - - - - 
	 */
	if (strcmp(zwi, "IM") == 0 &&
	    strcmp(zwi2, "IN") == 0 && strcmp(zwi3, "YR") == 0) {
		word(s, zwi4, 4);
		if (check_loop_name(zwi4) == FALSE) {
			fprintf(stderr,
				"ERROR 00008 - invalid loop name >%s<.\n",
				zwi4);
			sprintf(zwi, "ERROR 00008 - invalid loop name >%s<.",
				zwi4);
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		add_loop(zwi4, bytecnt);
		word(s, zwi5, 5);
		if (strlen(zwi5) == 0) {
			return (CMD_INYRLOOP_VAR_NUM);
		}
		ucase(zwi5);
		word(s, zwi6, 6);
		ucase(zwi6);
		// now additional loop commands
		f = 0;
		//printf("zwi5=>%s<, zwi6=>%s<\n", zwi5, zwi6);
		if (strcmp(zwi5, "UPPIN") == 0 && strcmp(zwi6, "YR") == 0) {
			f = 1;
			word(s, zwi7, 7);
			ucase(zwi7);
			bytes[bytecnt++] = CMD_UP;
			a = get_variable_index(zwi7, 1);
			//printf("variable =>%s<, a=%d\n", zwi7, a);
			if (a == -1) {
				fprintf(stderr,
					"ERROR 00006 -22- - Unknown variable >%s<.\n",
					zwi7);
				sprintf(zwi,
					"ERROR 00006 -22- - Unknown variable >%s<.",
					zwi7);
				write_line_to_list(zwi);
				errorcnt++;
				return (0);
			}
			if (vartype[a] != '%') {
				fprintf(stderr,
					"ERROR 00012 - Invalid operation on non numeric variable >%s<.\n",
					zwi2);
				sprintf(zwi,
					"ERROR 00012 - Invalid operation on non numeric variable >%s<.",
					zwi2);
				write_line_to_list(zwi);
				errorcnt++;
				return (0);
			}
			bytes[bytecnt++] = a & 255;
			a = a / 256;
			bytes[bytecnt++] = a & 255;
		}
		if (f == 0) {
			fprintf(stderr,
				"ERROR 00027 - Syntax error in IM IN YR statement.\n");
			sprintf(zwi,
				"ERROR 00027 - Syntax error in IM IN YR statement.");
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		f = 0;
		word(s, zwi8, 8);
		ucase(zwi8);
		word(s, zwi9, 9);
		ucase(zwi9);
		word(s, zwi10, 10);
		ucase(zwi10);
		word(s, zwi11, 11);
		ucase(zwi11);
		word(s, zwi12, 12);
		ucase(zwi12);
		word(s, zwi13, 13);
		ucase(zwi13);
		//printf("zwi8=>%s<, zwi9=>%s<\n", zwi8, zwi9);
		//printf("zwi10=>%s<, zwi11=>%s<\n", zwi10, zwi11);
		//printf("zwi12=>%s<, zwi13=>%s<\n", zwi12, zwi13);
		if (strcmp(zwi8, "TIL") == 0 &&
		    strcmp(zwi9, "BOTH") == 0 &&
		    strcmp(zwi10, "SAEM") == 0 && strcmp(zwi12, "AN") == 0) {
			f = 1;
			a = get_variable_index(zwi11, 1);
			b = get_variable_index(zwi13, 1);
			if (b == -1) {	// loop limit is a number
				bytes[bytecnt++] = CMD_INYRLOOP_VAR_NUM;	// command
			}
			else {
				bytes[bytecnt++] = CMD_INYRLOOP_VAR_VAR;	// command
			}
			c = a;	// first variable
			bytes[bytecnt++] = c & 255;
			c = c / 256;
			bytes[bytecnt++] = c & 255;
			bytes[bytecnt++] = 99;	// operator EQUALS
			if (b == -1) {
				c = atoi(zwi13);	// 2nd, the number
				write_signed_number(c);
			}
			else {
				c = b;	// 2nd, the limit variable
				bytes[bytecnt++] = c & 255;
				c = c / 256;
				bytes[bytecnt++] = c & 255;
			}
			reltab[relcnt] = bytecnt;	// relocate it after the LOOP
			relloop[relcnt] =
			    malloc(strlen(loopname[loopcnt - 1]) + 1);
			strcpy(relloop[relcnt], loopname[loopcnt - 1]);
			relcnt++;
			bytes[bytecnt++] = 0;
			bytes[bytecnt++] = 0;
			bytes[bytecnt++] = 0;
			bytes[bytecnt++] = 0;
		}
		if (f == 0) {
			fprintf(stderr,
				"ERROR 00027 - Syntax error in IM IN YR statement.\n");
			sprintf(zwi,
				"ERROR 00027 - Syntax error in IM IN YR statement.");
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		return (CMD_I_HAS_A);
	}
	/*
	 * - - - - - - - - - - - - - - - - - - - - - - - - - 
	 */
	if (strcmp(zwi, "IM") == 0 &&
	    strcmp(zwi2, "OUTTA") == 0 && strcmp(zwi3, "YR") == 0) {
		word(s, zwi4, 4);
		a = get_loop_pos(zwi4);
		//printf("outta yr loop >%s< %d\n", zwi4, a);
		if (a == -1) {
			fprintf(stderr,
				"ERROR 00010 - unknown loop name >%s<.\n",
				zwi4);
			sprintf(zwi, "ERROR 00010 - unknown loop name >%s<.",
				zwi4);
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		strcpy(zwi, zwi4);
		ucase(zwi);
		if (strcmp(loopname[loopcnt - 1], zwi) != 0) {
			fprintf(stderr,
				"ERROR 00011 - invalid nested loops >%s<.\n",
				zwi4);
			sprintf(zwi, "ERROR 00011 - invalid nested loops >%s<.",
				zwi4);
			write_line_to_list(zwi);
			errorcnt++;
			return (0);
		}
		bytes[bytecnt++] = CMD_GOTO;
		b = a;
		bytes[bytecnt++] = b & 255;
		a = a / 256;
		b = a;
		bytes[bytecnt++] = b & 255;
		a = a / 256;
		b = a;
		bytes[bytecnt++] = b & 255;
		a = a / 256;
		b = a;
		bytes[bytecnt++] = b & 255;
		loopcnt--;	// remove this loop from stack
		// now relocate the ENUF commands inside the loop
		//printf("relcnt=%d\n", relcnt);
		for (;;) {
			if (relcnt == 0)
				break;
			t = relcnt - 1;
			//printf("compare relloop[%d] >%s<->%s< loopname\n", t,
			//      relloop[t], loopname[loopcnt]);
			if (strcmp(relloop[t], loopname[loopcnt]) != 0) {
				break;	// not our loop anymore
			}
			a = bytecnt;
			bytes[reltab[t]] = a & 255;
			a = a / 256;
			bytes[reltab[t] + 1] = a & 255;
			a = a / 256;
			bytes[reltab[t] + 2] = a & 255;
			a = a / 256;
			bytes[reltab[t] + 3] = a & 255;
			free(relloop[t]);
			relloop[t] = NULL;
			relcnt--;
		}
		return (CMD_GOTO);
	}
	/*
	 * - - - - - - - - - - - - - - - - - - - - - - - - - 
	 */
	if (strcmp(zwi, "YARLY") == 0) {
		return (0);	// just ignore this statement
	}
	if (strcmp(zwi, "NOWAI") == 0) {
		// this GOTO ends the YARLY block!!!
		bytes[bytecnt++] = CMD_GOTO;
		izkthxreltab[izkthxrelcnt++] = bytecnt;	// relocate it after the
		// KTHX
		bytes[bytecnt++] = 0;
		bytes[bytecnt++] = 0;
		bytes[bytecnt++] = 0;
		bytes[bytecnt++] = 0;
		// now relocate the IZ command of the IZ block
		if (nowaiizlevel[nowairelcnt - 1] == izlevel) {
			nowairelcnt--;
			a = bytecnt;
			// printf("relocating to address %d\n", a);
			bytes[nowaireltab[nowairelcnt]] = a & 255;
			a = a / 256;
			bytes[nowaireltab[nowairelcnt] + 1] = a & 255;
			a = a / 256;
			bytes[nowaireltab[nowairelcnt] + 2] = a & 255;
			a = a / 256;
			bytes[nowaireltab[nowairelcnt] + 3] = a & 255;
		}

		return (CMD_GOTO);	// just ignore this statement
	}
	/*
	 * - - - - - - - - - - - - - - - - - - - - - - - - - 
	 */
	if (strcmp(zwi, "KTHX") == 0) {
		// printf("KTHX - bytecnt=%d izkthxrelcnt=%d\n", bytecnt,
		// izkthxrelcnt);
		// now relocate the YARLY command inside the IZ block
		if (izkthxrelcnt > 0) {
			izkthxrelcnt--;
			a = bytecnt;
			// printf("reloziere izkthx bei %d auf %d\n",
			// izkthxreltab[izkthxrelcnt], a);
			bytes[izkthxreltab[izkthxrelcnt]] = a & 255;
			a = a / 256;
			bytes[izkthxreltab[izkthxrelcnt] + 1] = a & 255;
			a = a / 256;
			bytes[izkthxreltab[izkthxrelcnt] + 2] = a & 255;
			a = a / 256;
			bytes[izkthxreltab[izkthxrelcnt] + 3] = a & 255;
		}
		// now relocate the IZ command of the IZ block
		if (nowairelcnt > 0) {
			if (nowaiizlevel[nowairelcnt - 1] == izlevel) {
				nowairelcnt--;
				a = bytecnt;
				// printf("reloziere nowai bei %d auf %d\n",
				// nowaireltab[nowairelcnt], a);
				bytes[nowaireltab[nowairelcnt]] = a & 255;
				a = a / 256;
				bytes[nowaireltab[nowairelcnt] + 1] = a & 255;
				a = a / 256;
				bytes[nowaireltab[nowairelcnt] + 2] = a & 255;
				a = a / 256;
				bytes[nowaireltab[nowairelcnt] + 3] = a & 255;
			}
		}
		izlevel--;	// done with this statement
		return (CMD_GOTO);
	}
	/*
	 * - - - - - - - - - - - - - - - - - - - - - - - - - 
	 */
	fprintf(stderr, "ERROR 00002 -2- Unknown statement >%s<.\n", s);
	sprintf(zwi, "ERROR 00002 -2- Unknown statement >%s<.", s);
	write_line_to_list(zwi);
	errorcnt++;

	return (0);
}

/******************************************************************************/
void do_dbdump(void)
{
	dbstr *dbp;

	dbp = dbptr;

	for (;;) {
		if (dbp == NULL) {
			break;

		}
		if (dbp->category == NULL) {
			printf(">< ");
		}
		else {

			printf(">");

			print_string(dbp->category);

			printf("< ");

		}
		if (dbp->key == NULL) {
			printf(">< ");
		}
		else {

			printf(">");

			print_string(dbp->key);
			printf("< ");

		}
		if (dbp->data == NULL) {
			printf("><");
		}
		else {

			printf(">");

			print_string(dbp->data);
			printf("< ");

		}
		puts("");
		dbp = dbp->next;
	}
}

/******************************************************************************/
void do_execute()
{
 do_put_var:
	{
		int c;
		int var;
		int content;

		c = bytes[pptr++];
		var = c;
		c = bytes[pptr++];
		var = var + (c * 256);

		if ((var & GLOVARIABLE) == GLOVARIABLE) {	// global variable?
			content = varcontent[var];
		}
		else {
			content = varcontent[var + oldvarptr];
		}

		if (sp >= MAXSTACK) {
			do_coredump("Stack overflow.");
			exit(84);
		}
		stack[sp++] = content;

		GO_ON;
	}
 do_up:
	{
		int a;
		int c;
		int content;

		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);

		if ((a & GLOVARIABLE) == GLOVARIABLE) {	// global variable?
			content = varcontent[a];
		}
		else {
			content = varcontent[a + oldvarptr];
		}
		content++;
		varcontent[a + oldvarptr] = content;
		if ((a & GLOVARIABLE) == GLOVARIABLE) {	// global variable?
			varcontent[a] = content;
		}
		else {
			varcontent[a + oldvarptr] = content;
		}

		GO_ON;
	}

 do_down:
	{
		int a;
		int c;
		int content;

		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);

		if ((a & GLOVARIABLE) == GLOVARIABLE) {	// global variable?
			content = varcontent[a];
		}
		else {
			content = varcontent[a + oldvarptr];
		}
		content--;
		if ((a & GLOVARIABLE) == GLOVARIABLE) {	// global variable?
			varcontent[a] = content;
		}
		else {
			varcontent[a + oldvarptr] = content;
		}

		GO_ON;
	}
 do_add_variables:
	{
		int a;
		int b;
		int c;
		int content;
		int content2;

		a = bytes[pptr++];
		a = a + (bytes[pptr++] * 256);

		b = bytes[pptr++];
		b = b + (bytes[pptr++] * 256);

		c = bytes[pptr++];
		c = c + (bytes[pptr++] * 256);

		//printf("do_add_variables: a=%d b=%d c=%d oldvarptr=%d\n", a, b,
		//      c, oldvarptr);
		//printf("vor varcontent[a]=%ld\n", varcontent[a + oldvarptr]);
		//printf("vor varcontent[b]=%ld\n", varcontent[b + oldvarptr]);
		//printf("vor varcontent[c]=%ld\n", varcontent[c + oldvarptr]);
		if ((b & GLOVARIABLE) == GLOVARIABLE) {	// global variable?
			content = varcontent[b];
		}
		else {
			content = varcontent[b + oldvarptr];
		}
		if ((c & GLOVARIABLE) == GLOVARIABLE) {	// global variable?
			content2 = varcontent[c];
		}
		else {
			content2 = varcontent[c + oldvarptr];
		}
		if ((a & GLOVARIABLE) == GLOVARIABLE) {	// global variable?
			varcontent[a] = content + content2;
		}
		else {
			varcontent[a + oldvarptr] = content + content2;
		}
		//printf("nach varcontent[a]=%ld\n", varcontent[a + oldvarptr]);
		//printf("nach varcontent[b]=%ld\n", varcontent[b + oldvarptr]);
		//printf("nach varcontent[c]=%ld\n", varcontent[c + oldvarptr]);

		GO_ON;
	}
 do_sub_variables:
	{
		int a;
		int b;
		int c;
		int content;
		int content2;

		a = bytes[pptr++];
		a = a + (bytes[pptr++] * 256);

		b = bytes[pptr++];
		b = b + (bytes[pptr++] * 256);

		c = bytes[pptr++];
		c = c + (bytes[pptr++] * 256);

		if ((b & GLOVARIABLE) == GLOVARIABLE) {	// global variable?
			content = varcontent[b];
		}
		else {
			content = varcontent[b + oldvarptr];
		}
		if ((c & GLOVARIABLE) == GLOVARIABLE) {	// global variable?
			content2 = varcontent[c];
		}
		else {
			content2 = varcontent[c + oldvarptr];
		}
		if ((a & GLOVARIABLE) == GLOVARIABLE) {	// global variable?
			varcontent[a] = content - content2;
		}
		else {
			varcontent[a + oldvarptr] = content - content2;
		}

		GO_ON;
	}
 do_mul_variables:{
		int a;
		int b;
		int c;
		int content;
		int content2;

		a = bytes[pptr++];
		a = a + (bytes[pptr++] * 256);

		b = bytes[pptr++];
		b = b + (bytes[pptr++] * 256);

		c = bytes[pptr++];
		c = c + (bytes[pptr++] * 256);

		if ((b & GLOVARIABLE) == GLOVARIABLE) {	// global variable?
			content = varcontent[b];
		}
		else {
			content = varcontent[b + oldvarptr];
		}
		if ((c & GLOVARIABLE) == GLOVARIABLE) {	// global variable?
			content2 = varcontent[c];
		}
		else {
			content2 = varcontent[c + oldvarptr];
		}
		if ((a & GLOVARIABLE) == GLOVARIABLE) {	// global variable?
			varcontent[a] = content * content2;
		}
		else {
			varcontent[a + oldvarptr] = content * content2;
		}

		GO_ON;
	}
 do_div_variables:{
		int a;
		int b;
		int c;
		int content;
		int content2;

		a = bytes[pptr++];
		a = a + (bytes[pptr++] * 256);

		b = bytes[pptr++];
		b = b + (bytes[pptr++] * 256);

		c = bytes[pptr++];
		c = c + (bytes[pptr++] * 256);

		if ((b & GLOVARIABLE) == GLOVARIABLE) {	// global variable?
			content = varcontent[b];
		}
		else {
			content = varcontent[b + oldvarptr];
		}
		if ((c & GLOVARIABLE) == GLOVARIABLE) {	// global variable?
			content2 = varcontent[c];
		}
		else {
			content2 = varcontent[c + oldvarptr];
		}
		if ((a & GLOVARIABLE) == GLOVARIABLE) {	// global variable?
			varcontent[a] = content / content2;
		}
		else {
			varcontent[a + oldvarptr] = content / content2;
		}

		GO_ON;
	}
 do_goto:{
		int a;
		int c;

		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		c = bytes[pptr++];
		a = a + (c * 256 * 256);
		c = bytes[pptr++];
		a = a + (c * 256 * 256 * 256);

		// printf("GOTO a=%d\n", a);
		pptr = a;

		GO_ON;
	}
 do_gosub:{
		int a;
		int off;
		int c;

		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		c = bytes[pptr++];
		a = a + (c * 256 * 256);
		c = bytes[pptr++];
		a = a + (c * 256 * 256 * 256);

		// now get the offset for the varstack
		c = bytes[pptr++];
		off = c;
		c = bytes[pptr++];
		off = off + (c * 256);

		// printf("GOSUB a=%d, off=%d\n", a, off);
		if (retsp >= (MAXRETSTACK - 1)) {
			do_coredump("return stack overflow.");
			exit(85);
		}
		retstack[retsp] = pptr;
		varretstack[retsp] = varptr;
		oldvarretstack[retsp] = oldvarptr;

		oldvarptr = varptr;	// here our parameters and local var start
		locvarptr = varptr;	// to assign the types to the local vars
		varptr += off;	// this points now AFTER our parameters
		// and local vars

		retsp++;

		pptr = a;

		GO_ON;
	}
 do_return:{
		int t;

		if (retsp < 1) {
			do_coredump("return stack underflow.");
			exit(86);
		}
		// free allocated strings from our local variables
		for (t = oldvarptr; t < varptr; t++) {
			if (vartype[t] == '$') {
				free((char *)varcontent[t]);
			}
			varcontent[t] = 0;	// cleanup
			vartype[t] = ' ';
		}

		retsp--;
		pptr = retstack[retsp];
		varptr = varretstack[retsp];	// free the space of the local
		// variables
		oldvarptr = oldvarretstack[retsp];	// free the space of the local
		// variables

		GO_ON;
	}
 do_tovarstackpstart:{
		// a new parameter list starts
		parvarptr = varptr;
		// printf("tovarstackpstart:parvarptr=%d\n", parvarptr);

		GO_ON;
	}
 do_tovarstackint:{
		int a;

		if (varptr >= MAXVARIABLES) {
			do_coredump("var stack overflow.");
			exit(87);
		}
		a = read_signed_number();
		varcontent[varptr++] = a;

		GO_ON;
	}
 do_tovarstackintpar:{
		int a;

		if (parvarptr >= MAXVARIABLES) {
			do_coredump("var stack overflow.");
			exit(88);
		}
		a = read_signed_number();
		// printf("tovarstackintpar: parvarptr=%d a=%d\n", parvarptr, a);
		varcontent[parvarptr++] = a;

		GO_ON;
	}
 do_tovarstackintvar:{
		int a;
		int c;

		if (varptr >= MAXVARIABLES) {
			do_coredump("var stack overflow.");
			exit(89);
		}
		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		// a = the absolute variable index, not relative!!
		varcontent[varptr++] = varcontent[a];

		GO_ON;
	}
 do_tovarstackintvarpar:{
		int a;
		int c;

		if (parvarptr >= MAXVARIABLES) {
			do_coredump("var stack overflow.");
			exit(90);
		}
		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		// a = the absolute variable index, not relative!!
		varcontent[parvarptr++] = varcontent[a];

		GO_ON;
	}
 do_tovarstackstrvar:{
		int a;
		int c;

		if (varptr >= MAXVARIABLES) {
			do_coredump("var stack overflow.");
			exit(91);
		}
		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		// a = the absolute variable index, not relative!!
		if (varcontent[varptr] != 0) {
			free((char *)varcontent[varptr]);
			varcontent[varptr] = 0;
		}
		varcontent[varptr] =
		    (long)malloc(strlen((char *)varcontent[a]) + 1);
		strcpy((char *)varcontent[varptr++], (char *)varcontent[a]);

		GO_ON;
	}
 do_tovarstackstrvarpar:{
		int a;
		int c;

		if (parvarptr >= MAXVARIABLES) {
			do_coredump("var stack overflow.");
			exit(92);
		}
		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		// a = the absolute variable index, not relative!!
		if (varcontent[parvarptr] != 0) {
			free((char *)varcontent[parvarptr]);
			varcontent[parvarptr] = 0;
		}
		varcontent[parvarptr] =
		    (long)malloc(strlen((char *)varcontent[a]) + 1);
		strcpy((char *)varcontent[parvarptr++], (char *)varcontent[a]);

		GO_ON;
	}
 do_tovarstacktype:{
		int c;

		// tovarstacktype doesnt increase varptr, that is done in one of the
		// other
		// tovarstack.... commands

		c = bytes[pptr++];	// the variable type of the variable to
		// come
		vartype[varptr] = c;
		// printf("tovarstacktype varptr=%d, type=>%c<\n", varptr, c);

		GO_ON;
	}
 do_reserve_params:{
		int c;

		// reserve space on the varstack for the parameters
		// by increasing locvarptr

		c = bytes[pptr++];	// the amount slots used by parameters on
		// the varstack
		locvarptr += c;
		// printf("reserve_params: locvarptr=%d, type=>%c<\n", locvarptr);

		GO_ON;
	}
 do_tovarstacktypepar:{
		int c;

		// tovarstacktypepar doesnt increase parvarptr, that is done in one of 
		// the other
		// tovarstack...par... commands

		c = bytes[pptr++];	// the variable type of the variable to
		// come
		vartype[parvarptr] = c;
		// printf("tovarstacktypepar parvarptr=%d, type=>%c<\n", parvarptr,
		// c);

		GO_ON;
	}
 do_tovarstackloctype:{
		int c;

		// tovarstacktype uses locarptr, a temporary pointer

		c = bytes[pptr++];	// the variable type of the variable to come
		vartype[locvarptr++] = c;
		// printf("tovarstackloctype locvarptr=%d, type=>%c<\n", locvarptr,
		// c);

		GO_ON;
	}

 do_tovarstackname:{
		int c;
		int t;
		char zwi[256];

		// tovarstackname uses varptr
		//printf("do_tovarstackname at pptr=%ld ", pptr);
		for (t = 0;; t++) {
			c = bytes[pptr++];
			zwi[t] = c;
			if (c == 0)
				break;
		}
		//printf(" name=>%s<\n", zwi);
		if (varname[varptr] != NULL) {
			free(varname[varptr]);
		}
		varname[varptr] = malloc(strlen(zwi) + 1);
		strcpy(varname[varptr], zwi);

		GO_ON;
	}
 do_tovarstacklocname:{
		int c;
		int t;
		char zwi[256];

		// tovarstacklocname uses locvarptr
		//printf("do_tovarstacklocname at pptr=%ld ", pptr);
		for (t = 0;; t++) {
			c = bytes[pptr++];
			zwi[t] = c;
			if (c == 0)
				break;
		}
		//printf(" name=>%s<\n", zwi);
		if (varname[locvarptr] != NULL) {
			free(varname[locvarptr]);
		}
		varname[locvarptr] = malloc(strlen(zwi) + 1);
		strcpy(varname[locvarptr], zwi);

		GO_ON;
	}
 do_tovarstacknamepar:{
		int c;
		int t;
		char zwi[256];

		// tovarstacknamepar uses parvarptr
		//printf("do_tovarstacknamepar at pptr=%ld ", pptr);
		for (t = 0;; t++) {
			c = bytes[pptr++];
			zwi[t] = c;
			if (c == 0)
				break;
		}
		//printf(" name=>%s<\n", zwi);
		if (varname[parvarptr] != NULL) {
			free(varname[parvarptr]);
		}
		varname[parvarptr] = malloc(strlen(zwi) + 1);
		strcpy(varname[parvarptr], zwi);

		GO_ON;
	}

 do_tovarstackstr:{
		int c;
		int t;
		char zwi[256];

		if (varptr >= MAXVARIABLES) {
			do_coredump("var stack overflow.");
			exit(93);
		}
		for (t = 0;; t++) {
			c = bytes[pptr++];
			zwi[t] = c;
			if (c == 0)
				break;
		}
		varcontent[varptr] = (long)make_string(zwi, strlen(zwi));
		varptr++;

		GO_ON;
	}
 do_tovarstackstrpar:{
		int c;
		int t;
		int l;
		char zwi[256];

		if (parvarptr >= MAXVARIABLES) {
			do_coredump("var stack overflow.");
			exit(94);
		}
		l = bytes[pptr++];
		l += bytes[pptr++] * 256;
		l += bytes[pptr++] * 256 * 256;
		l += bytes[pptr++] * 256 * 256 * 256;
		for (t = 0; t < l; t++) {
			c = bytes[pptr++];
			zwi[t] = c;
			//if (c == 0){
			//      break;
			//}
		}
		// printf("tovarstackstrpar: parvarptr=%d\n", parvarptr);
		varcontent[parvarptr] = (long)malloc(strlen(zwi) + 1);
		strcpy((char *)varcontent[parvarptr], zwi);
		varcontent[parvarptr] = (long)make_string(zwi, l);
		parvarptr++;

		GO_ON;
	}
 do_visible_text:{
		char zwi[256];
		int t;
		int c;
		int crlf = 0;

		crlf = bytes[pptr++];

		for (t = 0;; t++) {
			c = bytes[pptr++];
			// printf("c=%d >%c<\n", c, c);
			zwi[t] = c;
			if (c == 0) {
				break;
			}
		}
		printf("%s", zwi);
		if (crlf == 1) {
			puts("");
		}

		GO_ON;
	}
 do_visible_var:{
		long val;
		int a;
		int c;
		int pos;
		int crlf = 0;
		long len;
		unsigned char *ptr = NULL;

		crlf = bytes[pptr++];

		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		//printf("visible: a=%d oldvarptr=%d crlf=%d vartype=%c\n", a,
		//       oldvarptr, crlf, vartype[a + oldvarptr]);
		if ((a & GLOVARIABLE) == GLOVARIABLE) {	// global variable
			//printf("global variable: vartype[a]=>%c<\n",
			//       vartype[a]);
			if (vartype[a] == '%') {
				pos = a;
				val = varcontent[pos];
				printf("%ld", val);
			}
			if (vartype[a] == '$') {
				pos = a;
				val = varcontent[pos];
				//printf("visible: pos=%d val=%d\n", pos, val);
				if ((char *)val == NULL) {
					printf("<NULL>");
				}
				else {
					ptr =
					    extract_string(&len,
							   (unsigned char *)
							   val);
					printf("%s", ptr);
				}
			}
		}
		else {		// local variable or parameter
			if (vartype[a + oldvarptr] == '%') {
				pos = a;
				val = varcontent[pos + oldvarptr];
				printf("%ld", val);
			}
			if (vartype[a + oldvarptr] == '$') {
				pos = a;
				val = varcontent[pos + oldvarptr];
				//printf("visible: pos=%d val=%d\n", pos, val);
				if ((char *)val == NULL) {
					printf("<NULL>");
				}
				else {
					ptr =
					    extract_string(&len,
							   (unsigned char *)
							   val);
					printf("%s", ptr);
				}
			}
		}
		if (crlf == 1) {
			puts("");
		}

		GO_ON;
	}
 do_gimmeh:{
		int a;
		int c;
		int crlf = 0;
		int varidx;
		int filedesc;
		int z;

		// get the variable index
		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		varidx = a + oldvarptr;

		// get the file descriptor
		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		filedesc = a;

		if (filedesc == 0) {	// stdin
			z = 0;
			for (;;) {
				c = fgetc(stdin);
				if (c == EOF || c == '\n') {
					gbuff[z] = 0;
					break;
				}
				else {
					if (c != '\r') {
						gbuff[z++] = c;
					}
				}
			}
		}

		if (varcontent[varidx] != 0) {
			free((char *)varcontent[varidx]);
		}
		varcontent[varidx] = (long)make_string(gbuff, strlen(gbuff));
		gbuff[0] = 0;

		GO_ON;
	}
 do_inyrloop_var_num:{
		int var1;
		long content;
		int operator;
		long num;
		int addr;
		long a;
		int c;
		int f;

		//printf("inyrloop_var_num\n");
		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		var1 = a;

		content = varcontent[var1 + oldvarptr];

		operator= bytes[pptr++];

		a = read_signed_number();
		num = a;

		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		c = bytes[pptr++];
		a = a + (c * 256 * 256);
		c = bytes[pptr++];
		a = a + (c * 256 * 256 * 256);
		addr = a;

		//printf("inyrloop_var_num: content=%d num=%d operator=%d\n", content,
		//      num, operator);
		f = 0;
		if (operator== 97) {	// BIGGER
			if (content > num)
				f = 1;
		}
		if (operator== 98) {	// SMALLER
			if (content < num)
				f = 1;
		}
		if (operator== 99) {	// EQUALS
			if (content == num)
				f = 1;
		}
		//printf("inyrloop_var_num: content=%d operator=%d num=%d f=%d addr=%d\n",
		//      content, operator, num, f, addr);
		if (f == 1) {	// condition is true, jump behind OUTTA YR LOOP
			pptr = addr;
			//printf("going to addr %d now\n", addr);
		}

		GO_ON;
	}
 do_inyrloop_var_var:{
		int var1;
		int var2;
		long content1;
		long content2;
		int operator;
		int addr;
		long a;
		int c;
		int f;

		//printf("inyrloop_var_var\n");
		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		var1 = a;

		content1 = varcontent[var1 + oldvarptr];

		operator= bytes[pptr++];

		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		var2 = a;

		content2 = varcontent[var2 + oldvarptr];

		// printf("inyrloop_var_var: getting address from pptr %d\n", pptr);
		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		c = bytes[pptr++];
		a = a + (c * 256 * 256);
		c = bytes[pptr++];
		a = a + (c * 256 * 256 * 256);
		addr = a;

		//printf("inyrloop content1=%ld content2=%ld operator=%d\n", content1,
		// content2, operator);
		f = 0;
		if (operator== 97) {	// BIGGER
			if (content1 > content2)
				f = 1;
		}
		if (operator== 98) {	// SMALLER
			if (content1 < content2)
				f = 1;
		}
		if (operator== 99) {	// EQUALS
			if (content1 == content2)
				f = 1;
		}
		// printf("iz_var_var: content1=%d operator=%d content2=%d f=%d
		// addr=%d\n",
		// content1, operator, content2, f, addr);
		if (f == 1) {	// condition is true, jump behind OUTTA YR LOOP
			// iz-kthx block!
			pptr = addr;
			// printf("iz_var_var: going to addr %d now\n", addr);
		}

		GO_ON;
	}
 do_iz_var_num:{
		int var1;
		long content;
		int operator;
		long num;
		int addr;
		long a;
		int c;
		int f;

		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		var1 = a;

		content = varcontent[var1 + oldvarptr];

		operator= bytes[pptr++];

		num = read_signed_number();

		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		c = bytes[pptr++];
		a = a + (c * 256 * 256);
		c = bytes[pptr++];
		a = a + (c * 256 * 256 * 256);
		addr = a;

		f = 0;
		if (operator== 97) {	// BIGGER
			if (content > num)
				f = 1;
		}
		if (operator== 98) {	// SMALLER
			if (content < num)
				f = 1;
		}
		if (operator== 99) {	// EQUALS
			if (content == num)
				f = 1;
		}
		//printf("iz_var_num: content=%d operator=%d num=%d f=%d addr=%d\n",
		//      content, operator, num, f, addr);
		if (f == 0) {	// condition is false, jump over the
			// iz-kthx block!
			pptr = addr;
			//printf("going to addr %d now\n", addr);
		}

		GO_ON;
	}
 do_iz_var_var:{
		int var1;
		int var2;
		long content1;
		long content2;
		int operator;
		int addr;
		long a;
		int c;
		int f;

		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		var1 = a;

		content1 = varcontent[var1 + oldvarptr];

		operator= bytes[pptr++];

		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		var2 = a;

		content2 = varcontent[var2 + oldvarptr];

		// printf("iz_var_var: getting address from pptr %d\n", pptr);
		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		c = bytes[pptr++];
		a = a + (c * 256 * 256);
		c = bytes[pptr++];
		a = a + (c * 256 * 256 * 256);
		addr = a;

		f = 0;
		if (operator== 97) {	// BIGGER
			if (content1 > content2)
				f = 1;
		}
		if (operator== 98) {	// SMALLER
			if (content1 < content2)
				f = 1;
		}
		if (operator== 99) {	// EQUALS
			if (content1 == content2)
				f = 1;
		}
		// printf("iz_var_var: content1=%d operator=%d content2=%d f=%d
		// addr=%d\n",
		// content1, operator, content2, f, addr);
		if (f == 0) {	// condition is false, jump over the
			// iz-kthx block!
			pptr = addr;
			// printf("iz_var_var: going to addr %d now\n", addr);
		}

		GO_ON;
	}
 do_iz_var_str:{
		int var1;
		char *str1;
		char str2[256];
		int operator;
		int addr;
		int a;
		int c;
		int f;
		int z;

		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		var1 = a;

		str1 = (char *)varcontent[var1 + oldvarptr];

		operator= bytes[pptr++];

		for (z = 0;;) {
			c = bytes[pptr++];
			str2[z++] = c;
			if (c == 0)
				break;
		}
		// printf("iz_var_str: getting address from pptr %d\n", pptr);
		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		c = bytes[pptr++];
		a = a + (c * 256 * 256);
		c = bytes[pptr++];
		a = a + (c * 256 * 256 * 256);
		addr = a;

		f = 0;
		if (operator== 97) {	// BIGGER
			if (strcmp(str1, str2) > 0)
				f = 1;
		}
		if (operator== 98) {	// SMALLER
			if (strcmp(str1, str2) < 0)
				f = 1;
		}
		if (operator== 99) {	// EQUALS
			if (strcmp(str1, str2) == 0)
				f = 1;
		}
		// printf("iz_var_str: str1=>%s< operator=%d str2=>%s< f=%d
		// addr=%d\n",
		// str1, operator, str2, f, addr);
		if (f == 0) {	// condition is false, jump over the
			// iz-kthx block!
			pptr = addr;
			// printf("iz_var_str: going to addr %d now\n", addr);
		}

		GO_ON;
	}
 do_lol_str_var:{
		int var1;
		int var2;
		int a;
		int c;

		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		var1 = a;

		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		var2 = a;

		if ((char *)varcontent[var1 + oldvarptr] != NULL) {
			free((char *)varcontent[var1 + oldvarptr]);
			varcontent[var1 + oldvarptr] = 0;
		}
		varcontent[var1 + oldvarptr] =
		    (long)copy_string((unsigned char *)
				      varcontent[var2 + oldvarptr]);
		if (varcontent[var1 + oldvarptr] == 0) {
			do_coredump("Out of memory.");
			exit(95);
		}
		GO_ON;
	}
 do_lol_str:{
		int var1;
		int a;
		int c;
		int t;
		int l;
		char zwi[256];

		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		var1 = a;
		//printf("cmd_lol_str: var1=%d, oldvarptr=%d\n", var1, oldvarptr);
		l = 0;
		for (t = 0;; t++) {
			c = bytes[pptr++];
			zwi[t] = c;
			if (c == '"')
				break;
			if (c == 0)
				break;
			l++;
		}
		zwi[t] = 0;
		//printf("cmd_lol_str: zwi=>%s<\n", zwi);
		varcontent[var1 + oldvarptr] =
		    (long)make_string(zwi, strlen(zwi));
		//printf("cmd_lol_str: varcontent=%ld\n", varcontent[var1 + oldvarptr]);

		GO_ON;
	}
 do_lol_num:{
		int var1;
		int num;
		int a;
		int c;

		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		var1 = a;
		//printf("do_lol_num: var1=%d\n", var1);
		a = read_signed_number();
		num = a;
		//printf("do_lol_num: num=%d\n", num);
		if ((var1 & GLOVARIABLE) == GLOVARIABLE) {	// global variable?
			varcontent[var1] = num;
		}
		else {
			varcontent[var1 + oldvarptr] = num;
		}

		GO_ON;
	}
 do_lol_num_var:{
		int var1;
		int var2;
		int a;
		int c;

		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		var1 = a;

		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		var2 = a;

		//printf("lol_num_var: var1=%d var2=%d\n", var1, var2);
		if ((var2 & GLOVARIABLE) == GLOVARIABLE) {	// global variable?
			a = varcontent[var2];
		}
		else {
			a = varcontent[var2 + oldvarptr];
		}
		if ((var1 & GLOVARIABLE) == GLOVARIABLE) {	// global variable?
			varcontent[var1] = a;
		}
		else {
			varcontent[var1 + oldvarptr] = a;
		}

		GO_ON;
	}
 do_lol_num_rnd:
	{
		int var1;
		int limit;
		int a;
		int c;

		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		var1 = a;

		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		limit = a;

		if (limit == 0) {
			a = rand();
		}
		else {
			a = rand() % limit;
		}
		if ((var1 & GLOVARIABLE) == GLOVARIABLE) {	// global variable?
			varcontent[var1] = a;
		}
		else {
			varcontent[var1 + oldvarptr] = a;
		}

		GO_ON;
	}

 do_lol_errorcode:{
		int var1;
		int a;
		int c;
		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		var1 = a;

		if ((var1 & GLOVARIABLE) == GLOVARIABLE) {	// global variable?
			varcontent[var1] = gerrorcode;
		}
		else {
			varcontent[var1 + oldvarptr] = gerrorcode;
		}

		GO_ON;
	}

 do_lol_strcat:{
		int var1;
		int var2;
		int a;
		int b;
		int c;
		int t;
		int l2;
		long len;
		char *ptr1, *ptr2;

		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		var1 = a;

		b = bytes[pptr++];
		if (b == 0) {	// it's a string
			for (t = 0;; t++) {
				gbuff[t] = bytes[pptr++];
				if (gbuff[t] == 0)
					break;
			}
		}
		else {		// it's a string variable
			c = bytes[pptr++];
			a = c;
			c = bytes[pptr++];
			a = a + (c * 256);
			var2 = a;
			ptr2 = (char *)varcontent[var2 + oldvarptr];
			ptr1 = extract_string(&len, ptr2);
			strcpy(gbuff, ptr1);
			gbuff[len] = 0;
			free(ptr1);
		}
		if ((char *)varcontent[var1 + oldvarptr] != NULL) {
			ptr2 = (char *)varcontent[var1 + oldvarptr];
			ptr1 = extract_string(&len, ptr2);
			strcpy(gbuff2, ptr1);
			gbuff2[len] = 0;
			free(ptr1);
			gbuff2[len] = 0;
		}
		else {
			strcpy(gbuff2, "");
		}
		strcat(gbuff2, gbuff);
		l2 = strlen(gbuff2);
		varcontent[var1 + oldvarptr] =
		    (long)make_string(gbuff2, strlen(gbuff2));

		if ((var1 & GLOVARIABLE) == GLOVARIABLE) {	// global variable?
			varcontent[var1] =
			    (long)make_string(gbuff2, strlen(gbuff2));
		}
		else {
			varcontent[var1 + oldvarptr] =
			    (long)make_string(gbuff2, strlen(gbuff2));
		}
		GO_ON;
	}

 do_st0r:{
		char *data = NULL;
		char *key = NULL;
		char *category = NULL;
		int a;
		int c;
		dbstr *dbp;
		int t;
		long len;
		unsigned char *ptr = NULL;

		c = bytes[pptr++];
		if (c == 0) {	// its a string contstant
			for (t = 0;; t++) {
				c = bytes[pptr++];
				gbuff[t] = c;
				if (c == 0)
					break;
			}
			data = make_string(gbuff, strlen(gbuff));
		}
		else {		// its a string variable
			c = bytes[pptr++];
			a = c;
			c = bytes[pptr++];

			a = a + (c * 256);

			if ((char *)varcontent[a + oldvarptr] == NULL) {
				data = make_string("", 0);
			}
			else {
				ptr =
				    extract_string(&len,
						   (unsigned char *)varcontent[a
									       +
									       oldvarptr]);
				data = make_string(ptr, len);
			}
		}
//              printf("do_st0r: data=");
		print_string(data);
		c = bytes[pptr++];	// key
		if (c == 0) {	// its a string contstant
			for (t = 0;; t++) {
				c = bytes[pptr++];
				gbuff[t] = c;
				if (c == 0)
					break;
			}
			key = make_string(gbuff, strlen(gbuff));
		}
		else {		// its a string variable
			c = bytes[pptr++];
			a = c;
			c = bytes[pptr++];
			a = a + (c * 256);
			if ((char *)varcontent[a + oldvarptr] == NULL) {
				key = make_string("", 0);
			}
			else {
				ptr =
				    extract_string(&len,
						   (unsigned char *)varcontent[a
									       +
									       oldvarptr]);
				key = make_string(ptr, len);
			}
		}
//              printf("do_st0r: key=");
		print_string(key);

		c = bytes[pptr++];	// category
		if (c == 0) {	// its a string contstant
			for (t = 0;; t++) {
				c = bytes[pptr++];
				gbuff[t] = c;
				if (c == 0)
					break;
			}
			category = make_string(gbuff, strlen(gbuff));
		}
		else {		// its a string variable
			c = bytes[pptr++];
			a = c;
			c = bytes[pptr++];
			a = a + (c * 256);
			if ((char *)varcontent[a + oldvarptr] == NULL) {
				category = make_string("", 0);
			}
			else {
				ptr =
				    extract_string(&len,
						   (unsigned char *)varcontent[a
									       +
									       oldvarptr]);
				category = make_string(ptr, len);
			}
		}
//              printf("do_st0r: category=");
		print_string(category);

		dbp = malloc(sizeof(struct dbstruct));
		dbp->next = dbptr;
		dbptr = dbp;

		dbp->data = NULL;
		dbp->data = copy_string(data);

		dbp->key = NULL;
		dbp->key = copy_string(key);

		dbp->category = NULL;
		dbp->category = copy_string(category);

		free(data);
		free(key);
		free(category);

		GO_ON;
	}

 do_gief:{
		int data;	// data MUST BE a string variable
		char *key;
		char *category;
		int a;
		int c;
		dbstr *dbp;
		int f;
		int t;
		int l;
		char *ptr;

		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		data = a;

		c = bytes[pptr++];	// key
		if (c == 0) {	// its a string contstant
			for (t = 0;; t++) {
				c = bytes[pptr++];
				gbuff[t] = c;
				if (c == 0)
					break;
			}
			key = make_string(gbuff, t);
		}
		else {		// its a string variable
			c = bytes[pptr++];
			a = c;
			c = bytes[pptr++];
			a = a + (c * 256);
			ptr = (char *)varcontent[a + oldvarptr];
			if (ptr == NULL) {
				key = make_string("", 0);
			}
			else {
				key = copy_string(ptr);
			}
		}
//        printf("do_gief: key=");
		print_string(key);
		c = bytes[pptr++];	// category
		if (c == 0) {	// its a string contstant
			for (t = 0;; t++) {
				c = bytes[pptr++];
				gbuff[t] = c;
				if (c == 0)
					break;
			}
			category = malloc(t + 1);
			if (category == NULL) {
				do_coredump("Out of memory.");
				exit(96);
			}
			//strcpy(category, gbuff);
			category = make_string(gbuff, t);
		}
		else {		// its a string variable
			c = bytes[pptr++];
			a = c;
			c = bytes[pptr++];
			a = a + (c * 256);
			ptr = (char *)varcontent[a + oldvarptr];
			if (ptr == NULL) {
				category = malloc(1);
				if (category == NULL) {
					do_coredump("Out of memory.");
					exit(97);
				}
				category[0] = 0;
			}
			else {
				c = ptr[0];
				l = c;
				c = ptr[1];
				l = l + (c * 256);
				c = ptr[2];
				l = l + (c * 256 * 256);
				c = ptr[3];
				l = l + (c * 256 * 256 * 256);
				category = malloc(l + 4 + 1);
				if (category == NULL) {
					do_coredump("Out of memory.");
					exit(98);
				}
				memcpy(category, ptr, l + 4);
			}
		}
//        printf("do_gief: category=");
		print_string(category);

		dbp = dbptr;

		f = 0;
		for (;;) {
			if (dbp == NULL)
				break;
			if (strcmp((char *)dbp->category, category) == 0) {
				if (strcmp((char *)dbp->key, key) == 0) {
					f = 1;
					if ((char *)varcontent[data + oldvarptr]
					    != NULL) {
						free((char *)
						     varcontent[data +
								oldvarptr]);
					}
					c = dbp->data[0];
					l = c;
					c = dbp->data[1];
					l = l + (c * 256);
					c = dbp->data[2];
					l = l + (c * 256 * 256);
					c = dbp->data[3];
					l = l + (c * 256 * 256 * 256);
					varcontent[data + oldvarptr] =
					    (long)malloc(l + 4 + 1);
					if (varcontent[data + oldvarptr] == 0) {
						do_coredump("Out of memory.");
						exit(99);
					}
					ptr =
					    (char *)varcontent[data +
							       oldvarptr];
					memcpy(ptr, (char *)dbp->data, l + 4);
					break;
				}
			}
			dbp = dbp->next;
		}

		if (f == 0) {
			if ((char *)varcontent[data + oldvarptr] != NULL) {
				free((char *)varcontent[data + oldvarptr]);
			}
			varcontent[data + oldvarptr] = (long)malloc(1);
			strcpy((char *)varcontent[data + oldvarptr], "");
		}

		free(key);
		free(category);

		GO_ON;
	}

 do_eat_ma:{
		int c;

		c = bytes[pptr++];

		if (c == 0) {	// reset ERRORCODE
			gerrorcode = 0;
		}

		GO_ON;
	}

 do_lol_int2str:{
		int c;
		int a;
		int var1;
		int var2;
		long nbr;
		char zwi[256];

		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		var1 = a;

		c = bytes[pptr++];
		if (c == 0) {	// it's a number
			nbr = read_signed_number();
		}
		else {		// it's an integer variable
			c = bytes[pptr++];
			var2 = c;
			c = bytes[pptr++];
			var2 += c;
			nbr = varcontent[var2 + oldvarptr];
		}

		sprintf(zwi, "%ld", nbr);
		varcontent[var1 + oldvarptr] =
		    (long)make_string(zwi, strlen(zwi));

		GO_ON;
	}

 do_lol_str2int:{
		int c;
		int a;
		int var1;
		int var2;
		char zwi[1024];
		int t;
		int l;
		long nbr;
		char *cptr;

		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		var1 = a;

		c = bytes[pptr++];
		if (c == 0) {	// it's a string
			for (t = 0;; t++) {
				c = bytes[pptr++];
				zwi[t] = c;
				if (c == 0)
					break;
			}
			// printf("zwi=>%s<\n", zwi); 
			// printf("nbr=%ld\n", nbr);
			nbr = atol(zwi);
		}
		else {		// it's a string variable
			c = bytes[pptr++];
			var2 = c;
			c = bytes[pptr++];
			var2 += c;
			cptr = (char *)varcontent[var2 + oldvarptr];
			c = cptr[0];
			l = c;
			c = cptr[1];
			l = l + (c * 256);
			c = cptr[2];
			l = l + (c * 256 * 256);
			c = cptr[3];
			l = l + (c * 256 * 256 * 256);
			for (t = 0; t < l; t++) {
				zwi[t] = cptr[t + 4];
			}
			zwi[t] = 0;
			nbr = atol(zwi);
		}
		// printf("nbr=%ld\n", nbr);
		varcontent[var1 + oldvarptr] = nbr;

		GO_ON;
	}

 do_lol_strlen:{
		int c;
		int a;
		int var1;
		int var2;
		char zwi[1024];
		int t;
		long nbr;
		unsigned char *ptr;
		long len;

		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		var1 = a;

		c = bytes[pptr++];
		if (c == 0) {	// it's a string
			for (t = 0;; t++) {
				c = bytes[pptr++];
				zwi[t] = c;
				if (c == 0)
					break;
			}
			//printf("$len: zwi=>%s<\n", zwi);
			// printf("nbr=%ld\n", nbr);
			nbr = strlen(zwi);
		}
		else {		// it's a string variable
			c = bytes[pptr++];
			var2 = c;
			c = bytes[pptr++];
			var2 += c;
			//printf("var2=%d\n", var2);
			//printf("content=>%s<\n", (char*)varcontent[var2+oldvarptr]);
			//puts("== extract_string -10- ==");
			ptr =
			    extract_string(&len,
					   (unsigned char *)varcontent[var2 +
								       oldvarptr]);
			nbr = len;
		}
		// printf("nbr=%ld\n", nbr);
		varcontent[var1 + oldvarptr] = nbr;

		GO_ON;
	}

 do_imp:{
		char *fname;
		char *category;
		int a;
		int c;
		dbstr *dbp;
		int t;
		long lnbr;
		FILE *fp;
		char zwi[256];
		char line[8192];

		gerrorpptr = pptr - 1;	// save the pptr, in case we have an error
		c = bytes[pptr++];	// filename
		if (c == 0) {	// its a string contstant
			for (t = 0;; t++) {
				c = bytes[pptr++];
				gbuff[t] = c;
				if (c == 0)
					break;
			}
			fname = malloc(t + 1);
			strcpy(fname, gbuff);
		}
		else {		// its a string variable
			c = bytes[pptr++];
			a = c;
			c = bytes[pptr++];
			a = a + (c * 256);
			if ((char *)varcontent[a + oldvarptr] == NULL) {
				fname = malloc(1);
				fname[0] = 0;
			}
			else {
				fname =
				    malloc(strlen
					   ((char *)varcontent[a + oldvarptr]));
				strcpy(fname,
				       (char *)varcontent[a + oldvarptr]);
			}
		}

		c = bytes[pptr++];	// category
		if (c == 0) {	// its a string contstant
			for (t = 0;; t++) {
				c = bytes[pptr++];
				gbuff[t] = c;
				if (c == 0)
					break;
			}
			category = make_string(gbuff, strlen(gbuff));
		}
		else {		// its a string variable
			c = bytes[pptr++];
			a = c;
			c = bytes[pptr++];
			a = a + (c * 256);
			category = copy_string((unsigned char *)
					       varcontent[a + oldvarptr]);
		}

		dbp = dbptr;

		// find the highest unused line number
		lnbr = 0;
		for (;;) {
			if (dbp == NULL)
				break;
			if (strcmp((char *)dbp->category, category) == 0) {
				a = atoi((char *)dbp->key);
				if (a > lnbr) {
					lnbr = a + 1;
				}
			}
			dbp = dbp->next;
		}

		fp = fopen(fname, "rb");
		if (fp == NULL) {
			gerrorcode = ERR_FILE_NOT_FOUND;
		}
		else {
			setvbuf(fp, NULL, _IOFBF, 32768);
			for (;;) {
				if (read_one_line(fp, line) == EOF)
					break;
				dbp = malloc(sizeof(struct dbstruct));
				dbp->next = dbptr;
				dbptr = dbp;
				dbp->data = make_string(line, strlen(line));
				sprintf(zwi, "%ld", lnbr);
				lnbr++;
				dbp->key = make_string(zwi, strlen(zwi));
				dbp->category = copy_string(category);
			}
			fclose(fp);

		}

		free(category);
		free(fname);

		GO_ON;
	}

 do_lol_num_pron:{
		int var1;
		int a;
		int c;

		c = bytes[pptr++];
		a = c;
		c = bytes[pptr++];
		a = a + (c * 256);
		var1 = a;

		if (sp < 1) {
			do_coredump("Stack underflow.");
			exit(100);
		}
		sp--;
		a = stack[sp];

		// printf("do_lol_num_pron: a=%d sp=%d\n", a, sp);
		varcontent[var1 + oldvarptr] = a;

		GO_ON;
	}

 do_put_str:{
		int t;
		int c = 0;

		for (t = 0; t < 256; t++) {
			c = bytes[pptr++];
			stringstack[t] = c;
			if (c == 0)
				break;
		}

		GO_ON;
	}

 do_cmd_div:{
		int a, b;

		if (sp < 1) {
			do_coredump("Stack underflow.");
			exit(101);
		}
		sp--;
		a = stack[sp];
		if (sp < 1) {
			do_coredump("Stack underflow.");
			exit(102);
		}
		sp--;
		b = stack[sp];

		a = b / a;

		if (sp >= MAXSTACK) {
			do_coredump("Stack overflow.");
			exit(103);
		}
		stack[sp++] = a;

		GO_ON;
	}

 do_cmd_mul:{
		int a, b;

		if (sp < 1) {
			do_coredump("Stack underflow.");
			exit(104);
		}
		sp--;
		a = stack[sp];
		if (sp < 1) {
			do_coredump("Stack underflow.");
			exit(105);
		}
		sp--;
		b = stack[sp];

		a = a * b;

		if (sp >= MAXSTACK) {
			do_coredump("Stack overflow.");
			exit(106);
		}
		stack[sp++] = a;

		GO_ON;
	}

 do_cmd_sub:{
		int a, b;

		if (sp < 1) {
			do_coredump("Stack underflow.");
			exit(107);
		}
		sp--;
		a = stack[sp];
		if (sp < 1) {
			do_coredump("Stack underflow.");
			exit(108);
		}
		sp--;
		b = stack[sp];

		a = b - a;

		if (sp >= MAXSTACK) {
			do_coredump("Stack overflow.");
			exit(109);
		}
		stack[sp++] = a;
		// printf("cmd_sub: a=%d sp=%d\n", a, sp);

		GO_ON;
	}

 do_cmd_add:{
		int a, b;

		if (sp < 1) {
			do_coredump("Stack underflow.");
			exit(110);
		}
		sp--;
		a = stack[sp];
		if (sp < 1) {
			do_coredump("Stack underflow.");
			exit(111);
		}
		sp--;
		b = stack[sp];

		a = a + b;

		if (sp >= MAXSTACK) {
			do_coredump("Stack overflow.");
			exit(112);
		}
		stack[sp++] = a;
		// printf("cmd_add: a=%d sp=%d\n", a, sp);

		GO_ON;
	}

 do_cmd_dup:{
		int a;

		if (sp < 1) {
			do_coredump("Stack underflow.");
			exit(113);
		}
		sp--;
		a = stack[sp];

		if (sp >= MAXSTACK) {
			do_coredump("Stack overflow.");
			exit(114);
		}
		stack[sp++] = a;
		if (sp >= MAXSTACK) {
			do_coredump("Stack overflow.");
			exit(115);
		}
		stack[sp++] = a;
		// printf("cmd_dup: a=%d sp=%d\n", a, sp);

		GO_ON;
	}

 do_cmd_rot:{
		int a, b, c;

		if (sp < 1) {
			do_coredump("Stack underflow.");
			exit(116);
		}
		sp--;
		a = stack[sp];
		if (sp < 1) {
			do_coredump("Stack underflow.");
			exit(117);
		}
		sp--;
		b = stack[sp];
		if (sp < 1) {
			do_coredump("Stack underflow.");
			exit(118);
		}
		sp--;
		c = stack[sp];

		if (sp >= MAXSTACK) {
			do_coredump("Stack overflow.");
			exit(119);
		}
		stack[sp++] = b;
		if (sp >= MAXSTACK) {
			do_coredump("Stack overflow.");
			exit(120);
		}
		stack[sp++] = a;
		if (sp >= MAXSTACK) {
			do_coredump("Stack overflow.");
			exit(121);
		}
		stack[sp++] = c;

		GO_ON;
	}

 do_cmd_swap:{
		int a, b;

		if (sp < 1) {
			do_coredump("Stack underflow.");
			exit(122);
		}
		sp--;
		a = stack[sp];
		if (sp < 1) {
			do_coredump("Stack underflow.");
			exit(123);
		}
		sp--;
		b = stack[sp];

		if (sp >= MAXSTACK) {
			do_coredump("Stack overflow.");
			exit(124);
		}
		stack[sp++] = a;
		if (sp >= MAXSTACK) {
			do_coredump("Stack overflow.");
			exit(125);
		}
		stack[sp++] = b;

		GO_ON;
	}

 do_smaller_num:{
		int a, b;

		if (sp < 1) {
			do_coredump("Stack underflow.");
			exit(126);
		}
		sp--;
		a = stack[sp];
		if (sp < 1) {
			do_coredump("Stack underflow.");
			exit(127);
		}
		sp--;
		b = stack[sp];

		if (sp >= MAXSTACK) {
			do_coredump("Stack overflow.");
			exit(128);
		}
		if (b < a) {
			stack[sp++] = 1;	// 1 = TRUE
		}
		else {
			stack[sp++] = 0;	// 0 = FALSE
		}

		GO_ON;
	}

 do_equals_num:{
		int a, b;

		if (sp < 1) {
			do_coredump("Stack underflow.");
			exit(129);
		}
		sp--;
		a = stack[sp];
		if (sp < 1) {
			do_coredump("Stack underflow.");
			exit(130);
		}
		sp--;
		b = stack[sp];

		if (sp >= MAXSTACK) {
			do_coredump("Stack overflow.");
			exit(131);
		}
		if (b == a) {
			stack[sp++] = 1;	// 1 = TRUE
		}
		else {
			stack[sp++] = 0;	// 0 = FALSE
		}

		GO_ON;
	}

 do_bigger_num:{
		int a, b;

		if (sp < 1) {
			do_coredump("Stack underflow.");
			exit(132);
		}
		sp--;
		a = stack[sp];
		if (sp < 1) {
			do_coredump("Stack underflow.");
			exit(133);
		}
		sp--;
		b = stack[sp];

		if (sp >= MAXSTACK) {
			do_coredump("Stack overflow.");
			exit(134);
		}
		if (b > a) {
			stack[sp++] = 1;	// 1 = TRUE
		}
		else {
			stack[sp++] = 0;	// 0 = FALSE
		}

		GO_ON;
	}

 do_cmd_drop:{
		if (sp < 1) {
			do_coredump("Stack underflow.");
			exit(135);
		}
		sp--;

		GO_ON;
	}

 do_put_num:{
		int c;
		int num;

		c = bytes[pptr++];

		num = c;

		c = bytes[pptr++];

		num = num + (c * 256);

		c = bytes[pptr++];

		num = num + (c * 256 * 256);

		c = bytes[pptr++];

		num = num + (c * 256 * 256 * 256);

		if (sp >= MAXSTACK) {
			do_coredump("Stack overflow.");
			exit(136);
		}
		stack[sp++] = num;
		// printf("put_num: num=%d sp=%d\n", num, sp);

		GO_ON;
	}

 do_eeks:
	{
		do_coredump("Instruction EEKS! found");
		exit(137);
		GO_ON;
	}

 do_kthxbye:
	{
		goto do_execute_end;
	}

 do_can_haz:
	{
		GO_ON;
	}

 do_can_has:
	{
		GO_ON;
	}

 do_hai:
	{
		GO_ON;
	}

 do_dbdump:
	{
		do_dbdump();
		GO_ON;
	}

 do_execute_end:
	running = FALSE;
}

/******************************************************************************/
void do_vdump(void)
{
	int t;

	puts("-------------- vdump -----------------");
	for (t = 0; t < MAXVARIABLES; t++) {
		if (varname[t] != NULL) {
			printf("%d) %-20s ", t, varname[t]);
		}
		if (vartype[t] == '%') {
			printf("Integer %ld\n", varcontent[t]);
		}
		if (vartype[t] == '$') {
			if ((char *)varcontent[t] == NULL) {
				printf("String  >>>NULL<<<\n");
			}
			else {
				printf("String  >");
				print_string((char *)varcontent[t]);
				printf("<\n");
			}
		}
	}
	puts("-------------- end vdump -------------");

}

/******************************************************************************/
void do_trace(int p)
{
	char zwi[256];
//      int t;

	// for(t=0; t<5; t++){
	// printf("%d) %d\n", t, bytes[t]);
	// }
	printf("varcontent[0]=%ld\n", varcontent[0]);
	sprintf(zwi, "??? (cmd=%d)", bytes[p]);
	switch (bytes[p]) {
	case CMD_PUT_VAR:
		strcpy(zwi, "_PUT_VAR");
		break;
	case CMD_PUT_NUM:
		strcpy(zwi, "_PUT_NUM");
		break;
	case CMD_PUT_STR:
		strcpy(zwi, "_PUT_STR");
		break;
	case CMD_ADD:
		strcpy(zwi, "+");
		break;
	case CMD_SUB:
		strcpy(zwi, "-");
		break;
	case CMD_MUL:
		strcpy(zwi, "*");
		break;
	case CMD_DIV:
		strcpy(zwi, "/");
		break;
	case CMD_DUP:
		strcpy(zwi, "dup");
		break;
	case CMD_SWAP:
		strcpy(zwi, "swap");
		break;
	case CMD_ROT:
		strcpy(zwi, "rot");
		break;
	case CMD_DROP:
		strcpy(zwi, "drop");
		break;
	case CMD_SMALLER_NUM:
		strcpy(zwi, "<");
		break;
	case CMD_EQUALS_NUM:
		strcpy(zwi, "=");
		break;
	case CMD_BIGGER_NUM:
		strcpy(zwi, ">");
		break;
	case CMD_HAI:
		strcpy(zwi, "HAI");
		break;
	case CMD_CAN_HAS:
		strcpy(zwi, "CAN_HAS");
		break;
	case CMD_CAN_HAZ:
		strcpy(zwi, "CAN_HAZ");
		break;
	case CMD_VISIBLE_TEXT:
		strcpy(zwi, "VISIBLE (text)");
		break;
	case CMD_VISIBLE_VAR:
		strcpy(zwi, "VISIBLE (variable)");
		break;
	case CMD_GOTO:
		strcpy(zwi, "GOTO");
		break;
	case CMD_KTHXBYE:
		strcpy(zwi, "KTHXBYE");
		break;
	case CMD_UP:
		strcpy(zwi, "UP");
		break;
	case CMD_DOWN:
		strcpy(zwi, "DOWN");
		break;
	case CMD_IZ:
		strcpy(zwi, "IZ");
		break;
	case CMD_IZ_VAR_VAR:
		strcpy(zwi, "IZ_VAR_VAR");
		break;
	case CMD_IZ_VAR_NUM:
		strcpy(zwi, "IZ_VAR_NUM");
		break;
	case CMD_IZ_VAR_STR:
		strcpy(zwi, "IZ_VAR_STR");
		break;
	case CMD_EEKS:
		strcpy(zwi, "EEKS!");
		break;
	case CMD_LOL_NUM:
		strcpy(zwi, "LOL_NUM");
		break;
	case CMD_LOL_NUM_VAR:
		strcpy(zwi, "LOL_NUM_VAR");
		break;
	case CMD_LOL_NUM_PRON:
		strcpy(zwi, "LOL_NUM_PRON");
		break;
	case CMD_LOL_NUM_RND:
		strcpy(zwi, "LOL_NUM_RND");
		break;
	case CMD_LOL_STR:
		strcpy(zwi, "LOL_STR");
		break;
	case CMD_LOL_STR_VAR:
		strcpy(zwi, "LOL_STR_VAR");
		break;
	case CMD_LOL_ERRORCODE:
		strcpy(zwi, "LOL_ERRORCODE");
		break;
	case CMD_LOL_STRCAT:
		strcpy(zwi, "LOL_STRCAT");
		break;
	case CMD_ENUF:
		strcpy(zwi, "ENUF");
		break;
	case CMD_GOSUB:
		strcpy(zwi, "GOSUB");
		break;
	case CMD_RETURN:
		strcpy(zwi, "RETURN");
		break;
	case CMD_TOVARSTACKINT:
		strcpy(zwi, "TOVARSTACKINT");
		break;
	case CMD_TOVARSTACKSTR:
		strcpy(zwi, "TOVARSTACKSTR");
		break;
	case CMD_TOVARSTACKINTVAR:
		strcpy(zwi, "TOVARSTACKINTVAR");
		break;
	case CMD_TOVARSTACKSTRVAR:
		strcpy(zwi, "TOVARSTACKSTRVAR");
		break;
	case CMD_TOVARSTACKTYPE:
		strcpy(zwi, "TOVARSTACKTYPE");
		break;
	case CMD_TOVARSTACKLOCTYPE:
		strcpy(zwi, "TOVARSTACKLOCTYPE");
		break;
	case CMD_TOVARSTACKPSTART:
		strcpy(zwi, "TOVARSTACKPSTART");
		break;
	case CMD_TOVARSTACKINTPAR:
		strcpy(zwi, "TOVARSTACKINTPAR");
		break;
	case CMD_TOVARSTACKSTRPAR:
		strcpy(zwi, "TOVARSTACKSTRPAR");
		break;
	case CMD_TOVARSTACKINTVARPAR:
		strcpy(zwi, "TOVARSTACKINTVARPAR");
		break;
	case CMD_TOVARSTACKSTRVARPAR:
		strcpy(zwi, "TOVARSTACKSTRVARPAR");
		break;
	case CMD_TOVARSTACKTYPEPAR:
		strcpy(zwi, "TOVARSTACKTYPEPAR");
		break;
	case CMD_TOVARSTACKNAME:
		strcpy(zwi, "TOVARSTACKNAME");
		break;
	case CMD_TOVARSTACKLOCNAME:
		strcpy(zwi, "TOVARSTACKLOCNAME");
		break;
	case CMD_TOVARSTACKNAMEPAR:
		strcpy(zwi, "TOVARSTACKNAMEPAR");
		break;
	case CMD_RESERVE_PARAMS:
		strcpy(zwi, "RESERVE_PARAMS");
		break;
	case CMD_ADD_VARIABLES:
		strcpy(zwi, "ADD_VARIABLESS");
		break;
	case CMD_SUB_VARIABLES:
		strcpy(zwi, "SUB_VARIABLESS");
		break;
	case CMD_MUL_VARIABLES:
		strcpy(zwi, "MUL_VARIABLESS");
		break;
	case CMD_DIV_VARIABLES:
		strcpy(zwi, "DIV_VARIABLESS");
		break;
	case CMD_GIMMEH:
		strcpy(zwi, "GIMMEH");
		break;
	case CMD_ST0R:
		strcpy(zwi, "ST0R");
		break;
	case CMD_GIEF:
		strcpy(zwi, "GIEF");
		break;
	case CMD_IMP:
		strcpy(zwi, "IMP");
		break;
	case CMD_EAT_MA:
		strcpy(zwi, "EAT MA");
		break;
	case CMD_DBDUMP:
		strcpy(zwi, "DBDUMP");
		break;
	case CMD_LOL_INT2STR:
		strcpy(zwi, "LOL_INT2STR");
		break;
	case CMD_LOL_STR2INT:
		strcpy(zwi, "LOL_STR2INT");
		break;
	case CMD_INYRLOOP_VAR_NUM:
		strcpy(zwi, "INYRLOOP_VAR_NUM");
		break;
	case CMD_INYRLOOP_VAR_VAR:
		strcpy(zwi, "INYRLOOP_VAR_VAR");
		break;
	case CMD_LOL_STRLEN:
		strcpy(zwi, "LOL_STRLEN");
		break;
	}
	printf("%08d %s\n", p, zwi);

}

/******************************************************************************/
void do_dbg_help(void)
{
	puts("Debugger help");
	puts("=============");
	puts("help / h / ?    - display this message.");
	puts("ver / version   - display the version of lolcode.");
	puts("ENTER key       - single step one line/command.");
	puts("u / until       - run the program until the end of the current");
	puts("                  subroutine.");
	puts("z / zoom        - run the program and display all lines of executed");
	puts("                  source code.");
	puts("zu / zoomuntil  - run the program and display all lines of executed.");
	puts("                  source code until the end of the current subroutine.");
	puts("vdump           - dump the current variable stack.");
	puts("dbdump          - dump the current content of the internal database.");
	puts("break <line>    - sets a breakpoint at line <line>, like break 20.");
	puts("unbreak <line>  - deletes a breakpoint from line <line>, like unbreak 20.");
	puts("breakif         - sets a conditional break, like breakif idx = 3");
	puts("clearbreakif    - removes all conditional breaks");
	puts("breaks          - lists all current breakpoints.");
	puts("monitor <var>   - displays the content of variable <var> each step");
	puts("unmonitor <var> - stops monitoring variable <var>");
	puts("clearmonitor    - removes all monitors");
	puts("quit/bye/exit   - terminates lolcode");
}

/******************************************************************************/
int dbg_cont_flag = FALSE;
int dbg_zoom_flag = FALSE;
int dbg_until_flag = FALSE;

void do_debug(int pptr)
{
	char zwi[256];
	char zwi2[256];
	char zwi3[256];
	char zwi4[256];
	char display_line[256];
	int line_displayed;
	int a, b;
	int f;
	int t, i;

	if (bytes[pptr] == CMD_RETURN) {
		if (dbg_until_flag == TRUE) {
			dbg_zoom_flag = FALSE;
		}
		dbg_until_flag = FALSE;
	}

	strcpy(display_line, "");
	line_displayed = FALSE;
	for (t = 0; t < MAXDEBUGLINE; t++) {
		if (debug_pptr[t] == pptr) {	// we found the actual byte positon!
			sprintf(display_line, "%s\n", debug_src[t]);	// display this after flags handling
			line_displayed = TRUE;
		}
		if (debug_pptr[t] > pptr) {
			break;
		}
	}

	if (line_displayed == TRUE) {
		// handle conditional breaks
		if (breakifcnt > 0) {
			f = 0;
			for (t = 0; t < breakifcnt; t++) {
				if (breakifvariable[t] != NULL) {
					a = breakifvalue[t];
					for (i = 0; i < MAXVARIABLES; i++) {
						if (varname[i] != NULL) {
							if (strUcmp
							    (breakifvariable[t],
							     varname[i]) == 0) {
								b = varcontent
								    [i];
								f = 0;
								switch
								    (breakifcondition
								     [t]) {
								case 1:
									if (a <
									    b) {
										f = 1;
										strcpy
										    (zwi,
										     "<");
									}
									break;
								case 2:
									if (a >
									    b) {
										f = 1;
										strcpy
										    (zwi,
										     ">");
									}
									break;
								case 3:
									if (a ==
									    b) {
										f = 1;
										strcpy
										    (zwi,
										     "=");
									}
									break;
								case 4:
									if (a !=
									    b) {
										f = 1;
										strcpy
										    (zwi,
										     "!=");
									}
									break;
								}
								if (f == 1) {
									dbg_cont_flag
									    =
									    FALSE;
									dbg_until_flag
									    =
									    FALSE;
									dbg_zoom_flag
									    =
									    FALSE;
									printf
									    ("\n* * * conditional break (%s %s %d) * * *\n",
									     breakifvariable
									     [t],
									     zwi,
									     breakifvalue
									     [t]);
								}
								f = 99;
								break;
							}
						}
					}
					if (f == 99) {
						break;
					}
				}
			}
		}
		// handle breakpoints
		if (breakcnt > 0) {
			for (t = 0; t < debuglcnt; t++) {
				if (pptr == debug_pptr[t]) {
					//printf("pptr=%d debug_pptr=%d debug_line=%d\n", pptr, debug_pptr[t], debug_line[t]);
					for (i = 0; i < MAXBREAKPOINTS; i++) {
						if (breakpoints[i] ==
						    debug_line[t]) {
							//      printf("t=%d breakpointline=%d debug_line=%d pptr=%d debug_pptr=%d\n", t, breakpoints[i], debug_line[t], pptr, debug_pptr[t]);
							dbg_cont_flag = FALSE;
							dbg_until_flag = FALSE;
							dbg_zoom_flag = FALSE;
							puts("\n* * * break * * *");
							break;
						}
					}
					break;
				}
			}
		}
	}
	if (dbg_until_flag == TRUE && dbg_zoom_flag == FALSE) {
		debug_done = TRUE;
		return;
	}
	if (dbg_cont_flag == TRUE) {
		debug_done = TRUE;
		return;
	}
	if (line_displayed == TRUE) {
		printf("%s", display_line);	// NOW display the line of source code
		if (monitorcnt > 0) {
			strcpy(zwi, "");
			for (t = 0; t < monitorcnt; t++) {
				if (monitorvar[t] != NULL) {
					for (i = 0; i < MAXVARIABLES; i++) {
						if (varname[i] != NULL) {
							if (strUcmp
							    (varname[i],
							     monitorvar[t]) ==
							    0) {
								if (vartype[i]
								    == '%') {
									sprintf
									    (zwi2,
									     "%s%c %ld ",
									     varname
									     [i],
									     '%',
									     varcontent
									     [i]);
									strcat
									    (zwi,
									     zwi2);
								}
								else {
									sprintf
									    (zwi2,
									     "%s%c >%s< ",
									     varname
									     [i],
									     '%',
									     (char
									      *)
									     varcontent[i]);
									strcat
									    (zwi,
									     zwi2);
								}
							}
						}
					}
				}
			}
			printf("Monitor>%s\n", zwi);
		}
	}
	if (dbg_zoom_flag == TRUE) {
		debug_done = TRUE;
		return;
	}

	if (line_displayed == TRUE) {
		for (;;) {
			f = 0;
			printf("dbg>");
			read_one_line(stdin, zwi);
			if (strlen(zwi) == 0) {
				debug_done = TRUE;
				f = 1;
			}
			if (strcmp(zwi, "c") == 0 || strcmp(zwi, "cont") == 0) {
				dbg_cont_flag = TRUE;
				debug_done = TRUE;
				f = 1;
			}
			if (strcmp(zwi, "z") == 0 || strcmp(zwi, "zoom") == 0) {
				dbg_zoom_flag = TRUE;
				debug_done = TRUE;
				f = 1;
			}
			if (strcmp(zwi, "u") == 0 || strcmp(zwi, "until") == 0) {
				dbg_until_flag = TRUE;
				debug_done = TRUE;
				f = 1;
			}
			if (strcmp(zwi, "zu") == 0
			    || strcmp(zwi, "zoomuntil") == 0) {
				dbg_until_flag = TRUE;
				dbg_zoom_flag = TRUE;
				debug_done = TRUE;
				f = 1;
			}
			if (strcmp(zwi, "vdump") == 0) {
				do_vdump();
			}
			if (strcmp(zwi, "dbdump") == 0) {
				do_dbdump();
			}
			if (strcmp(zwi, "help") == 0 ||
			    strcmp(zwi, "h") == 0 || strcmp(zwi, "?") == 0) {
				do_dbg_help();
			}
			if (strcmp(zwi, "version") == 0
			    || strcmp(zwi, "ver") == 0) {
				printf("%s (%s %s)\n", VERSION, __DATE__,
				       __TIME__);
			}
			if (strcmp(zwi, "exit") == 0 ||
			    strcmp(zwi, "bye") == 0
			    || strcmp(zwi, "quit") == 0) {
				exit(1);
			}
			if (strncmp(zwi, "monitor ", 8) == 0) {
				f = 0;
				for (t = 0; t < MAXMONITOR; t++) {
					if (monitorvar[t] == NULL) {
						monitorvar[t] =
						    malloc(strlen(zwi + 8) + 1);
						strcpy(monitorvar[t], zwi + 8);
						trim(monitorvar[t]);
						printf("Monitoring %s now.\n",
						       monitorvar[t]);
						monitorcnt++;
						f = 1;
						break;
					}
				}
				if (f == 0) {
					printf
					    ("Variable %s is unknown right now.\n",
					     zwi + 8);
				}
				f = 1;
			}
			if (strncmp(zwi, "unmonitor ", 10) == 0) {
				f = 0;
				for (t = 0; t < MAXMONITOR; t++) {
					if (monitorvar[t] != NULL) {
						if (strcmp
						    (monitorvar[t],
						     zwi + 10) == 0) {
							free(monitorvar[t]);
							monitorvar[t] = NULL;
							printf
							    ("Variable %s is not monitored anymore.\n",
							     zwi + 10);
							f = 1;
							break;
						}
					}
				}
				if (f == 0) {
					printf
					    ("Variable %s is not monitored right now.\n",
					     zwi + 10);
				}
				f = 1;
			}
			if (strcmp(zwi, "clearmonitor") == 0) {
				for (t = 0; t < MAXMONITOR; t++) {
					if (monitorvar[t] != NULL) {
						free(monitorvar[t]);
					}
				}
				monitorcnt = 0;
				puts("All monitors cleared.");
				f = 1;
			}
			if (strncmp(zwi, "break ", 6) == 0) {
				a = atoi(zwi + 6);
				for (t = 0; t < MAXBREAKPOINTS; t++) {
					if (breakpoints[t] == -1) {
						breakpoints[t] = a;
						printf
						    ("Breakpoint set at line %d\n",
						     breakpoints[t]);
						breakcnt++;
						break;
					}
				}
				f = 1;
			}
			if (strcmp(zwi, "breaks") == 0) {
				for (t = 0; t < MAXBREAKPOINTS; t++) {
					if (breakpoints[t] > -1) {
						printf("Line %d\n",
						       breakpoints[t]);
					}
				}
				for (t = 0; t < MAXBREAKIF; t++) {
					if (breakifvariable[t] != NULL) {
						switch (breakifcondition[t]) {
						case 1:
							strcpy(zwi, "<");
							break;
						case 2:
							strcpy(zwi, ">");
							break;
						case 3:
							strcpy(zwi, "=");
							break;
						case 4:
							strcpy(zwi, "!=");
							break;
						default:
							strcpy(zwi, "???");
							break;
						}
						printf("%s %s %ld\n",
						       breakifvariable[t], zwi,
						       breakifvalue[t]);
					}
				}
				f = 1;
			}
			if (strncmp(zwi, "unbreak ", 8) == 0) {
				a = atoi(zwi + 8);
				f = 0;
				for (t = 0; t < breakcnt; t++) {
					if (breakpoints[t] == a) {
						breakpoints[t] = -1;
						printf
						    ("Breakpoint at line %d removed.\n",
						     a);
						f = 1;
						break;
					}
				}
				if (f == 0) {
					printf
					    ("There is no breakpoint at line %d...\n",
					     a);
				}
				else {
					breakcnt = 0;
					for (t = 0; t < MAXBREAKPOINTS; t++) {
						if (breakpoints[t] > -1) {
							breakcnt++;
						}
					}
				}
				f = 1;
			}
			if (strcmp(zwi, "clearbreakif") == 0) {
				for (t = 0; t < breakifcnt; t++) {
					free(breakifvariable[t]);
					breakifvariable[t] = NULL;
					breakifcondition[t] = 0;
					breakifvalue[t] = 0;
				}
				breakifcnt = 0;
				puts("All breakif's cleared.");
				f = 1;
			}
			if (strncmp(zwi, "breakif ", 8) == 0) {
				word(zwi, zwi2, 2);	// numeric variable
				word(zwi, zwi3, 3);	// condition
				word(zwi, zwi4, 4);	// numeric value
				f = 0;
				for (t = 0; t < MAXVARIABLES; t++) {
					if (varname[t] != NULL) {
						if (strUcmp(varname[t], zwi2) ==
						    0) {
							f = 1;
							break;
						}
					}
				}
				if (f == 0) {
					printf("ERROR: Unknown variable >%s<\n",
					       zwi2);
				}
				else {
					if (vartype[t] != '%') {
						f = 0;
						printf
						    ("ERROR: Variable %s is not integer.\n",
						     zwi2);
					}
				}
				if (f == 1) {
					if (breakifvariable[breakifcnt] != NULL) {
						free(breakifvariable
						     [breakifcnt]);
					}
					breakifvariable[breakifcnt] =
					    malloc(strlen(zwi2) + 1);
					strcpy(breakifvariable[breakifcnt],
					       zwi2);
					f = 0;
					if (strcmp(zwi3, "<") == 0) {
						breakifcondition[breakifcnt] =
						    1;
						f = 1;
					}
					if (strcmp(zwi3, ">") == 0) {
						breakifcondition[breakifcnt] =
						    2;
						f = 1;
					}
					if (strcmp(zwi3, "=") == 0) {
						breakifcondition[breakifcnt] =
						    3;
						f = 1;
					}
					if (strcmp(zwi3, "!=") == 0) {
						breakifcondition[breakifcnt] =
						    4;
						f = 1;
					}
					if (f == 0) {
						printf
						    ("ERROR: invalid condition >%s<\n",
						     zwi3);
					}
				}
				if (f == 1) {
					breakifvalue[breakifcnt] = atol(zwi4);
					breakifcnt++;
					puts("Conditional break set.");
				}
				f = 1;
			}
			if (f == 1) {
				break;
			}
			else {
				printf("Unknown command >%s<\n", zwi);
			}
		}
	}
	else {
		debug_done = TRUE;
	}
}

/******************************************************************************/
void show_start_banner(void)
{
	int a;
	int t;
	int z;
	int hh, mm, ss;
	char zwi[256];
	char zwi2[256];
	char machine[256];

	a = time(NULL) - gtim;
	hh = a / 3600;
	a -= (hh * 3600);
	mm = a / 60;
	a -= (mm * 60);
	ss = a;
	programname[20] = 0;
	strcpy(zwi, programname);
	for (t = 0, z = 0; t < 20; t++) {
		programname[z++] = zwi[t];
		programname[z++] = ' ';
	}
	trim(programname);
	z = 58 - strlen(programname);
	z /= 2;
	for (t = 0; t < z; t++) {
		zwi[t] = ' ';
	}
	zwi[t] = 0;
	z = 58 - strlen(programname) - strlen(zwi);
	for (t = 0; t < z; t++) {
		zwi2[t] = ' ';
	}
	zwi2[t] = 0;
	printf
	    ("************************************************************\n");
	printf("* This is %-20s                             *\n", VERSION);
	printf
	    ("*                                                          *\n");
	printf
	    ("*                Program started mother fucker             *\n");
	printf
	    ("*                                                          *\n");
	printf("*%s%s%s*\n", zwi, programname, zwi2);
	s_tm = localtime(&tsec);
	s_tm->tm_year += 1900;
	s_tm->tm_mon++;
	sprintf(zwi, "%04d.%02d.%02d %02d:%02d:%02d",
		s_tm->tm_year, s_tm->tm_mon, s_tm->tm_mday,
		s_tm->tm_hour, s_tm->tm_min, s_tm->tm_sec);
	printf
	    ("*                                                          *\n");
	printf("*                at %s                    *\n", zwi);
	printf
	    ("*                                                          *\n");
	sprintf(machine, "%-20s", getenv("LOLCODEMACHINE"));
	printf("*                  at machine %s         *\n", machine);
	printf
	    ("*                                                          *\n");
	printf
	    ("*                  Fuck your computer. - KF                *\n");
	printf
	    ("************************************************************\n");
}

/******************************************************************************/
void show_terminate_banner(void)
{
	int a;
	int t;
	int z;
	int hh, mm, ss;
	char zwi[256];
	char zwi2[256];
	char zwi3[256];
	char machine[256];
	FILE *fp;

	a = time(NULL) - gtim;
	hh = a / 3600;
	a -= (hh * 3600);
	mm = a / 60;
	a -= (mm * 60);
	ss = a;
	trim(programname);
	z = 58 - strlen(programname);
	z /= 2;
	for (t = 0; t < z; t++) {
		zwi[t] = ' ';
	}
	zwi[t] = 0;
	z = 58 - strlen(programname) - strlen(zwi);
	for (t = 0; t < z; t++) {
		zwi2[t] = ' ';
	}
	zwi2[t] = 0;
	printf
	    ("************************************************************\n");
	printf
	    ("*                                                          *\n");
	printf
	    ("*               Program terminated you bitch               *\n");
	printf
	    ("*                                                          *\n");
	printf("*%s%s%s*\n", zwi, programname, zwi2);
	s_tm = localtime(&tsec);
	s_tm->tm_year += 1900;
	s_tm->tm_mon++;
	sprintf(zwi, "%04d.%02d.%02d %02d:%02d:%02d",
		s_tm->tm_year, s_tm->tm_mon, s_tm->tm_mday,
		s_tm->tm_hour, s_tm->tm_min, s_tm->tm_sec);
	printf
	    ("*                                                          *\n");
	printf("*                at %s                    *\n", zwi);
	printf
	    ("*                                                          *\n");
	sprintf(machine, "%-20s", getenv("LOLCODEMACHINE"));
	printf("*                  at machine %s         *\n", machine);
	printf
	    ("*                                                          *\n");
	printf
	    ("*       Time used %02d:%02d:%02d                                 *\n",
	     hh, mm, ss);
	printf
	    ("*       Errorcode=%02d                                       *\n",
	     gerrorcode);
	if (gerrorcode != 0) {
		printf
		    ("*       Program pointer at error time: %04d                *\n",
		     gerrorpptr);
		switch (gerrorcode) {
		case ERR_NO_ERROR:
			puts("*       Error message: NO ERROR                            *");
			break;
		case ERR_FILE_NOT_FOUND:
			puts("*       Error message: FILE NOT FOUND                      *");
			break;
		default:
			puts("*       Error message: UNKNOWN                             *");
		}
		printf
		    ("*                                                          *\n");
		sprintf(zwi2, "(%06d)", gerrorpptr);
		fp = fopen(listfile, "rb");
		if (fp != NULL) {
			for (;;) {
				if (read_one_line(fp, zwi) == EOF) {
					break;
				}
				if (strstr(zwi, zwi2) != NULL) {
					sprintf(zwi3, "* %s\n", zwi);
					zwi3[56] = 0;
					for (;;) {
						if (zwi3
						    [strlen(zwi3) -
						     1] != '\r'
						    && zwi3[strlen(zwi3)
							    - 1] != '\n') {
							break;
						}
						zwi3[strlen(zwi3) - 1] = 0;
					}
					for (;;) {
						if (strlen(zwi3) > 55)
							break;
						strcat(zwi3, " ");
					}
					strcat(zwi3, "*");
					puts(zwi3);

				}
			}
			fclose(fp);
		}
		else {
			printf
			    ("*       No listing file found!                             *\n");
		}
		printf
		    ("*                                                          *\n");
		puts("*             E R R O R   T E R M I N A T I O N            *");
		puts("*             E R R O R   T E R M I N A T I O N            *");
		puts("*             E R R O R   T E R M I N A T I O N            *");
	}
	printf
	    ("*                                                          *\n");
	printf
	    ("************************************************************\n");
}

/******************************************************************************/
void run_program(char *s)
{
	int t;
	int c;
	char zwi[256];
	char zwi2[256];
	char zwi3[256];
	FILE *fp;

	strcpy(listfile, s);
	for (t = 0;; t++) {
		if (listfile[t] == 0)
			break;
		if (listfile[t] == '.') {
			listfile[t + 1] = 0;
			strcat(listfile, "lst");
			break;
		}
	}

	//printf("run_program: vartype[2048]=>%c<\n", vartype[2048]);
	if (opt_dbg == TRUE) {
		// read the line numbers and corresponding pptr's from the *.lst file
		// for debugging purposes, mainly breakpoints
		// save the source lines too
		for (t = 0; t < MAXDEBUGLINE; t++) {
			debug_line[t] = 0;
			debug_pptr[t] = 0;
			debug_src[t] = NULL;
		}
		debuglcnt = 0;
		fp = fopen(listfile, "rb");
		if (fp != NULL) {
			for (;;) {
				if (read_one_line(fp, zwi) == EOF) {
					break;
				}
				word(zwi, zwi2, 1);
				debug_line[debuglcnt] = atoi(zwi2);
				word(zwi, zwi2, 2);
				if (zwi2[0] == '(') {	// is this really a line of the program source??
					strcpy(zwi3, zwi2 + 1);
					debug_pptr[debuglcnt] = atoi(zwi3);
					debug_src[debuglcnt] =
					    malloc(strlen(zwi) + 1);
					strcpy(debug_src[debuglcnt], zwi);
					debuglcnt++;
				}

			}
			fclose(fp);
		}
		else {
			printf("Could not open list file >%s<...\n", listfile);
		}
		breakcnt = 0;
		for (t = 0; t < MAXMONITOR; t++) {
			monitorvar[t] = NULL;
		}
		monitorcnt = 0;
	}
	for (t = 0; t < MAXBREAKIF; t++) {
		breakifvariable[t] = NULL;
		breakifcondition[t] = 0;
		breakifvalue[t] = 0;
	}
	breakifcnt = 0;
	// clear the local variables (MAXVARIABLES/2), but
	// dont touch the global ones!!
	for (t = 0; t < (MAXVARIABLES / 2); t++) {
		varname[t] = NULL;
		varcontent[t] = 0;
		vartype[t] = ' ';
	}
	for (t = 0; t < MAXBREAKPOINTS; t++) {
		breakpoints[t] = -1;
	}

	gtim = time(NULL);

	gerrorcode = 0;
	gerrorpptr = 0;

	running = FALSE;

	if (opt_trace == TRUE || opt_dbg == TRUE) {
		do_extra_checks = TRUE;
	}
	else {
		do_extra_checks = FALSE;
	}
	// first look for the HAI line 
	if (startbyte == -1) {
		fprintf(stderr, "ERROR 00001 - no HAI statement found.\n");
		exit(1);
	}

	// ok, here the program starts..

	varptr = 0;		// varstack is empty right now
	oldvarptr = 0;		// varstack is empty right now
	locvarptr = 0;		// varstack is empty right now
	gvarcnt = 0;		// varstack is empty right now
	gvarcnt = 0;		// varstack is empty right now
	pptr = startbyte;

	if (opt_nobanners == FALSE) {
		show_start_banner();
	}
	running = TRUE;
	//printf("run_program:2: vartype[2048]=>%c<\n", vartype[2048]);
	if (opt_trace == TRUE || opt_dbg == TRUE) {
		for (; running == TRUE;) {
			//printf("pptr=%d cmd=%d\n", pptr, bytes[pptr]);
			if (opt_trace == TRUE)
				do_trace(pptr);
			if (opt_dbg == TRUE) {
				do_debug(pptr);
			}
			do_execute();
		}
	}
	else {
		for (; running == TRUE;) {
			do_execute();
		}
	}

	if (opt_nobanners == FALSE) {
		show_terminate_banner();
	}
	if (opt_wait_at_end == TRUE) {
		printf("Press ENTER key to terminate...");
		c = fgetc(stdin);
	}
}

/******************************************************************************/
int outcnt = 0;
int passcnt = 0;
void do_output(FILE * fp, int c)
{
	int a;

	if (password[0] != 0) {
		a = password[passcnt];
		if (a == 0) {
			passcnt = 0;
			a = password[passcnt];
		}
		passcnt++;
		c += a;
		if (c > 255) {
			c -= 256;
		}
	}
	if (opt_hex == TRUE) {
		fprintf(fp, "%02X", c);
	}
	else {
		fprintf(fp, "%c", c);
	}
	if (opt_hex == TRUE) {
		outcnt++;
		if (outcnt > 39) {
			fprintf(fp, "\n");
			outcnt = 0;
		}
	}

}

/******************************************************************************/
void create_binary(void)
{
	char name[256];
	char zwi[256];
	FILE *fp;
	int t;
	int a;
	int c;
	int checksum;
	int z;

	passcnt = 0;
	outcnt = 0;
	strcpy(name, fname[0]);
	for (t = strlen(name) - 1; t > 0; t--) {
		if (name[t] == '.') {
			name[t] = 0;
			break;
		}
	}
	if (opt_hex == TRUE) {
		strcat(name, ".lil");
	}
	else {
		strcat(name, ".lul");
	}
	fp = fopen(name, "wb");
	if (fp == NULL) {
		fprintf(stderr, "Couldn't fucking open >%s< for output you douche, aborting this shit...\n",
			listname);
		exit(1);
	}
	setvbuf(fp, NULL, _IOFBF, 32768);
	if (opt_hex == TRUE) {
		sprintf(zwi, "%s%s", MAGICNUMBERHEX, compiledate);	// magic number
	}
	else {
		sprintf(zwi, "%s%s", MAGICNUMBER, compiledate);	// magic number
	}
	for (t = 0;; t++) {
		c = zwi[t];
		if (c == 0)
			break;
		do_output(fp, c);
	}
	a = startbyte;
	c = a & 255;
	do_output(fp, c);
	a = a / 256;
	c = a & 255;
	do_output(fp, c);
	a = a / 256;
	c = a & 255;
	do_output(fp, c);
	a = a / 256;
	c = a & 255;
	do_output(fp, c);
	checksum = 0;
	for (t = 0; t < bytecnt; t++) {
		c = bytes[t] * t;
		checksum = checksum + c;
		checksum = checksum & 65535;
	}
	// printf("checksum=%d\n", checksum);
	a = checksum;
	c = a & 255;
	do_output(fp, c);
	a = a / 256;
	c = a & 255;
	do_output(fp, c);
	/*
	 * the variable types 
	 */
	a = varcnt;
	c = a & 255;
	do_output(fp, c);
	a = a / 256;
	c = a & 255;
	do_output(fp, c);
	for (t = 0; t < varcnt; t++) {
		do_output(fp, vartype[t]);
	}

	z = 0;
	for (t = 0; t < bytecnt; t++) {
		// printf("create_binary: bytes[%d]=%d\n", t, bytes[t]);
		c = bytes[t] + z;	// make it unreadable
		c = c & 255;
		do_output(fp, c);
		z += 13;
		if (z > 255) {
			z = 0;
		}
	}
	fclose(fp);
	if (errorcnt > 0) {
		remove(name);
	}
}

/******************************************************************************/
int hex2int(char *s)
{
	int a;
	int res;
	char hex[17] = "0123456789ABCDEF";
	int t;

	a = 0;
	for (t = 0; t < 16; t++) {
		if (s[0] == hex[t]) {
			a = t;
			break;
		}
	}
	res = a * 16;

	a = 0;
	for (t = 0; t < 16; t++) {
		if (s[1] == hex[t]) {
			a = t;
			break;
		}
	}
	res += a;
	return (res);
}

/******************************************************************************/
int do_input(FILE * fp)
{
	char hex[3];
	int c;
	int a;

	if (hexmode == TRUE) {
		for (;;) {
			c = fgetc(fp);
			if (c == '\r' || c == '\n') {
			}
			else {
				break;
			}
		}
		if (c != EOF) {
			hex[0] = c;
			for (;;) {
				c = fgetc(fp);
				if (c == '\r' || c == '\n') {
				}
				else {
					break;
				}
			}
			hex[1] = c;
			if (c != EOF) {
				hex[2] = 0;
				c = hex2int(hex);
			}
		}
	}
	else {
		c = fgetc(fp);
	}
	if (password[0] != 0 && c != EOF) {
		a = password[passcnt];
		if (a == 0) {
			passcnt = 0;
			a = password[passcnt];
		}
		passcnt++;
		c -= a;
		if (c < 0) {
			c += 256;
		}
	}
	return (c);
}

/******************************************************************************/
void load_binary(char *s)
{
	FILE *fp;
	int t;
	int checksum, checksum2;
	int a;
	int c;
	int z;
	unsigned char zwi[256];
	gvarcnt = 0;		// varstack is empty

	passcnt = 0;
	strcpy(listfile, s);
	for (t = 0;; t++) {
		if (listfile[t] == 0)
			break;
		if (listfile[t] == '.') {
			listfile[t + 1] = 0;
			strcat(listfile, "lst");
			break;
		}
	}
	fp = fopen(s, "rb");
	if (fp == NULL) {
		fprintf(stderr, "Couldn't fucking open >%s< for input you fat fuck, aborting this shit...\n",
			s);
		exit(1);
	}
	setvbuf(fp, NULL, _IOFBF, 32768);
	for (t = 0; t < 8; t++) {
		zwi[t] = do_input(fp);
	}
	zwi[8] = 0;
	if (hexmode == TRUE) {
		if (strcmp(zwi, MAGICNUMBERHEX) != 0) {
			//printf("zwi=>%s< magic=>%s< \n", zwi, MAGICNUMBERHEX);
			fprintf(stderr,
				"This is not a lolcode hexdump >%s<, aborting..\n",
				s);
			exit(2);
		}
	}
	else {
		if (strcmp(zwi, MAGICNUMBER) != 0) {
			fprintf(stderr,
				"This is not a lolcode binary >%s<, aborting..\n",
				s);
			exit(2);
		}
	}

	for (t = 0; t < 19; t++) {	// get the compile date
		compiledate[t] = do_input(fp);
	}
	compiledate[20] = 0;
	for (t = 0; t < 4; t++) {	// get the start position
		zwi[t] = do_input(fp);
	}
	a = zwi[0];
	a = a + (zwi[1] * 256);
	a = a + (zwi[2] * 256 * 256);
	a = a + (zwi[3] * 256 * 256 * 256);
	startbyte = a;
	// printf("startbyte=%d\n", startbyte);

	checksum = do_input(fp);	// get the checksum
	checksum = checksum + (do_input(fp) * 256);

	// read the variable types
	for (t = 0; t < 2; t++) {	// get the varcnt
		zwi[t] = do_input(fp);
	}
	a = zwi[0];
	a = a + (zwi[1] * 256);
	varcnt = a;
	for (t = 0; t < varcnt; t++) {
		vartype[t] = do_input(fp);
		varcontent[t] = 0;
	}

	z = 0;
	checksum2 = 0;
	for (t = 0;; t++) {
		c = do_input(fp);
		if (c == EOF)
			break;
		c = c - z;
		if (c < 0) {
			c = c + 256;
		}
		bytes[t] = c;
		// printf("load_binary: %d) %d\n", t, c);
		// printf("load_binary: bytes[%d]=%d\n", t, c);
		c = bytes[t] * t;
		checksum2 = checksum2 + c;
		checksum2 = checksum2 & 65535;
		z += 13;
		if (z > 255) {
			z = 0;
		}
	}
	bytecnt = t;
	// printf("checksum=%d checksum2=%d\n", checksum, checksum2);
	if (checksum != checksum2) {
		fprintf(stderr,
			"This binary is corrupted, checksum error >%s<, aborting..\n",
			s);
		exit(3);
	}
	fclose(fp);
}

/******************************************************************************/
int main(int argc, char **argv)
{
	char zwi[256];
	char zwi2[256];
	int c;
	int t;
	int f;
	int rc;

	tsec = time(NULL);

	srand(tsec);

	s_tm = localtime(&tsec);
	s_tm->tm_year += 1900;
	s_tm->tm_mon++;

	sprintf(compiledate, "%04d.%02d.%02d %02d:%02d:%02d",
		s_tm->tm_year, s_tm->tm_mon, s_tm->tm_mday,
		s_tm->tm_hour, s_tm->tm_min, s_tm->tm_sec);
	for (t = 0; t < MAXVARIABLES; t++) {
		varname[t] = NULL;
		varcontent[t] = 0;
		vartype[t] = ' ';
	}
	for (t = 0; t < MAXLINES; t++) {
		flptr[t] = NULL;
	}
	for (t = 0; t < MAXRELTAB; t++) {
		reltab[t] = 0;
		relloop[t] = NULL;
	}

	strcpy(fname[fpcnt], "lolcode.lol");
	for (t = 1; t < argc; t++) {
		f = 0;
		if (argv[t][0] != '-') {
			strcpy(fname[fpcnt], argv[t]);
			strcpy(zwi, argv[t]);
			if (zwi[strlen(zwi) - 4] != '.') {
				strcat(fname[fpcnt], ".lol");
			}
			f = 1;
		}
		else {
			if (strcmp(argv[t], "-l") == 0) {
				opt_list = TRUE;
				f = 1;
			}
			if (strcmp(argv[t], "-ll") == 0) {
				opt_list = TRUE;
				opt_long_list = TRUE;
				f = 1;
			}
			if (strcmp(argv[t], "-b") == 0) {
				opt_binary = TRUE;
				opt_hex = FALSE;
				f = 1;
			}
			if (strcmp(argv[t], "-hex") == 0) {
				opt_binary = TRUE;
				opt_hex = TRUE;
				f = 1;
			}
			if (strcmp(argv[t], "-d") == 0) {
				opt_dump = TRUE;
				f = 1;
			}
			if (strcmp(argv[t], "-nobanners") == 0) {
				opt_nobanners = TRUE;
				f = 1;
			}
			if (strcmp(argv[t], "-dbg") == 0) {
				opt_dbg = TRUE;
				opt_list = TRUE;
				f = 1;
			}

			if (strcmp(argv[t], "-wae") == 0) {
				opt_wait_at_end = TRUE;
				f = 1;
			}

			if (strcmp(argv[t], "-t") == 0) {
				opt_trace = TRUE;
				f = 1;
			}
			if (strncmp(argv[t], "-p", 2) == 0) {
				strcpy(password, argv[t] + 2);
				printf("password=>%s<\n", password);
				f = 1;
			}
			if (strcmp(argv[t], "-h") == 0 ||
			    strcmp(argv[t], "--help") == 0 ||
			    strcmp(argv[t], "?") == 0 ||
			    strcmp(argv[t], "v") == 0 ||
			    strcmp(argv[t], "-v") == 0
			    || strcmp(argv[t], "-?") == 0) {
				printf
				    ("%s (%s %s) (C) 2014\n",
				     VERSION, __DATE__, __TIME__);
				exit(0);
			}
		}
		if (f == 0) {
			fprintf(stderr, "What the fuck. I don't know that option >%s<, aborting this shit...\n",
				argv[t]);
			exit(1);
		}
	}

	strcpy(zwi, fname[fpcnt]);
	strcpy(programname, fname[fpcnt]);
	if (zwi[strlen(zwi) - 4] == '.' &&	// start a binary?
	    zwi[strlen(zwi) - 3] == 'l' &&
	    zwi[strlen(zwi) - 2] == 'u' && zwi[strlen(zwi) - 1] == 'l') {
		hexmode = FALSE;
		load_binary(zwi);
		run_program(zwi);
		exit(0);
	}
	if (zwi[strlen(zwi) - 4] == '.' &&	// start a hexdump?
	    zwi[strlen(zwi) - 3] == 'l' &&
	    zwi[strlen(zwi) - 2] == 'i' && zwi[strlen(zwi) - 1] == 'l') {
		hexmode = TRUE;
		load_binary(zwi);
		run_program(zwi);
		exit(0);
	}

	fptab[fpcnt] = fopen(fname[fpcnt], "rb");
	if (fptab[fpcnt] == NULL) {
		fprintf(stderr, "Couldn't fucking open >%s< for input you dumbass, aborting this shit...\n",
			fname[fpcnt]);
		exit(1);
	}
	setvbuf(fptab[fpcnt], NULL, _IOFBF, 32768);
	if (opt_list == TRUE) {
		strcpy(listname, fname[fpcnt]);
		for (t = strlen(listname) - 1; t > 0; t--) {
			if (listname[t] == '.') {
				listname[t] = 0;
				break;
			}
		}
		strcat(listname, ".lst");
		strcpy(listfile, listname);
		fplist = fopen(listname, "wb");
		if (fplist == NULL) {
			fprintf(stderr,
				"Couldn't fucking open >%s< for output you fuck, aborting this shit...\n",
				listname);
			exit(1);
		}
	}

	for (;;) {
		// printf("vor read_one_line() fpcnt=%d\n", fpcnt);
		rc = read_one_line(fptab[fpcnt], zwi);
		if (rc == EOF) {
			if (fpcnt < 1) {
				break;
			}
			fclose(fptab[fpcnt]);
			fpcnt--;
			write_line_to_list("");
			write_line_to_list("");
		}
		sprintf(zwi2, "%08ld (%06d) %s", linecount, bytecnt, zwi);
		write_line_to_list(zwi2);
		remove_comment(zwi);
		trim(zwi);
		flptr[lcnt] = malloc(strlen(zwi) + 1);
		strcpy(flptr[lcnt], zwi);
		parse_command(zwi);
		lcnt++;
	}
	if (errorcnt == 1) {
		sprintf(zwi, "         %d Error.", errorcnt);
	}
	else {
		sprintf(zwi, "         %d Errors.", errorcnt);
	}
	write_line_to_list(zwi);
	if (opt_long_list == TRUE) {
		write_line_to_list("   ");
		write_line_to_list("   ");
		write_line_to_list("   ");
		write_line_to_list
		    ("                              * * * BYTE-DUMP * * *");
		for (t = 0; t < bytecnt; t++) {
			c = bytes[t];
			if (isprint(c) == FALSE) {
				c = '.';
			}
			sprintf(zwi, "%08d [%03d] x'%02X' c[%c]", t, bytes[t],
				bytes[t], c);
			write_line_to_list(zwi);
		}
		write_line_to_list("   ");
		write_line_to_list("   ");
		write_line_to_list("   ");
		write_line_to_list
		    ("                              * * * VARIABLE DUMP * * *");
		for (t = 0; t < varcnt; t++) {
			if (vartype[t] == '%')
				c = '%';
			if (vartype[t] == '$')
				c = '$';
			sprintf(zwi, "%06d %s %c", t, varname[t], c);
			write_line_to_list(zwi);
		}
		fclose(fplist);
	}
	fclose(fptab[fpcnt]);

	if (opt_binary == TRUE) {
		create_binary();
		if (errorcnt > 0) {
			if (errorcnt == 1) {
				printf
				    ("No binary created, existing binary deleted, 1 Error.\n");
			}
			else {
				printf
				    ("No binary created, existing binary deleted, %d Errors.\n",
				     errorcnt);
			}
		}
		else {
			printf("Binary created, no Errors.\n");
		}

	}
	else {
		if (errorcnt == 0) {
			run_program(fname[fpcnt]);
		}
		else {
			printf("%d Errors.\n", errorcnt);
		}
	}
	return (errorcnt);
}
