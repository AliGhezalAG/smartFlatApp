#ifndef INIT_H
#define INIT_H

#include <QObject>

using namespace std;

// Kinvent KForce params

#define SERVICE_UUID                        "{49535343-fe7d-4ae5-8fa9-9fafd205e455}"
#define CHARACTERISTIC_UUID                 "{49535343-1e4d-4bd9-ba61-23c647249616}"
#define SET_TIME_CLOCK_COMMAND              "71"
#define GET_MEASURE_MULYIPLIER_COMMAND      "21"
#define GET_BASELINE_COMMAND                "43"
#define GET_MEMORY_DUMP_COMMAND             "67"
#define MEASUREMENT_MULTIPLIER_DENOMINATOR  2000000.0

// UTT board params
#define PACKET_SIZE                         12
#define REQUEST_DATA_COMMAND                "5253"
#define END_OF_DATA_CODE                    "5250"
#define OPENING_PACKET_START_PREFIX         "5049"
#define OPENING_PACKET_END_PREFIX           "4950"
#define CLOSING_PACKET_START_PREFIX         "5052"

#endif // INIT_H
