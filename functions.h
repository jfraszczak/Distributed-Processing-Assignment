#ifndef _functions
#define _functions

#include <mpi.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "declarations.h"

void initialize();

void reset_request_data();

void show_process_info();

void set_color();

void send_message(int type, int process_id, int tunnel_id);

void request();

void rest();

void update_lamport_clock(int msg_clock);

void add_to_requests(int process_id);

void add_to_requests_to_queue(int process_id);

void update_ACK_ABSOLUTE();

void update_ACK_EXCLUSION(int tunnel_id);

void update_ACK_CONDITIONAL(int tunnel_id);

void update_ACK_TO_QUEUE(struct message msg);

void update_num_of_processes_on_our_site(struct message msg);

void update_JOIN_QUEUE(int message_sender);

void request_when_not_busy(int message_sender);

void request_when_in_opposite_direction(int message_sender);

void request_when_in_tunnel(int message_sender);

void request_when_in_queue(int message_sender);

void request_when_competition(int message_sender, struct message msg);

void broadcast_RELEASE_QUEUE();

void update_RELEASE_QUEUE(struct message msg, int message_sender);

void enter_tunnel();

void broadcast_RELEASE_TUNNEL();

void update_RELEASE_TUNNEL(struct message msg);

void leave_tunnel();

void handle_message(struct message msg, MPI_Status status);

int queue_to_be_entered();

void wait_to_join_queue();

void join_queue();

void wait_to_enter_tunnel();

void travel();

#endif