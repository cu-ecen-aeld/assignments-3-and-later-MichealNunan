/* writer.c
 * Converted from writer.sh
 * Behavior:
 *  - Expects exactly two arguments: <dirAndFile> <text>
 *  - Ensures parent directory exists (mkdir -p behavior)
 *  - Writes the text to the target file (overwrites existing file)
 *  - Verifies the file exists after writing
 *  - Logs errors to both stderr and a logfile (/tmp/writer.log)
 *
 * Build: gcc -std=c11 -Wall -Wextra -o writer writer.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/*#define LOG_PATH "/tmp/writer.log"*/

/* Append an error message to LOG_PATH and also print to stderr. */
/*static void log_error(const char *fmt, ...)
{
    va_list ap;
    FILE *lf = fopen(LOG_PATH, "a");
    if (lf == NULL) {
        va_start(ap, fmt);
        vfprintf(stderr, fmt, ap);
        va_end(ap);
        fprintf(stderr, "\n");
        return;
    }

    va_start(ap, fmt);
    vfprintf(lf, fmt, ap);
    va_end(ap);
    fprintf(lf, "\n");
    fclose(lf);

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, "\n");
}*/

/* Create directories recursively like mkdir -p. Returns 0 on success, -1 on error. */
/* static int mkdir_p(const char *path, mode_t mode)
{
    char *copypath = NULL;
    char *p;
    int status = 0;

    if (path == NULL || *path == '\0') return -1;

    copypath = strdup(path);
    if (!copypath) return -1;

     for (p = copypath + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            if (mkdir(copypath, mode) != 0) {
                if (errno != EEXIST) {
                    status = -1;
                    goto out;
                }
            }
            *p = '/';
        }
    }
    if (mkdir(copypath, mode) != 0) {
        if (errno != EEXIST) {
            status = -1;
            goto out;
        }
    }

out:
    free(copypath);
    return status;
}*/

/* Extract parent directory of a path. Returns a newly allocated string which
 * should be free()d by the caller. If there's no directory component, returns
 * a copy of "." */
/* static char *parent_dir(const char *path)
{
    if (!path) return NULL;
    size_t len = strlen(path);
    if (len == 0) return strdup(".");

    while (len > 1 && path[len - 1] == '/') {
        len--;
    }
    const char *last = NULL;
    for (size_t i = 0; i < len; ++i) {
        if (path[i] == '/') last = path + i;
    }
    if (!last) return strdup(".");
    if (last == path) return strdup("/");

    size_t pdlen = last - path;
    char *pd = malloc(pdlen + 1);
    if (!pd) return NULL;
    memcpy(pd, path, pdlen);
    pd[pdlen] = '\0';
    return pd;
}*/

int main(int argc, char **argv)
{

/*	echo "ERROR. Invalid number of arguments"
	echo "There should be 2 argument" 
	echo " 1)The directory and filename text string" 
	echo " 2)The text to insert in the file"
	exit 1
*/

	openlog(NULL,0,LOG_USER);
    if (argc != 3) {
        /*log_error("ERROR. Invalid number of arguments. There should be 2 arguments: 1) directory+filename 2) text to insert");*/
        syslog(LOG_ERR, "ERROR. Invalid number of arguments. There should be 2 arguments 1) The directory and filename text string 2) The text to insert in the file");
        return 1;
    }

    const char *dirAndFile = argv[1];
    const char *theText = argv[2];

    /* Print the directory and file string to stdout (mimic original script). */
    /* printf(" The directory and file string is: %s\n", dirAndFile); */
    syslog(LOG_DEBUG, "Writing %s to %s,argc", theText, dirAndFile);
    /* Determine parent directory */
   /* char *dirName = parent_dir(dirAndFile);
    if (!dirName) {
        syslog(LOG_ERR, "ERROR: failed to compute parent directory for '%s'", dirAndFile);
        return 1;
    }
*/
    /* Create parent directory (mkdir -p) */
 /*   if (strcmp(dirName, ".") != 0) {
        if (mkdir_p(dirName, 0755) != 0) {
            log_error("ERROR: failed to create directory '%s': %s", dirName, strerror(errno));
            free(dirName);
            return 1;
        }
    }
    free(dirName);
*/
    /* Create / overwrite file with theText */
    FILE *out = fopen(dirAndFile, "w");
    if (!out) {
        /* log_error("ERROR: cannot open file '%s' for writing: %s", dirAndFile, strerror(errno));*/
	syslog(LOG_ERR, "ERROR: cannot open file '%s' for writing: %s", dirAndFile, strerror(errno));
        return 1;
    }

    if (fputs(theText, out) == EOF) {
       /*  log_error("ERROR: failed to write to file '%s': %s", dirAndFile, strerror(errno));*/
	syslog(LOG_ERR, "ERROR: failed to write to file '%s': %s", dirAndFile, strerror(errno));
        fclose(out);
        return 1;
    }

    if (fclose(out) != 0) {
       /* log_error("ERROR: failed to close file '%s' after writing: %s", dirAndFile, strerror(errno));*/
 	syslog(LOG_ERR, "ERROR: failed to close file '%s' after writing: %s", dirAndFile, strerror(errno));
       return 1;
    }

    /* Verify file exists */
    struct stat st;
    if (stat(dirAndFile, &st) != 0) {
        /* log_error("ERROR: The file creation didn't work. '%s' doesn't exist: %s", dirAndFile, strerror(errno));*/
 	syslog(LOG_ERR, "ERROR: failed to close file '%s' after writing: %s", dirAndFile, strerror(errno));
        return 1;
    }

    return 0;
}
