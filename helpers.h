#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>

/* do not modify these functions */
void SimOperation(int simTime);
double GetRealTimeStamp();
int GetSimTimeStampSec(int simStartTime, double realStartTime);

/* modify (and complete) the following functions */
int TimeStampStr2Sec(char* timeStamp);
void TimeStampSec2Str(char* timeStampStr, int timeStampSec);
int ServiceTimeStr2Sec(char* serviceTimeAsString);
void GetSimTimeStamp(char* simTimeStampStr, int simStartTime, double realStartTime);
int GetCashierDelay(char* paymentType);
double GetHairCutPrice(char* customerType);

#endif
