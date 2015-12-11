#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <inttypes.h>
#include <unistd.h>
#include "msg.h"

#define QUEUE_NAME  "/qtest"
typedef int bool;
#define true        1
#define false       0

volatile bool isRunning = true;
volatile bool messagesAvailable = false;

// This handler will be called when the queue 
// becomes non-empty.

void handler (int sig_num) {
    messagesAvailable = true;
}

void handleClose(int sigNum) {
  isRunning = false;
}

mqd_t setUpQueue(char *qname) {
  struct mq_attr attr;
  mqd_t q;
  // First we need to set up the attribute structure
  attr.mq_maxmsg = 10;
  attr.mq_msgsize = MSG_SIZE;
  attr.mq_flags = 0;

  // Open a queue with the attribute structure
  return mq_open(qname, O_RDWR | O_CREAT, 0664, &attr);
}

void sendMessageToPushover(struct Message *msgObj) {
  printf ("{token: \"%s\", text: \"%s\"}.\n", msgObj->token, msgObj->text);
}

void handleMessages(mqd_t mqdes) {
  struct mq_attr attr, old_attr;   // To store queue attributes
  char buf[MSG_SIZE];              // A good-sized buffer
  unsigned int prio;               // Priority
  struct Message msgObj;           // The message from the queue
  buf[MSG_SIZE - 1] = '\0';

  // Get the attributes for the queue
  mq_getattr (mqdes, &attr);
  if (attr.mq_curmsgs != 0) {
    
    // There are some messages on this queue....eat em
    
    // First set the queue to not block any calls    
    attr.mq_flags = O_NONBLOCK;
    mq_setattr (mqdes, &attr, &old_attr);    
        
    // Now eat all of the messages
    while (mq_receive (mqdes, &buf[0], MSG_SIZE, &prio) != -1) {
      unpackMessageFromString(buf, &msgObj);
      sendMessageToPushover(&msgObj);

      // TODO: Do something with the message
      sleep(5); // Sleep for 5 seconds after processing a message
    }
      
    // The call failed.  Make sure errno is EAGAIN
    if (errno != EAGAIN) { 
      perror ("mq_receive()");
      _Exit (EXIT_FAILURE);
    }
        
    // Now restore the attributes
    mq_setattr (mqdes, &old_attr, 0);            
  }
}
    
void main () {
  struct sigevent sigevent;        // For notification
  mqd_t queue = setUpQueue(QUEUE_NAME);

  if (queue == (mqd_t) -1) {
    perror("mq_open");
  }
    
  handleMessages(queue);
    
  // We want to be notified when something is there 
  signal (SIGUSR1, handler);
  sigevent.sigev_notify = SIGEV_SIGNAL;
  sigevent.sigev_signo = SIGUSR1;

  // Hook up to sigint too
  signal(SIGINT, handleClose);
    
  if (mq_notify (queue, &sigevent) == -1) {
    if (errno == EBUSY) {
      printf ("Another process has registered for notification.\n");
    } else {
      printf("Some other error registering for notifications %d\n", errno);
      perror("mq_notify");
    }
    _Exit (EXIT_FAILURE);
  }

  while (isRunning) {
    if (messagesAvailable) {
      handleMessages(queue);
    }
    sleep(10); // Sleep a little before checking again.
  }

  printf("Shutting down.\n");
  // Close all open message queue descriptors    
  mq_close (queue);
}
