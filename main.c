
#include "connection.h"



int main(void){

    int i;

    start_daytime();
    // start_chargen();

    sleep(2);
    printf("now start client...\n");

    run_client();

    return 0;
}