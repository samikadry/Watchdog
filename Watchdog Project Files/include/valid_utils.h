#ifndef __VALIDATION_UTILS_H__
#define __VALIDATION_UTILS_H__

#include <stdio.h>  /* fputs, stderr, perror */

/* Definition of DEBUG_ONLY macro */                             
#ifndef NDEBUG 
#define DEBUG_ONLY(CODE) do{ CODE } while(0)                   
#else
#define DEBUG_ONLY(CODE)
#endif  


/* Definition of UNUSED_NO_WARN macro */
#ifdef __GNUC__
#define UNUSED_NO_WARN __attribute__((unused)) 
#elif defined __CLANG__
#define UNUSED_NO_WARN __attribute__((unused))
#else  /* #if defined __GNUC__ */
#warning if nececery on your platform, add support for disabling warning 
"unused static function" here
#define UNUSED_NO_WARN
#endif  /* #else */


/* Definition of unused variables */
#define UNUSED(VAR) ((void)VAR)

/* Print error msg and returns appropriate value when happened a bad thing */
#define RETURN_IF_BAD(IS_GOOD, MSG, RET_VAL) \
if (!(IS_GOOD))         \
{                       \
    fputs(MSG, stderr); \
    return (RET_VAL);   \
}                    

/* Cleanup resources, print error msg and returns with error value if happened
   a bad thing */
#define CLEAN_AND_RETURN_IF_BAD(IS_GOOD, MSG, RET_VAL, CLEANUP_FUNC) \
if (!(IS_GOOD))         \
{                       \
    CLEANUP_FUNC();     \
    fputs(MSG, stderr); \
    return (RET_VAL);   \
}

/* Print error msg and exit with appropriate value when happened a bad thing */
UNUSED_NO_WARN 
static void ExitIfBad(int is_good, char *msg, int exit_val)
{
    if (!(is_good))
    {
        fputs(msg, stderr);
        exit(exit_val);
    }
}

/* Like ExitIfBad but print with perror */
UNUSED_NO_WARN 
static void ExitIfBadPerror(int is_good, char *msg, int exit_val)
{
    if (!(is_good))
    {
        perror(msg);
        exit(exit_val);
    }
}

/* Print an error msg when happened a bad thing */
UNUSED_NO_WARN 
static void PrintIfBad(int is_good, char *msg)
{
    if (!(is_good))
    {
        fputs(msg, stderr);
    }
}

#endif /* __VALIDATION_UTILS_H__ */
