
#include "connection.h"



int main(void){

    int i;

    start_daytime();
    // start_chargen();


    sleep(2);
    printf("now start client...\n");

    run_client();

    printf("server and client started\n");
    for (i = 0; i < 10000; i++){
        sleep(200000);
    }

    return 0;
}