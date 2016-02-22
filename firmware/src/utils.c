
#include "utils.h"
#include "system/random/sys_random.h"

int32_t getDelay(int32_t const multip)
{
    return SYS_TMR_TickCountGet() + multip * SYS_TMR_TickCounterFrequencyGet();
}

void generateDeviceId(char* deviceId, uint32_t random)
{
    sprintf(deviceId,"device%i",random);
}
