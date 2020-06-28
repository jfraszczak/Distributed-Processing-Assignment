#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "functions.h"

void mutex_initialize(){
    if (pthread_mutex_init(&lock_lamport, NULL) != 0)
    {
        printf("\n mutex init failed\n");
    }

     if (pthread_mutex_init(&lock_leaders_of_queues, NULL) != 0)
    {
        printf("\n mutex init failed\n");
    }

     if (pthread_mutex_init(&lock_requests, NULL) != 0)
    {
        printf("\n mutex init failed\n");
    }

     if (pthread_mutex_init(&lock_requests_to_queue, NULL) != 0)
    {
        printf("\n mutex init failed\n");
    }

     if (pthread_mutex_init(&lock_tunnels_ack, NULL) != 0)
    {
        printf("\n mutex init failed\n");
    }

     if (pthread_mutex_init(&lock_ack_from_processes, NULL) != 0)
    {
        printf("\n mutex init failed\n");
    }

     if (pthread_mutex_init(&lock_num_of_processes_on_our_site, NULL) != 0)
    {
        printf("\n mutex init failed\n");
    }

     if (pthread_mutex_init(&lock_queues_ack, NULL) != 0)
    {
        printf("\n mutex init failed\n");
    }

     if (pthread_mutex_init(&lock_processes_in_queue, NULL) != 0)
    {
        printf("\n mutex init failed\n");
    }

     if (pthread_mutex_init(&lock_num_of_processes_in_queue, NULL) != 0)
    {
        printf("\n mutex init failed\n");
    }

     if (pthread_mutex_init(&lock_state, NULL) != 0)
    {
        printf("\n mutex init failed\n");
    }

     if (pthread_mutex_init(&lock_tunnels_conditional_ack, NULL) != 0)
    {
        printf("\n mutex init failed\n");
    }

    if (pthread_mutex_init(&lock_location, NULL) != 0)
    {
        printf("\n mutex init failed\n");
    }

    if (pthread_mutex_init(&lock_everything, NULL) != 0)
    {
        printf("\n mutex init failed\n");
    }
}

void initialize(){
    requests = malloc(mpi_process_data->size * sizeof(int));
    ack_from_processes = malloc(mpi_process_data->size * sizeof(int));
    tunnels_ack = malloc(sizeof(int));
    tunnels_conditional_ack = malloc(num_of_tunnels * sizeof(int));
    queues_ack = malloc(num_of_tunnels * sizeof(int));
    leaders_of_queues = malloc(num_of_tunnels * sizeof(int));
    processes_in_queue = malloc(mpi_process_data->size * sizeof(int));
    requests_to_queue = malloc(mpi_process_data->size * sizeof(int));
    request_clocks =  malloc(mpi_process_data->size * sizeof(int));
    releases = malloc(num_of_tunnels * sizeof(int));

    for(int i = 0; i < num_of_tunnels; i++){
        tunnels_ack[i] = 0;
    }

    for(int i = 0; i < num_of_tunnels; i++){
        queues_ack[i] = 0;
    }

    for(int i = 0; i < num_of_tunnels; i++){
        tunnels_conditional_ack[i] = 0;
    }

    for(int i = 0; i < mpi_process_data->size; i++){
        requests[i] = 0;
    }

    for(int i = 0; i < mpi_process_data->size; i++){
        ack_from_processes[i] = 0;
    }

    for(int i = 0; i < num_of_tunnels; i++){
        leaders_of_queues[i] = -1;
    }

    for(int i = 0; i < mpi_process_data->size; i++){
        processes_in_queue[i] = 0;
    }

    for(int i = 0; i < mpi_process_data->size; i++){
        requests_to_queue[i] = 0;
    }

    for(int i = 0; i < mpi_process_data->size; i++){
        request_clocks[i] = 0;
    }

    for(int i = 0; i < num_of_tunnels; i++){
        releases[i] = 0;
    }

    if(mpi_process_data->tid % 2 == 0){
      state = IN_ANOTHER_DIMENSION;
      location = IN_ANOTHER_DIMENSION;
   }

   state = IN_ANOTHER_DIMENSION;
   location = IN_ANOTHER_DIMENSION;

   if(tunnel_capacity < rich_group_capacity){
      rich_group_capacity = tunnel_capacity;
   }

}

void reset_request_data(){
    pthread_mutex_lock(&lock_tunnels_ack);
    for(int i = 0; i < num_of_tunnels; i++){
        tunnels_ack[i] = 0;
    }
    pthread_mutex_unlock(&lock_tunnels_ack);

    pthread_mutex_lock(&lock_queues_ack);
    for(int i = 0; i < num_of_tunnels; i++){
        queues_ack[i] = 0;
    }
    pthread_mutex_unlock(&lock_queues_ack);

    pthread_mutex_lock(&lock_tunnels_conditional_ack);
    for(int i = 0; i < num_of_tunnels; i++){
        tunnels_conditional_ack[i] = 0;
    }
    pthread_mutex_unlock(&lock_tunnels_conditional_ack);

    pthread_mutex_lock(&lock_requests);
    for(int i = 0; i < mpi_process_data->size; i++){
        requests[i] = 0;
    }
    pthread_mutex_unlock(&lock_requests);

    pthread_mutex_lock(&lock_ack_from_processes);
    for(int i = 0; i < mpi_process_data->size; i++){
        ack_from_processes[i] = 0;
    }
    pthread_mutex_unlock(&lock_ack_from_processes);

    pthread_mutex_lock(&lock_leaders_of_queues);
    for(int i = 0; i < num_of_tunnels; i++){
        leaders_of_queues[i] = -1;
    }
    pthread_mutex_unlock(&lock_leaders_of_queues);

    pthread_mutex_lock(&lock_processes_in_queue);
    for(int i = 0; i < mpi_process_data->size; i++){
        processes_in_queue[i] = 0;
    }
    pthread_mutex_unlock(&lock_processes_in_queue);

    pthread_mutex_lock(&lock_requests_to_queue);
    for(int i = 0; i < mpi_process_data->size; i++){
        requests_to_queue[i] = 0;
    }
    pthread_mutex_unlock(&lock_requests_to_queue);

    for(int i = 0; i < mpi_process_data->size; i++){
        request_clocks[i] = 0;
    }

    for(int i = 0; i < num_of_tunnels; i++){
        releases[i] = 0;
    }

    request_clock = -1;
    is_leader = 0;
    in_tunnel_id = -1;

    num_of_processes_in_tunnel = 0;
    
    pthread_mutex_lock(&lock_num_of_processes_in_queue);
    num_of_processes_in_queue = 0;
    pthread_mutex_unlock(&lock_num_of_processes_in_queue);

    pthread_mutex_lock(&lock_num_of_processes_on_our_site);
    num_of_processes_on_our_site = 0;
    pthread_mutex_unlock(&lock_num_of_processes_on_our_site);
    
}

void reset_state(){
    state = location;
}

void show_process_info(){
    printf("\nID: %d\nState: %d\nLocation: %d\nLamport clock: %d\nRequest clock: %d\nLeader: %d\nTunnel ID: %d\nNum of processes in queue: %d\n", mpi_process_data->tid, state, location, lamport_clock, request_clock, is_leader, in_tunnel_id, num_of_processes_in_queue);
    printf("TUNNELS ACK\n");
    for(int i = 0; i < num_of_tunnels; i++){
        printf("%d ", tunnels_ack[i]);
    }
    printf("\n");
    printf("QUEUES ACK\n");
    for(int i = 0; i < num_of_tunnels; i++){
        printf("%d ", queues_ack[i]);
    }
    printf("\n");
    printf("TUNNELS CONDITIONAL ACK\n");
    for(int i = 0; i < num_of_tunnels; i++){
        printf("%d ", tunnels_conditional_ack[i]);
    }
    printf("\n");

    printf("REQUESTS\n");
    for(int i = 0; i < mpi_process_data->size; i++){
        printf("%d ", requests[i]);
    }
    printf("\n");

    printf("PROCESSES IN QUEUE\n");
    for(int i = 0; i < mpi_process_data->size; i++){
        printf("%d ", processes_in_queue[i]);
    }
    printf("\n");

    printf("ACK FROM PROCESSES\n");
    for(int i = 0; i < mpi_process_data->size; i++){
        printf("%d ", ack_from_processes[i]);
    }
    printf("\n");

    printf("REQUEST CLOCKS\n");
        for(int i = 0; i < mpi_process_data->size; i++){
         printf("%d ", request_clocks[i]);
    }
    printf("\n");

    printf("LEADERS\n");
    for(int i = 0; i < num_of_tunnels; i++){
        printf("%d ", leaders_of_queues[i]);
    }
    printf("\n");

    printf("RELEASES\n");
    for(int i = 0; i < num_of_tunnels; i++){
        printf("%d ", releases[i]);
    }
    printf("\n");

}

void set_color(){
    if(mpi_process_data->tid == 0){
        printf("\033[1;36m");
    }
    if(mpi_process_data->tid == 1){   
        printf("\033[1;35m");
    }   
    if(mpi_process_data->tid == 2){
        printf("\033[1;34m");
    }
    if(mpi_process_data->tid == 3){
        printf("\033[1;33m");
    }
    if(mpi_process_data->tid == 4){
        printf("\033[1;32m");
    }
    if(mpi_process_data->tid == 5){
        printf("\033[1;31m");
    }      
}


void send_message(int type, int process_id, int tunnel_id){
    struct message msg;
    msg.type = type;
    msg.tunnel_id = tunnel_id;
    msg.lamport_clock = request_clock;
    msg.is_leader = is_leader;
    msg.state = state;
    msg.location = location;
    msg.answer_clock = request_clocks[process_id];
    MPI_Send(&msg, sizeof(struct message), MPI_BYTE, process_id, type, MPI_COMM_WORLD);
}

void request(){
    lamport_clock++;
    request_clock = lamport_clock;
    int msg_type;
    if(location == ON_EARTH)
        msg_type = REQ_TO_ANOTHER_DIMENSION;
    else if(location == IN_ANOTHER_DIMENSION)
        msg_type = REQ_TO_EARTH;

    for(int id = 0; id < mpi_process_data->size; id++){
        if(id != mpi_process_data->tid){
            send_message(msg_type, id, -1);
        }
    }
   //printf("WYSYLAM REQ ID: %d LAMPORT: %d\n", mpi_process_data->tid, lamport_clock);
}

void rest(){
    reset_request_data();
    set_color();
    if(location == ON_EARTH)
        printf("JESTEM NA ZIEMI ID: %d LAMPORT: %d\n", mpi_process_data->tid, lamport_clock);
    else if(location == IN_ANOTHER_DIMENSION)
        printf("JESTEM W INNYM WYMIARZE ID: %d LAMPORT: %d\n", mpi_process_data->tid, lamport_clock);    

    set_color();
    printf("ZMIANA STANU NA WAIT FOR QUEUE ID: %d LAMPORT: %d\n", mpi_process_data->tid, lamport_clock);
    state = WAIT_FOR_QUEUE;
}

void update_lamport_clock(int msg_clock){
    int max_clock;
    pthread_mutex_lock(&lock_lamport);
    if(msg_clock > lamport_clock)
        max_clock = msg_clock;
    else
        max_clock = lamport_clock;
    lamport_clock = max_clock + 1;
    pthread_mutex_unlock(&lock_lamport);
}

void add_to_requests(int process_id){
    pthread_mutex_lock(&lock_requests);
    requests[process_id] = 1;
    pthread_mutex_unlock(&lock_requests);
}

void add_to_requests_to_queue(int process_id){
    pthread_mutex_lock(&lock_requests_to_queue);
    requests_to_queue[process_id] = 1;
    pthread_mutex_unlock(&lock_requests_to_queue);
}

void update_ACK_ABSOLUTE(){
    pthread_mutex_lock(&lock_tunnels_ack);
    for(int i = 0; i < num_of_tunnels; i++){
        tunnels_ack[i]++;
    }
    pthread_mutex_unlock(&lock_tunnels_ack);
}

void update_ACK_EXCLUSION(int tunnel_id){
    pthread_mutex_lock(&lock_tunnels_ack);
    for(int i = 0; i < num_of_tunnels; i++){
        if(i != tunnel_id){
            tunnels_ack[i]++;
        }
    }
    pthread_mutex_unlock(&lock_tunnels_ack);
}

void update_ACK_CONDITIONAL(int tunnel_id) {
    pthread_mutex_lock(&lock_tunnels_conditional_ack);
    tunnels_conditional_ack[tunnel_id]++;
    pthread_mutex_unlock(&lock_tunnels_conditional_ack);
    pthread_mutex_lock(&lock_tunnels_ack);
    for(int i = 0; i < num_of_tunnels; i++){
        tunnels_ack[i]++;
    }
    pthread_mutex_unlock(&lock_tunnels_ack);
}

void update_ACK_TO_QUEUE(struct message msg) {
    if(msg.location == location){
        pthread_mutex_lock(&lock_queues_ack);
        queues_ack[msg.tunnel_id]++;
        pthread_mutex_unlock(&lock_queues_ack);

        pthread_mutex_lock(&lock_tunnels_ack);
        for(int i = 0; i < num_of_tunnels; i++){
            tunnels_ack[i]++;
        }
        pthread_mutex_unlock(&lock_tunnels_ack);
    }
    else{
        pthread_mutex_lock(&lock_tunnels_ack);
        for(int i = 0; i < num_of_tunnels; i++){
            if(i != msg.tunnel_id){
                tunnels_ack[i]++;
            }
        }
        pthread_mutex_unlock(&lock_tunnels_ack);
    }
}

void update_RELEASE(struct message msg) {
    if(msg.location == location){
        pthread_mutex_lock(&lock_queues_ack);
        queues_ack[msg.tunnel_id]++;
        pthread_mutex_unlock(&lock_queues_ack);

        pthread_mutex_lock(&lock_tunnels_ack);
        for(int i = 0; i < num_of_tunnels; i++){
            tunnels_ack[i]++;
        }
        pthread_mutex_unlock(&lock_tunnels_ack);
    }
    else{
        pthread_mutex_lock(&lock_tunnels_ack);
        for(int i = 0; i < num_of_tunnels; i++){
            if(i != msg.tunnel_id){
                tunnels_ack[i]++;
            }
        }
        pthread_mutex_unlock(&lock_tunnels_ack);
    }
}

void update_num_of_processes_on_our_site(struct message msg){
    pthread_mutex_lock(&lock_num_of_processes_on_our_site);
    if(msg.type != RELEASE_TUNNEL){
        if(location == msg.location && (msg.state == location || msg.state == WAIT_FOR_QUEUE)){
            num_of_processes_on_our_site++;
        }
    }
    else if(msg.location != location){
        num_of_processes_on_our_site++;
    }
    pthread_mutex_unlock(&lock_num_of_processes_on_our_site);
}

void update_JOIN_QUEUE(int message_sender){
    pthread_mutex_lock(&lock_processes_in_queue);
    pthread_mutex_lock(&lock_num_of_processes_in_queue);
    processes_in_queue[message_sender] = 1;
    num_of_processes_in_queue++;
    pthread_mutex_unlock(&lock_num_of_processes_in_queue);
    pthread_mutex_unlock(&lock_processes_in_queue);

    //printf("ID %d do rodzinki dolaczyl proces %d i mamy obecnie %d procesow\n", mpi_process_data->tid, message_sender, num_of_processes_in_queue);
    
}

void request_when_not_busy(int message_sender, struct message msg){
    //set_color();
    //printf("WYSYLAM ACK ABSOLUTE ID: %d LAMPORT: %d\n", mpi_process_data->tid, lamport_clock);
    pthread_mutex_lock(&lock_lamport);
    lamport_clock++;
    request_clocks[message_sender] = msg.lamport_clock;
    send_message(ACK_ABSOLUTE, message_sender, -1);
    pthread_mutex_unlock(&lock_lamport);
}

void request_when_in_opposite_direction(int message_sender, struct message msg){
    //set_color();
    //printf("WYSYŁAM ACK_EXCLUSION BO TUNEL O ID %d PROWADZI W PRZYECIWNYM KIERUNKU LAMPORT: %d\n", in_tunnel_id, lamport_clock);
    pthread_mutex_lock(&lock_lamport);
    lamport_clock++;
    request_clocks[message_sender] = msg.lamport_clock;
    send_message(ACK_EXCLUSION, message_sender, in_tunnel_id);
    add_to_requests_to_queue(message_sender);
    pthread_mutex_unlock(&lock_lamport);
}

void request_when_in_tunnel(int message_sender, struct message msg){
    //set_color();
    //printf("WYSYŁAM ACK_CONDITIONAL DO %d ID: %d\n", message_sender, mpi_process_data->tid);
    pthread_mutex_lock(&lock_lamport);
    lamport_clock++;
    request_clocks[message_sender] = msg.lamport_clock;
    send_message(ACK_CONDITIONAL, message_sender, in_tunnel_id);
    add_to_requests_to_queue(message_sender);
    pthread_mutex_unlock(&lock_lamport);
}

void request_when_in_queue(int message_sender, struct message msg){
    //set_color();
    //printf("WYSYLAM ACK_TO_QUEUE DO %d ID: %d\n", message_sender, mpi_process_data->tid);
    pthread_mutex_lock(&lock_lamport);
    lamport_clock++;
    request_clocks[message_sender] = msg.lamport_clock;
    send_message(ACK_TO_QUEUE, message_sender, in_tunnel_id);
    add_to_requests_to_queue(message_sender);
    pthread_mutex_unlock(&lock_lamport);
}

void request_when_competition(int message_sender, struct message msg){
    if(request_clock < msg.lamport_clock || (request_clock == msg.lamport_clock && mpi_process_data->tid < message_sender)){
        add_to_requests(message_sender);
        add_to_requests_to_queue(message_sender);
        request_clocks[message_sender] = msg.lamport_clock;
    }
    else{
        pthread_mutex_lock(&lock_lamport);
        lamport_clock++;
        request_clocks[message_sender] = msg.lamport_clock;
        send_message(ACK_ABSOLUTE, message_sender, in_tunnel_id);
        pthread_mutex_unlock(&lock_lamport);
    }
}

void broadcast_RELEASE_QUEUE(){
    //set_color();  
    //printf("OPUSZCZAM KOLEJKE numer: %d ID: %d LAMPORT: %d\n", in_tunnel_id, mpi_process_data->tid, lamport_clock);
    pthread_mutex_lock(&lock_lamport);
    lamport_clock++;

    for(int i = 0; i < mpi_process_data->size; i++){
        if(requests_to_queue[i] == 1 && processes_in_queue[i] == 0){
            //printf("WYSYLAM RELEASE QUEUE DO %d ID: %d\n", i, mpi_process_data->tid);
            send_message(RELEASE_QUEUE, i, in_tunnel_id);
        }
    }
    pthread_mutex_unlock(&lock_lamport);
}

void update_RELEASE_QUEUE(struct message msg, int message_sender){
    if(msg.location == location){
        if(msg.is_leader == 1){
            pthread_mutex_lock(&lock_leaders_of_queues);
            leaders_of_queues[msg.tunnel_id] = -1;
            pthread_mutex_unlock(&lock_leaders_of_queues);
        }
        pthread_mutex_lock(&lock_queues_ack);
        queues_ack[msg.tunnel_id] -= msg.num_of_processes;
        pthread_mutex_unlock(&lock_queues_ack);

        pthread_mutex_lock(&lock_tunnels_conditional_ack);
        tunnels_conditional_ack[msg.tunnel_id] += msg.num_of_processes;
        pthread_mutex_unlock(&lock_tunnels_conditional_ack);
    }
    //show_process_info();
}

void enter_tunnel(){
    set_color();
    printf("WCHODZE DO TUNELU numer %d ID: %d LAMPORT: %d\n", in_tunnel_id, mpi_process_data->tid, lamport_clock);
    state = IN_TUNNEL;
}

void wait_for_processes(){
    if(is_leader == 1){
        while(num_of_processes_in_tunnel < rich_group_capacity - 1 && num_of_processes_in_tunnel < num_of_processes_on_our_site){
            handle_message();
        }
        set_color();
        printf("JESTEM LIDEREM ID %d I WCHODZE DO TUNELU %d LAMPORT: %d\n", mpi_process_data->tid, in_tunnel_id, lamport_clock);
        enter_tunnel();
        broadcast_RELEASE_QUEUE();
    }

}

void update_JOIN_IN_TUNNEL(){
    num_of_processes_in_tunnel++;
}



void broadcast_RELEASE_TUNNEL() {
    pthread_mutex_lock(&lock_lamport);
    lamport_clock++;

    for(int i = 0; i < mpi_process_data->size; i++){
        if(requests_to_queue[i] == 1 && processes_in_queue[i] == 0){
            //printf("WYSYLAM RELEASE TUNNEL DO %d ID: %d\n", i, mpi_process_data->tid);
            send_message(RELEASE_TUNNEL, i, in_tunnel_id);
        }
    }

    if(is_leader == 1){
        for(int i = 0; i < mpi_process_data->size; i++){
            if(processes_in_queue[i] == 1){
                //printf("WYSYLAM LEAVE TUNNEL DO %d ID: %d\n", i, mpi_process_data->tid);
                send_message(LEAVE_TUNNEL, i, in_tunnel_id);
            }
        }
    }
    pthread_mutex_unlock(&lock_lamport);
}


void update_RELEASE_TUNNEL(struct message msg){
    if(msg.location != location){
        pthread_mutex_lock(&lock_tunnels_ack);
        tunnels_ack[msg.tunnel_id]++;
        pthread_mutex_unlock(&lock_tunnels_ack);
    }
    else{
        pthread_mutex_lock(&lock_tunnels_conditional_ack);
        tunnels_conditional_ack[msg.tunnel_id]--;
        pthread_mutex_unlock(&lock_tunnels_conditional_ack);
    }
    //show_process_info();
}


void leave_tunnel(){
    set_color();
    printf("OPUSZCZAM TUNEL numer: %d ID: %d LAMPORT: %d\n", in_tunnel_id, mpi_process_data->tid, lamport_clock);
    broadcast_RELEASE_TUNNEL();
    pthread_mutex_lock(&lock_location);
    pthread_mutex_lock(&lock_state);
    if(location == ON_EARTH){
        state = IN_ANOTHER_DIMENSION;
        location = IN_ANOTHER_DIMENSION;
    }
    else if(location == IN_ANOTHER_DIMENSION) {
        state = ON_EARTH;
        location = ON_EARTH;
    }
    pthread_mutex_unlock(&lock_state);
    pthread_mutex_unlock(&lock_location);
    //printf("SIEMA ID: %d STATE: %d\n", mpi_process_data->tid, state);
}

struct message handle_message(){

    MPI_Status status;
    struct message msg;

    MPI_Recv(&msg, sizeof(struct message), MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    pthread_mutex_lock(&lock_everything);

    int message_sender = status.MPI_SOURCE;
    update_lamport_clock(msg.lamport_clock);


    if(msg.is_leader == 1 && msg.state == IN_QUEUE && msg.location == location){
        pthread_mutex_lock(&lock_leaders_of_queues);
        leaders_of_queues[msg.tunnel_id] = message_sender;
        pthread_mutex_unlock(&lock_leaders_of_queues);
    }

    if(msg.type == REQ_TO_ANOTHER_DIMENSION || msg.type == REQ_TO_EARTH){
        if(state == ON_EARTH || state == IN_ANOTHER_DIMENSION){
            request_when_not_busy(message_sender, msg);
        }
        else if((state == IN_TUNNEL || state == IN_QUEUE) && ((location == ON_EARTH && msg.type == REQ_TO_EARTH) || (location == IN_ANOTHER_DIMENSION && msg.type == REQ_TO_ANOTHER_DIMENSION))){
            request_when_in_opposite_direction(message_sender, msg);
        }
        else if(state == IN_TUNNEL){
            request_when_in_tunnel(message_sender, msg);
        }
        else if(state == IN_QUEUE){
            request_when_in_queue(message_sender, msg);
        }
        else if(state == WAIT_FOR_QUEUE){
            request_when_competition(message_sender, msg);
        }

    }
    else if(msg.type == ACK_ABSOLUTE && state == WAIT_FOR_QUEUE){
        if(ack_from_processes[message_sender] == 0){
            update_ACK_ABSOLUTE();
            pthread_mutex_lock(&lock_ack_from_processes);
            ack_from_processes[message_sender] = 1;
            pthread_mutex_unlock(&lock_ack_from_processes);
            update_num_of_processes_on_our_site(msg);
        }
    }
    else if(msg.type == ACK_EXCLUSION && state == WAIT_FOR_QUEUE){
        update_ACK_EXCLUSION(msg.tunnel_id);
        pthread_mutex_lock(&lock_ack_from_processes);
        ack_from_processes[message_sender] = 1;
        pthread_mutex_unlock(&lock_ack_from_processes);
    }
    else if(msg.type == ACK_CONDITIONAL && state == WAIT_FOR_QUEUE){
        update_ACK_CONDITIONAL(msg.tunnel_id);
        pthread_mutex_lock(&lock_ack_from_processes);
        ack_from_processes[message_sender] = 1;
        pthread_mutex_unlock(&lock_ack_from_processes);
    } 
    else if(msg.type == ACK_TO_QUEUE && state == WAIT_FOR_QUEUE && msg.answer_clock == request_clock){
        update_ACK_TO_QUEUE(msg);
        pthread_mutex_lock(&lock_ack_from_processes);
        ack_from_processes[message_sender] = 1;
        pthread_mutex_unlock(&lock_ack_from_processes);
        update_num_of_processes_on_our_site(msg);
    }
    else if(msg.type == RELEASE && state == WAIT_FOR_QUEUE && msg.answer_clock == request_clock){
        update_RELEASE(msg);
        ack_from_processes[message_sender] = 1;

    }
    else if(msg.type == JOIN_QUEUE && is_leader == 1){
        update_JOIN_QUEUE(message_sender);
    }
    else if(msg.type == ENTER_TUNNEL){
        enter_tunnel();
        send_message(JOINED_IN_TUNNEL, message_sender, in_tunnel_id);
        //sleep(1);
    }
    else if(msg.type == LEAVE_TUNNEL){
        leave_tunnel();
         //sleep(1);
    }
    else if(msg.type == RELEASE_QUEUE && state == WAIT_FOR_QUEUE && msg.answer_clock == request_clock){
        update_RELEASE_QUEUE(msg, message_sender);
    }
    else if(msg.type == RELEASE_TUNNEL && msg.answer_clock == request_clock){
        update_RELEASE_TUNNEL(msg);
        update_num_of_processes_on_our_site(msg);
    }
    else if(msg.type == JOINED_IN_TUNNEL){
        update_JOIN_IN_TUNNEL();
    }

    
    if(1){
        set_color();
        printf("OTRZYMANO WIADOMOSC %d od %d ID: %d LAMPORT: %d, %d, %d, %d %d\n", msg.type, message_sender, mpi_process_data->tid, lamport_clock, msg.lamport_clock, msg.state, msg.location, msg.answer_clock);
        show_process_info();
    }

    pthread_mutex_unlock(&lock_everything);

    return msg;

}

int check_msg (){
    int msg_counter, requests_counter;
    int blocked = 0;
    do{
        msg_counter = 0;
        requests_counter = 0;
        pthread_mutex_lock(&lock_ack_from_processes);
        for(int i = 0; i < mpi_process_data->size; i++){
            //printf("%d", ack_from_processes[i]);
            if(ack_from_processes[i] == 1)
                msg_counter++;
            if(requests[i] == 1)
                requests_counter++;
        }
        //printf("\n");
        pthread_mutex_unlock(&lock_ack_from_processes);
    }
    while(msg_counter < mpi_process_data->size - 1);

    pthread_mutex_lock(&lock_tunnels_ack);
    for(int i = 0; i < num_of_tunnels; i++){
        if(tunnels_ack[i] < mpi_process_data->size - 1){
            blocked++;
        }
    }
    pthread_mutex_unlock(&lock_tunnels_ack);

    //set_color();
    //printf("BLOCKED %d ID: %d\n", blocked, mpi_process_data->tid);

    if(blocked == num_of_tunnels && requests_counter > 0){
        set_color();
        printf("%d %d %d\n", msg_counter, requests_counter, blocked);
        return 0; //potrzebne działanie
    }   
    else{
        set_color();
        printf("KURWA %d %d %d\n", msg_counter, requests_counter, blocked);
        return 1; //jest ok, mozemy isc dalej
    }
    
}



int queue_to_be_entered(){
    int best_tunnel = -1;
    int max_queue_quantity = -1;
    int max_tunnel_quantity = -1;

    //sleep(0.2);

    for(int i = 0; i < num_of_tunnels; i++){
        if(tunnels_ack[i] >= mpi_process_data->size - 1 && ((i % 2 == location - 1) || num_of_tunnels == 1)){
            if(leaders_of_queues[i] == -1){
                pthread_mutex_lock(&lock_queues_ack);
                queues_ack[i] = 0;
                pthread_mutex_unlock(&lock_queues_ack);
            }
            if(queues_ack[i] < rich_group_capacity){
                if((queues_ack[i] > max_queue_quantity) || (queues_ack[i] == max_queue_quantity && 
                (tunnels_conditional_ack[i] > max_tunnel_quantity && tunnels_conditional_ack[i] <= (tunnel_capacity / rich_group_capacity - 1) * rich_group_capacity))){
                    best_tunnel = i;
                    max_queue_quantity = queues_ack[i];
                    max_tunnel_quantity = tunnels_conditional_ack[i];
                }
            }
        }
    }


    //set_color();
    //printf("TUBELIK %d ID: %d\n", best_tunnel, mpi_process_data->tid);

    return best_tunnel;
}

void wait_to_join_queue(){

    struct message msg;

    do
    {
        msg = handle_message();
        in_tunnel_id = queue_to_be_entered();
        //set_color();
        //show_process_info();

    }
    while(in_tunnel_id == -1);

}

void join_queue(){
    //sleep(1);
    set_color();
    printf("DOŁĄCZAM DO KOLEJKI NUMER %d, ID: %d LAMPORT: %d\n", in_tunnel_id, mpi_process_data->tid, lamport_clock);
    //show_process_info();
    if(leaders_of_queues[in_tunnel_id] != -1){
        //printf("WYSYŁAM DO LIDERA %d ZE WCHODZE DO KOLEJKI\n", leaders_of_queues[in_tunnel_id]);
        pthread_mutex_lock(&lock_lamport);
        lamport_clock++;
        pthread_mutex_unlock(&lock_lamport);
        send_message(JOIN_QUEUE, leaders_of_queues[in_tunnel_id], in_tunnel_id);
    }
    pthread_mutex_lock(&lock_state);
    state = IN_QUEUE;
    pthread_mutex_unlock(&lock_state);
    if(leaders_of_queues[in_tunnel_id] == -1){
        set_color();
        printf("JESTEM LIDEREM W KOLEJCE NUMER %d ID: %d LAMPORT: %d\n", in_tunnel_id, mpi_process_data->tid, lamport_clock);
        is_leader = 1;
    }
    pthread_mutex_lock(&lock_lamport);
    lamport_clock++;
    pthread_mutex_unlock(&lock_lamport);
    for(int i = 0; i < mpi_process_data->size; i++){
        if(requests[i] == 1){
            //printf("WYSYLAM INFORMACJE ZE WSZEDLEM DO KOLEJKI DO %d, ID: %d\n", i, mpi_process_data->tid);
            send_message(RELEASE, i, in_tunnel_id);
        }
    }
}

int check_if_abort(){
    set_color();
    if(in_tunnel_id == -1)
        show_process_info();
    return in_tunnel_id;
}

void wait_to_enter_tunnel(){
    if(is_leader == 1){
        while(num_of_processes_in_queue < rich_group_capacity - 1 && num_of_processes_in_queue < num_of_processes_on_our_site){
            handle_message();
        }
        while(tunnels_conditional_ack[in_tunnel_id] >= tunnel_capacity){
            handle_message();
        }
        for(int i = 0; i < mpi_process_data->size; i++){
            if(processes_in_queue[i] == 1){
                send_message(ENTER_TUNNEL, i, in_tunnel_id);
            }
        }
    }
    else{
        //printf("SIEMA ID %d\n", mpi_process_data->tid);
        //show_process_info(); !!!!!
        while(state == IN_QUEUE){
            handle_message();
        }
    }
}

void travel(){
    set_color();
    //printf("ELO ID: %d STATE: %d\n", mpi_process_data->tid, state);
    if(is_leader == 1){
        //sleep(0.2);
        leave_tunnel();
    }
    else{
        while(state == IN_TUNNEL){
            handle_message();
        }
    }
    //printf("ELO2 ID: %d STATE: %d\n", mpi_process_data->tid, state);
}

void action(){
          
    //Losowanie kiedy proces chce wejsc do tunelu
    rest();

    //Wysłanie żądania wejścia do tunelu
    request();

    //Czekamy aż będziemy mogli wejść do tunelu
    wait_to_join_queue();

    //Dołączamy do kolejki
    join_queue();

    //Czekamy aż kolejka sie zapelni po czym wchodzimy do tunelu
    wait_to_enter_tunnel();

    wait_for_processes();

    //Podróż tunelem
    travel();

}