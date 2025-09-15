#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <limits.h>
#include <errno.h>
#include <signal.h>

#include "watch_cmd.h"
#include "build_engine.h"

#define EVENT_BUF_LEN (1024 * (sizeof(struct inotify_event) + NAME_MAX + 1))
static char event_buffer[EVENT_BUF_LEN];
static int inotify_fd;
static int watch_descriptor;

/**
 * @brief Signal handler to gracefully stop the watch process on Ctrl+C.
 */
static void signal_handler(int signum) {
    if (signum == SIGINT) {
        printf("\nReceived SIGINT signal. Stopping watch...\n");
        if (watch_descriptor > 0) {
            inotify_rm_watch(inotify_fd, watch_descriptor);
        }
        if (inotify_fd > 0) {
            close(inotify_fd);
        }
        exit(0);
    }
}

/**
 * @brief The main event loop that listens for filesystem events.
 */
static void run_event_loop(const char *dir_to_watch, const char *config_path) {
    int length, i = 0;
    printf("Monitoring directory '%s'. Press Ctrl+C to stop.\n", dir_to_watch);
    while (1) {
        length = read(inotify_fd, event_buffer, EVENT_BUF_LEN);
        if (length < 0) {
            perror("Error reading inotify events");
            break;
        }
        i = 0;
        while (i < length) {
            struct inotify_event *event = (struct inotify_event *) &event_buffer[i];
            if (event->mask & (IN_MODIFY | IN_CREATE | IN_DELETE)) {
                if (strncmp(event->name, ".", 1) == 0) {
                    i += sizeof(struct inotify_event) + event->len;
                    continue;
                }
                printf("Change detected in '%s'. Initiating build...\n", event->name);
                if (build_project(config_path) != 0) {
                    fprintf(stderr, "Build failed. Resuming watch...\n");
                }
            }
            i += sizeof(struct inotify_event) + event->len;
        }
    }
}

int watch_project(const char *config_path) {
    signal(SIGINT, signal_handler);
    printf("Performing initial build...\n");
    if (build_project(config_path) != 0) {
        fprintf(stderr, "Initial build failed. Cannot start watch mode.\n");
        return 1;
    }
    printf("Initial build completed. Starting watch...\n");
    inotify_fd = inotify_init();
    if (inotify_fd < 0) {
        perror("inotify_init failed");
        return 1;
    }
    watch_descriptor = inotify_add_watch(inotify_fd, "src", IN_MODIFY | IN_CREATE | IN_DELETE);
    if (watch_descriptor < 0) {
        perror("inotify_add_watch failed");
        close(inotify_fd);
        return 1;
    }
    run_event_loop("src", config_path);
    inotify_rm_watch(inotify_fd, watch_descriptor);
    close(inotify_fd);
    return 0;
}