#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "functions.h"

void initialize(){
    requests = malloc(mpi_process_data->size * sizeof(int));
    ack_from_processes = malloc(mpi_process_data->size * sizeof(int));
    tunnels_ack = malloc(sizeof(int));
    tunnels_conditional_ack = malloc(num_of_tunnels * sizeof(int));
    queues_ack = malloc(num_of_tunnels * sizeof(int));
    leaders_of_queues = malloc(num_of_tunnels * sizeof(int));
    processes_in_queue = malloc(mpi_process_data->size * sizeof(int));
    requests_to_queue = malloc(mpi_process_data->size * sizeof(int));

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

    if(mpi_process_data->tid % 2 == 0){
      state = IN_ANOTHER_DIMENSION;
      location = IN_ANOTHER_DIMENSION;
   }

   if(tunnel_capacity < rich_group_capacity){
      rich_group_capacity = tunnel_capacity;
   }

}

void reset_request_data(){
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

    request_clock = -1;
    is_leader = 0;
    in_tunnel_id = -1;
    num_of_processes_in_queue = 0;
    num_of_processes_on_our_site = 0;
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
   set_color();
   if(location == ON_EARTH)
        printf("JESTEM NA ZIEMI ID: %d LAMPORT: %d\n", mpi_process_data->tid, lamport_clock);
    else if(location == IN_ANOTHER_DIMENSION)
        printf("JESTEM W INNYM WYMIARZE ID: %d LAMPORT: %d\n", mpi_process_data->tid, lamport_clock);    
   srand(mpi_process_data->tid);
   sleep(2);
   while(state != WAIT_FOR_QUEUE){
      sleep(2);
      int r = rand() % 10 + 1;
      if(r <= 5){
         set_color();
         printf("ZMIANA STANU NA WAIT FOR QUEUE ID: %d LAMPORT: %d\n", mpi_process_data->tid, lamport_clock);
         state = WAIT_FOR_QUEUE;
      }
   }
}

void update_lamport_clock(int msg_clock){
   int max_clock;
   if(msg_clock > lamport_clock)
      max_clock = msg_clock;
   else
      max_clock = lamport_clock;
   lamport_clock = max_clock + 1;
}

void add_to_requests(int process_id){
    requests[process_id] = 1;
}

void add_to_requests_to_queue(int process_id){
    requests_to_queue[process_id] = 1;
}

void update_ACK_ABSOLUTE(){
    for(int i = 0; i < num_of_tunnels; i++){
        tunnels_ack[i]++;
    }
}

void update_ACK_EXCLUSION(int tunnel_id){
    for(int i = 0; i < num_of_tunnels; i++){
        if(i != tunnel_id){
            tunnels_ack[i]++;
        }
    }
}

void update_ACK_CONDITIONAL(int tunnel_id) {
    tunnels_conditional_ack[tunnel_id]++;
    for(int i = 0; i < num_of_tunnels; i++){
        tunnels_ack[i]++;
    }
}

void update_ACK_TO_QUEUE(struct message msg) {
    if(msg.location == location){
        queues_ack[msg.tunnel_id]++;
        for(int i = 0; i < num_of_tunnels; i++){
            tunnels_ack[i]++;
        }
    }
    else{
        for(int i = 0; i < num_of_tunnels; i++){
            if(i != msg.tunnel_id){
                tunnels_ack[i]++;
            }
        }
    }
}

void update_num_of_processes_on_our_site(struct message msg){
    if(msg.type != RELEASE_TUNNEL){
        if(location == msg.location && (msg.state == location || msg.state == WAIT_FOR_QUEUE)){
            num_of_processes_on_our_site++;
        }
    }
    else if(msg.location != location){
        num_of_processes_on_our_site++;
    }
}

void update_JOIN_QUEUE(int message_sender){
    processes_in_queue[message_sender] = 1;
    num_of_processes_in_queue++;
    //printf("ID %d do rodzinki dolaczyl proces %d i mamy obecnie %d procesow\n", mpi_process_data->tid, message_sender, num_of_processes_in_queue);
}

void request_when_not_busy(int message_sender){
    //set_color();
    //printf("WYSYLAM ACK ABSOLUTE ID: %d LAMPORT: %d\n", mpi_process_data->tid, lamport_clock);
    send_message(ACK_ABSOLUTE, message_sender, -1);
}

void request_when_in_opposite_direction(int message_sender){
    //set_color();
    //printf("WYSYŁAM ACK_EXCLUSION BO TUNEL O ID %d PROWADZI W PRZYECIWNYM KIERUNKU LAMPORT: %d\n", in_tunnel_id, lamport_clock);
    send_message(ACK_EXCLUSION, message_sender, in_tunnel_id);
    add_to_requests_to_queue(message_sender);
}

void request_when_in_tunnel(int message_sender){
    //set_color();
    //printf("WYSYŁAM ACK_CONDITIONAL DO %d ID: %d\n", message_sender, mpi_process_data->tid);
    send_message(ACK_CONDITIONAL, message_sender, in_tunnel_id);
    add_to_requests_to_queue(message_sender);
}

void request_when_in_queue(int message_sender){
    //set_color();
    //printf("WYSYLAM ACK_TO_QUEUE DO %d ID: %d\n", message_sender, mpi_process_data->tid);
    send_message(ACK_TO_QUEUE, message_sender, in_tunnel_id);
    add_to_requests_to_queue(message_sender);
}

void request_when_competition(int message_sender, struct message msg){
    if(location == msg.location){
        if(request_clock < msg.lamport_clock || (request_clock == msg.lamport_clock && mpi_process_data->tid < message_sender)){
            if(ack_from_processes[message_sender] == 0){
                update_ACK_ABSOLUTE();
                ack_from_processes[message_sender] = 1;
                update_num_of_processes_on_our_site(msg);
            }
            add_to_requests(message_sender);
            add_to_requests_to_queue(message_sender);
        }
    }
    else if(location == IN_ANOTHER_DIMENSION){
        if(ack_from_processes[message_sender] == 0){
            update_ACK_ABSOLUTE();
            ack_from_processes[message_sender] = 1;
            update_num_of_processes_on_our_site(msg);
        }
        add_to_requests(message_sender);
        add_to_requests_to_queue(message_sender);
    }
}

void broadcast_RELEASE_QUEUE(){
    //set_color();  
    //printf("OPUSZCZAM KOLEJKE numer: %d ID: %d LAMPORT: %d\n", in_tunnel_id, mpi_process_data->tid, lamport_clock);
    for(int i = 0; i < mpi_process_data->size; i++){
        if(requests_to_queue[i] == 1 && processes_in_queue[i] == 0){
            //printf("WYSYLAM RELEASE QUEUE DO %d ID: %d\n", i, mpi_process_data->tid);
            send_message(RELEASE_QUEUE, i, in_tunnel_id);
        }
    }
}

void update_RELEASE_QUEUE(struct message msg, int message_sender){
    if(msg.location == location){
        if(msg.is_leader == 1){
            leaders_of_queues[msg.tunnel_id] = -1;
        }
        queues_ack[msg.tunnel_id]--;
        tunnels_conditional_ack[msg.tunnel_id]++;
    }
    //show_process_info();
}

void enter_tunnel(){
    set_color();
    printf("WCHODZE DO TUNELU numer %d ID: %d LAMPORT: %d\n", in_tunnel_id, mpi_process_data->tid, lamport_clock);
    state = IN_TUNNEL;
    if(is_leader == 1){
        for(int i = 0; i < mpi_process_data->size; i++){
            if(processes_in_queue[i] == 1){
                send_message(ENTER_TUNNEL, i, in_tunnel_id);
            }
        }
    }
    broadcast_RELEASE_QUEUE();
}

void broadcast_RELEASE_TUNNEL() {
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
}


void update_RELEASE_TUNNEL(struct message msg){
    if(msg.location != location){
        tunnels_ack[msg.tunnel_id]++;
    }
    else{
        tunnels_conditional_ack[msg.tunnel_id]--;
    }
    //show_process_info();
}


void leave_tunnel(){
    set_color();
    printf("OPUSZCZAM TUNEL numer: %d ID: %d LAMPORT: %d\n", in_tunnel_id, mpi_process_data->tid, lamport_clock);
    broadcast_RELEASE_TUNNEL();

    if(location == ON_EARTH){
        state = IN_ANOTHER_DIMENSION;
        location = IN_ANOTHER_DIMENSION;
    }
    else if(location == IN_ANOTHER_DIMENSION) {
        state = ON_EARTH;
        location = ON_EARTH;
    }
    reset_request_data();
}

void handle_message(struct message msg, MPI_Status status){
    int message_sender = status.MPI_SOURCE;
    update_lamport_clock(msg.lamport_clock);


    if(msg.is_leader == 1 && msg.state == IN_QUEUE && msg.location == location){
        leaders_of_queues[msg.tunnel_id] = message_sender;
    }

    if(msg.type == REQ_TO_ANOTHER_DIMENSION || msg.type == REQ_TO_EARTH){
        if(state == ON_EARTH || state == IN_ANOTHER_DIMENSION){
                request_when_not_busy(message_sender);
        }
        else if((state == IN_TUNNEL || state == IN_QUEUE) && ((location == ON_EARTH && msg.type == REQ_TO_EARTH) || (location == IN_ANOTHER_DIMENSION && msg.type == REQ_TO_ANOTHER_DIMENSION))){
                request_when_in_opposite_direction(message_sender);
        }
        else if(state == IN_TUNNEL){
                request_when_in_tunnel(message_sender);
        }
        else if(state == IN_QUEUE){
                request_when_in_queue(message_sender);
        }
        else if(state == WAIT_FOR_QUEUE){
                request_when_competition(message_sender, msg);
        }

    }
    else if(msg.type == ACK_ABSOLUTE && state == WAIT_FOR_QUEUE){
        if(ack_from_processes[message_sender] == 0){
            update_ACK_ABSOLUTE();
            ack_from_processes[message_sender] = 1;
            update_num_of_processes_on_our_site(msg);
        }
    }
    else if(msg.type == ACK_EXCLUSION && state == WAIT_FOR_QUEUE){
        update_ACK_EXCLUSION(msg.tunnel_id);
        ack_from_processes[message_sender] = 1;
    }
    else if(msg.type == ACK_CONDITIONAL && state == WAIT_FOR_QUEUE){
        update_ACK_CONDITIONAL(msg.tunnel_id);
        ack_from_processes[message_sender] = 1;
    } 
    else if(msg.type == ACK_TO_QUEUE && state == WAIT_FOR_QUEUE){
        update_ACK_TO_QUEUE(msg);
        ack_from_processes[message_sender] = 1;
        update_num_of_processes_on_our_site(msg);
    }
    else if(msg.type == JOIN_QUEUE && is_leader == 1){
        update_JOIN_QUEUE(message_sender);
    }
    else if(msg.type == ENTER_TUNNEL){
        enter_tunnel();
    }
    else if(msg.type == LEAVE_TUNNEL){
        leave_tunnel();
    }
    else if(msg.type == RELEASE_QUEUE && state == WAIT_FOR_QUEUE){
        update_RELEASE_QUEUE(msg, message_sender);
    }
    else if(msg.type == RELEASE_TUNNEL){
        update_RELEASE_TUNNEL(msg);
        update_num_of_processes_on_our_site(msg);
    }

    
    // if(state == WAIT_FOR_QUEUE && mpi_process_data->tid == 0){
    //     set_color();
    //     printf("OTRZYMANO WIADOMOSC %d od %d ID: %d LAMPORT: %d\n", msg.type, message_sender, mpi_process_data->tid, lamport_clock);
    //     show_process_info();
    // }

}


int queue_to_be_entered(){
    int best_tunnel = -1;
    int max_queue_quantity = -1;
    int max_tunnel_quantity = -1;
    int can_enter = 0;
    for(int i = 0; i < num_of_tunnels; i++){
        if(tunnels_ack[i] == mpi_process_data->size - 1){
            can_enter = 1;
        }
    }
    if(can_enter == 1){
        sleep(1);

        for(int i = 0; i < num_of_tunnels; i++){
            if(tunnels_ack[i] == mpi_process_data->size - 1){
                if(leaders_of_queues[i] == -1){
                    queues_ack[i] = 0;
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
    }
    return best_tunnel;
}

void wait_to_join_queue(){
    do
    {
        in_tunnel_id = queue_to_be_entered();
    }
    while(in_tunnel_id == -1);
}

void join_queue(){
    set_color();
    printf("DOŁĄCZAM DO KOLEJKI NUMER %d, ID: %d LAMPORT: %d\n", in_tunnel_id, mpi_process_data->tid, lamport_clock);
    //show_process_info();
    if(leaders_of_queues[in_tunnel_id] != -1){
        //printf("WYSYŁAM DO LIDERA %d ZE WCHODZE DO KOLEJKI\n", leaders_of_queues[in_tunnel_id]);
        lamport_clock++;
        send_message(JOIN_QUEUE, leaders_of_queues[in_tunnel_id], in_tunnel_id);
    }
    state = IN_QUEUE;
    if(leaders_of_queues[in_tunnel_id] == -1){
        set_color();
        printf("JESTEM LIDEREM W KOLEJCE NUMER %d ID: %d LAMPORT: %d\n", in_tunnel_id, mpi_process_data->tid, lamport_clock);
        is_leader = 1;
    }
    lamport_clock++;
    for(int i = 0; i < mpi_process_data->size; i++){
        if(requests[i] == 1){
            //printf("WYSYLAM INFORMACJE ZE WSZEDLEM DO KOLEJKI DO %d, ID: %d\n", i, mpi_process_data->tid);
            send_message(ACK_TO_QUEUE, i, in_tunnel_id);
        }
    }
}

void wait_to_enter_tunnel(){
    set_color();
    if(is_leader == 1){
        while(num_of_processes_in_queue < rich_group_capacity - 1 && num_of_processes_in_queue < num_of_processes_on_our_site){}
        while(tunnels_conditional_ack[in_tunnel_id] >= tunnel_capacity){}
        set_color();
        printf("JESTEM LIDEREM ID %d I WCHODZE DO TUNELU %d LAMPORT: %d\n", mpi_process_data->tid, in_tunnel_id, lamport_clock);
        enter_tunnel();
    }
    else{
        while(state != IN_TUNNEL){}
    }
}

void travel(){
    if(is_leader == 1){
        sleep(4);
        leave_tunnel();
    }
}