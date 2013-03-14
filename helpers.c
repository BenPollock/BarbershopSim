#include "helpers.h"
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

/* simulate a real world operation at 1000x speed */
void SimOperation( int simTime )
{
   /* simTime is the actual time it would take the operation to complete in real world seconds */
   usleep((useconds_t)(simTime*1000));
}

/* get the elapsed time since 1/1/1970 in seconds with microsecond precision */
double GetRealTimeStamp()
{
   struct timeval tp;
   gettimeofday(&tp, NULL);
   return (tp.tv_sec + tp.tv_usec/1000000.0);
}

/* get the simulated time stamp in seconds */
/* simStartTime is the time in seconds when the simulation is suppose to start (e.g., barbershop opens at 9:00am so simStartTime should be 9*3600 = 32400 in this scenario */
/* realStartTime is the actual time in seconds of when the simulation began given in elapsed time since 1/1/1970 */
int GetSimTimeStampSec(int simStartTime, double realStartTime )
{
   double simElapsedTime, realElapsedTime;

   /* calculate the elapsed time since realStartTime */
   realElapsedTime = GetRealTimeStamp() - realStartTime;

   /* simulation runs at 1000x speed */
   simElapsedTime = 1000*realElapsedTime;

   return (simStartTime + (int)simElapsedTime);
}

/* convert a time stamp from seconds to a string */
/* e.g., 9124 =  02:32:04 */
void TimeStampSec2Str( char* timeStampStr, int timeStampSec )
{
	int hours = (int)(timeStampSec / 3600);
	int minutes = (int)((timeStampSec - (hours*3600))/60);
	int seconds = timeStampSec - (hours*3600) - (minutes*60);

	//Then it is hours:minutes:seconds
	strcpy(timeStampStr, "");
	//if(hours > 0){
		//strcat(timeStampStr, itoa(hours));
		char* temphours = malloc(sizeof(char)*10);
		sprintf(temphours, "%d", hours);
		if(hours < 10)
            strcat(timeStampStr, "0");
		strcat(timeStampStr, temphours);
		strcat(timeStampStr, ":");
	//}
	//if(minutes > 0){
        char* tempminutes = malloc(sizeof(char)*10);
		sprintf(tempminutes, "%d", minutes);
		if(minutes < 10)
            strcat(timeStampStr, "0");
		strcat(timeStampStr, tempminutes);
		//strcat(timeStampStr, itoa(minutes));
		strcat(timeStampStr, ":");
	//}
	char *tempseconds = malloc(sizeof(char)*10);
	sprintf(tempseconds, "%d", seconds);
	if(seconds < 10)
        strcat(timeStampStr, "0");
	strcat(timeStampStr, tempseconds);


}

/* convert customer service time from string to integer */
/* e.g., 32:04 = 32*60 + 4 = 1924 */
int ServiceTimeStr2Sec( char* serviceTime )
{
    char * copy = strdup(serviceTime);
    char * copy2 = strdup(serviceTime);
	int serviceTimeInt = 0;
	char *tokens = malloc(sizeof(char)*10);
	tokens = strtok(copy, ":");
	int i = 0;

	//Find the number of tokens, to determine the layout of the string.
	//For instance, i = 3 means it is H:M:S and i = 2 means it is only M:S
	while(tokens!=NULL){
		i++;
		tokens = strtok(NULL, ":");
	}

	char* tokens2 = strtok(copy2, ":");

	while(tokens2 != NULL){
		if(i == 3)
			serviceTimeInt += (atoi(tokens2)*3600);
		else if(i == 2)
			serviceTimeInt += (atoi(tokens2)*60);
		else if(i == 1)
			serviceTimeInt += atoi(tokens2);
		i--;
		tokens2 = strtok(NULL, ":");
	}
   return serviceTimeInt;
}

/* convert a time stamp from string to an integer in seconds */
/* e.g., 02:32:04 = 2*3600 + 32*60 + 4 = 9124 */
int TimeStampStr2Sec( char* timeStamp )
{
    char* copy = strdup(timeStamp);
    char* copy2 = strdup(timeStamp);
  int serviceTime = 0;
	char *tokens;
	tokens = strtok(copy, ":");
	int i = 0;

	//Find the number of tokens, to determine the layout of the string.
	//For instance, i = 3 means it is H:M:S and i = 2 means it is only M:S
	while(tokens!=NULL){
		i++;
		tokens = strtok(NULL, ":");
	}

	char* tokens2 = strtok(copy2, ":");
	while(tokens2 != NULL){
		if(i == 3)
			serviceTime += (atoi(tokens2)*3600);
		else if(i == 2)
			serviceTime += (atoi(tokens2)*60);
		else if(i == 1)
			serviceTime += atoi(tokens2);
		i--;
		tokens2 = strtok(NULL, ":");
	}
   return serviceTime;
}



/* look up the number of seconds it would take complete a certain payment transaction */
int GetCashierDelay( char* paymentType )
{
   if(strcmp(paymentType, "credit") == 0)
   		return 120;
   else if(strcmp(paymentType, "cash") == 0)
   		return 60;
   	else
   		return 0;
}

/* look up the amount to charge a particular customer */
double GetHairCutPrice( char* customerType )
{
	if(strcmp(customerType, "child") == 0)
		return 10.0;
	else if(strcmp(customerType, "man") == 0)
		return 20.0;
	else if(strcmp(customerType, "woman") == 0)
		return 30.0;
}
