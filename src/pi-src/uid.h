/******************************************************************************
Decodes unique identifiers from transmissions

The details of UID encoding and decoding can be found in
.../doc/TransmissionProtocol/TransmissionProtocol.pdf,
also known as the iCRAB Protocol Documentation.

Note that we expect some amount of noise or variance in our measured signals,
so we can't rely on the measured durations or delays matching perfectly with
our mathematical expectations. To compensate for this, we simply round each
decoded ID (as a float) to the nearest integer. This way, if a given duration
is 0.074ms longer than it "should be" based on our documentation, the small
error is thrown away.

Author:  Noah Strong
Project: Crab Tracker
Created: 2018-02-28
******************************************************************************/

#ifndef __UID_H
#define __UID_H

#include "data_collection.h"

/*** Transmission Protocol Constants ***
 * Values should match those in
 * .../doc/TransmissionProtocol/TransmissionProtocol.pdf
 */
#define MIN_INTERVAL_S 15
#define MAX_INTERVAL_S 30
#define MAX_ID 499
#define MIN_PING_DUR_MS 1.0
#define MIN_DELAY_DUR_MS 10.0
#define STEP_SIZE_MS 0.1

int id_decode_ping(ping);
int id_decode_delay(ping, ping);

#endif /* __UID_H */
