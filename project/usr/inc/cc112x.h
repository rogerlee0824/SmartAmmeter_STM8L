#ifndef _CC112X_H_
#define _CC112X_H_

typedef enum {CC112X_INIT_EVENT, CC112X_TRANSMIT_EVENT,CC112X_RECEIVE_EVENT} emCC112x_event_t;

typedef struct {
	emCC112x_event_t eCC112x_event;
	uint8_t * pData;
} cc112x_event_t;

#endif      /* #ifndef _CC112X_H_ */
