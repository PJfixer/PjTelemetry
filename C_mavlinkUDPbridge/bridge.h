#ifndef BRIDGE_H

#define BRIDGE_H

#include <stdbool.h>
#include <pthread.h>
#include "udp.h"
#include "serial.h"
void *SerialTask(void *arg);
void *UdpTask(void *arg);

#endif
