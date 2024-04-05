#ifdef __GNUC__
#define NORETURN __attribute__ ((__noreturn__))
#else
#define NORETURN
#endif

void err_msg(const char *format, ...);
void err_exit(const char *format, ...) NORETURN;
void err_exit_noflush(const char *format, ...) NORETURN;
void err_exit_num(int errnum, const char *format, ...) NORETURN;
void fatal(const char *format, ...) NORETURN;
void usage_err(const char *format, ...) NORETURN;
void cmd_line_err(const char *format, ...) NORETURN;
