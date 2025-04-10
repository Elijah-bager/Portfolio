
/* this file is mandatory so we can run 3 ways:
 * GRAHPICS set to 1 - supresses ALL text output, draws instead.
 * GRAPHICS set to 0 - text mode, VERBOSE controls what prints.
 * TEXT has 2 modes: 
 * VERBOSE to 1 to get DEBUG output 
 * VERBOSE to 0 to get only the required text output
 * Do not set TEXT or DEBUG directly; change GRAHPICS and VERBOSE instead.
 */


#define VERBOSE 0	/* 1: all messages, 0: only required mesages */
#define TEXT (! GRAPHICS)	/* do not change */
#define GRAPHICS 0	/* 1 for graphics and no text, 0 for text only */

#define DEBUG (VERBOSE && TEXT)	/* do not change this */

#define BONUS 0	/* 1 for bonus mode, 0 for regular mode */
