#ifndef SERVER_H
#define SERVER_H

int server_init(void);
void server_shutdown(void);
void server_spawnservice_feedback(int success, char *message);

#endif