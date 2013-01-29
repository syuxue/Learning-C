/* ****************************** Learn Function ****************************** */
void	m_helloworld(void);

/* ****************************** Bash Function ****************************** */
#define M_bash_default		"\e[0m"
#define M_bash_red			"\e[0;31m"
#define M_bash_RED			"\e[1;31m"
#define M_bash_green		"\e[0;32m"
#define M_bash_GREEN		"\e[1;32m"
#define M_bash_yellow		"\e[0;33m"
#define M_bash_YELLOW		"\e[1;33m"
#define M_bash_blue			"\e[0;34m"
#define M_bash_BLUE			"\e[1;34m"
#define M_bash_magenta		"\e[0;35m"
#define M_bash_MAGENTA		"\e[1;35m"
#define M_bash_cyan			"\e[0;36m"
#define M_bash_CYAN			"\e[1;36m"
#define M_bash_white		"\e[0;37m"
#define M_bash_WHITE		"\e[1;37m"
void	m_bash_clear(void);
void	m_bash_pause(const char *message);

/* ****************************** Input/Output Function ****************************** */
int		m_getline(char *str, int maxsize);

/* ****************************** String Function ****************************** */
char	*m_strcpy(char *to, const char *from);
char	*m_strncpy(char *to, const char *from, int n);
char	*m_strcat(char *to, const char *from);
char	*m_strncat(char *to, const char *from, int n);
char	*m_strinsert(char *to, const char *from, int pos);
char	*m_strfield(char *to, const char *from, int key);
char	*m_strrev(char *str);
char	*m_trim(char *str);
char	*m_strtolower(char *str);
char	*m_strtoupper(char *str);
int		m_strlen(const char *str);
int		m_strcmp(const char *to, const char *from);
int		m_strncmp(const char *to, const char *from, int n);
int		m_strcasecmp(const char *to, const char *from);
int		m_strend(const char *str, const char *needle);
int		m_strpos(const char *str, const char *needle);
int		m_strrpos(const char *str, const char *needle);

/* ****************************** Math Function ****************************** */
char	*m_int2str(int dec, char *str, int base, int minwidth, char filler);
int		m_str2int(const char *str, unsigned int base);
double	m_str2float(const char *str, unsigned int base);

/* ****************************** Debug Function ****************************** */
#define M_printd(val)		printf(M_bash_magenta #val M_bash_cyan " = " M_bash_yellow "%d\n" M_bash_default, val)
#define M_printu(val)		printf(M_bash_magenta #val M_bash_cyan " = " M_bash_yellow "%u\n" M_bash_default, val)
#define M_prints(val)		printf(M_bash_magenta #val M_bash_cyan " = " M_bash_yellow "%s\n" M_bash_default, val)
#define M_printf(val)		printf(M_bash_magenta #val M_bash_cyan " = " M_bash_yellow "%g\n" M_bash_default, val)
#define M_showvariable(val) printf(M_bash_magenta #val M_bash_cyan " : ");m_showvariable(&val, sizeof val);
void	m_showvariable(const void *val, size_t size);

/* ****************************** [Experimental] ****************************** */

/* ****************************** [Deprecated] ****************************** */
char	*m_alloc(int n);
void	m_afree(char *p);
