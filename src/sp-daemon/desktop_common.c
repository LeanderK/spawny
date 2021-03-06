#include "main.h"

#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <libgen.h>
#include <ini.h>
typedef struct {
    char *name;
    char *exec;
    char *icon;
} Session;

#define EXT ".desktop"

static int
_parse_file_handler(void* user, const char* section,
              const char* name, const char* value) {
    Session *session = user;
    if (!strcmp(name, "Name")) {
        session->name = strdup(value);
    } else if (!strcmp(name, "Exec")) {
        session->exec = strdup(value);
    } else if (!strcmp(name, "Icon")) {
        session->icon = strdup(value);
    } else {
        //printf("Parsing does not understand [%s] %s=%s\n", section, name, value);
        return 1;
    }
    return 1;
}

void
_parse_file(const char *file, const char *type, Template_Fire_Up fire_up) {
    Session session = { NULL };

    if (!parse_ini_verbose(file, _parse_file_handler, &session)) {
        ERR("Failed to parse %s", file);
        return;
    }

    if (!session.name) {
        ERR("Session name not found %s", file);
        goto not_register;
    }

    if (!session.exec) {
        ERR("Session exec not found %s", session.name);
        goto not_register;
    }

    if (!session.icon) {
        INF("Session icon not set for %s.", session.name);
    }

    template_register(session.name, session.icon, type, fire_up, session.exec);

    return;
not_register:
    if (session.name) free(session.name);
    if (session.exec) free(session.exec);
    if (session.icon) free(session.icon);
}

void
parse_dir(const char *directory_path, const char *type, Template_Fire_Up fire_up) {
    DIR *directory;
    struct dirent *dir;

    directory = opendir(directory_path);

    if (!directory) return;

    while((dir = readdir(directory))) {
        char path[PATH_MAX];
        int len;

        len = strlen(dir->d_name);

        if (!strcmp(dir->d_name + len - sizeof(EXT) + 1, EXT)) {
            snprintf(path, sizeof(path), "%s/%s", directory_path, dir->d_name);
            _parse_file(path, type, fire_up);
        }
    }

    closedir(directory);
}

/**
 * 0 on fail
 * 1 on success
 */
int
parse_ini_verbose(const char* filename, ini_handler handler, void* user) {
    int ret;

    ret = ini_parse(filename, handler, user);

    if (!ret) return 1; /* yey no error */

    if (ret == -1) {
        ERR("Failed to open %s", filename);
    } else if (ret == -2) {
        ERR("Failed to allocate memory on parsing %s.", filename);
    } else {
        ERR("Failed to parse %s. Error on line %d", filename, ret);
    }
    return 0;
}
