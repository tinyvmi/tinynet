#include <stdio.h>
#include <unistd.h>


/* 
 * sample daytime server based on xen/extras/mini-os/daytime.c
 * 
 */

#include <os.h>
#include <xmalloc.h>
#include <console.h>
#include <netfront.h>
#include <lwip/api.h>

#include "connection.h"

#define LISTEN_PORT 49152

static char message[29];

void run_daytime(void *p)
{
    struct ip_addr listenaddr = { 0 };
    struct netconn *listener;
    struct netconn *session;
    struct timeval tv;
    u16_t listen_port=LISTEN_PORT; //13;

    err_t rc;

    //start_networking();

    // if (0) {
    //     struct ip_addr ipaddr = { htonl(0x0a000001) };
    //     struct ip_addr netmask = { htonl(0xffffff00) };
    //     struct ip_addr gw = { 0 };
    //     networking_set_addr(&ipaddr, &netmask, &gw);
    // }

    printf("server: %s: Opening connection\n", __FUNCTION__);

    listener = netconn_new(NETCONN_TCP);
    printf("%s: Connection at %p, port: %d\n", __FUNCTION__, listener, listen_port);

    //rc = netconn_bind(listener, &listenaddr, listen_port);
    rc = netconn_bind(listener, IP_ADDR_ANY, listen_port);
    if (rc != ERR_OK) {
        printf("%s: Failed to bind connection: %i\n", __FUNCTION__, rc);
        return;
    }
    printf("%s: success bind port: %u\n", __FUNCTION__, listen_port);

    rc = netconn_listen(listener);
    if (rc != ERR_OK) {
        printf("Failed to listen on connection: %i\n", rc);
        return;
    }

    printf("%s: now listening on connection\n", __FUNCTION__);

    while (1) {

        session = netconn_accept(listener);

        printf("%s: got a connection from client!!!!!!\n", __FUNCTION__);

        if (session == NULL) 
            continue;
        //rc = netconn_accept(listener, &session);
        //if (rc != ERR_OK)
        //   continue;

        gettimeofday(&tv, NULL);
        sprintf(message, "%20lu.%6.6lu\n", tv.tv_sec, tv.tv_usec);
        (void) netconn_write(session, message, strlen(message), NETCONN_COPY);
        (void) netconn_disconnect(session);
        (void) netconn_delete(session);
    }
}


int start_daytime(void)
{
    // int i ;

    create_thread("server", run_daytime, NULL);

    // for (i = 0; i < 10000; i++){
    //     sleep(200000);
    // }
    return 0;
}


/**
 *  sample client based on  http://lwip.wikia.com/wiki/Netconn_connect
*/

void run_client(void)
{

    struct ip_addr remote_ip = { htonl(0x0a000065) };
    
    struct ip_addr localip = { htonl(0x0a000065) };
    
    struct netconn *targetconn;
    struct netconn *session;
    struct timeval tv;
    u16_t listen_port=LISTEN_PORT; //13;

    err_t rc;

    while (1){
        printf("----Client: %s: Opening connection\n", __FUNCTION__);

        targetconn = netconn_new(NETCONN_TCP);
            if ( targetconn == NULL ) { 
        
            /* No memory for new connection? */
            continue;
        }
        printf("----%s: Connection at %p, port: %d\n",
            __FUNCTION__, targetconn, listen_port);
    
        rc = netconn_bind(targetconn, &localip, 0);
        // rc = netconn_bind(targetconn, IP_ADDR_ANY, listen_port);
        if (rc != ERR_OK) {
            printf("----%s: Failed to bind connection: %i\n",
                 __FUNCTION__, rc);
            // return;
            continue;
        }

        printf("----%s: Connecting remote ip %x, port: %d\n", __FUNCTION__,
            ntohl(remote_ip.addr), LISTEN_PORT);
    
        rc = netconn_connect ( targetconn, &remote_ip, LISTEN_PORT ); 
        
        if ( rc != ERR_OK )
        {

            printf("----%s: Error connecting. Now reconnect...\n", __FUNCTION__);
            netconn_delete ( targetconn );
            continue;
        }

        // while (1) {
            gettimeofday(&tv, NULL);
            sprintf(message, "%20lu.%6.6lu\n", tv.tv_sec, tv.tv_usec);
            (void) netconn_write(targetconn, message, strlen(message), NETCONN_COPY);
            (void) netconn_disconnect(targetconn);
            (void) netconn_delete(targetconn);
        // }
    }
}
