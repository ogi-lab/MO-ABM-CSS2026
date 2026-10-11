#define _CRTDBG_MAP_ALLOC
#include <iostream> 
#include <stdlib.h>
#include <crtdbg.h>
#include <time.h>
#include <stdio.h>
#include "SimulatorApp.h"

SimulatorContext app_context;

// RandomGenerator::set_seed()の定義。RandomGenerator.hの時点ではSimulatorConfigが
// 前方宣言のみで未完全型のため定義できないので、SimulatorApp.h経由でSimulatorConfig.hの
// 読み込みが完了したこの場所（app_context定義の直後）で定義する。
void RandomGenerator::set_seed() {
	engine().seed(app_context.config->seed);
}
void print_time(){
	  //printf("before time t\n");
	  time_t timer=time(NULL);
	  //printf("after time_t\n");
	  struct tm local;
		localtime_s(&local,&timer);
	fprintf(app_context.config->out,"%4d/%2d/%2d %2d:%2d:%2d\n",local.tm_year+1900,local.tm_mon+1,local.tm_mday,local.tm_hour,local.tm_min,local.tm_sec);
	
 }


/** main関数 */
int main() {
	_CrtMemState s1;
	_CrtMemState s2;
	_CrtMemState s3;
	_CrtMemCheckpoint( &s1 );
	printf("before new SimulatorConfig()\n");
	app_context.config = new SimulatorConfig();
	printf("after new SimulatorConfig()\n");
	
	print_time();
	fprintf(app_context.config->out,"agent*id_seq=%d in the begining of set_up()\n",app_context.agent_id_seq);
	fprintf(app_context.config->consumerData->out_title_list, "period_count,agent_id, agent_type,size,account_title,\n");

    printf("-- Start --\n");
    SimulatorApp app;
    app.run_scenario();
	print_time();
    printf("-- End --\n");
	


// memory allocations take place here
	_CrtMemCheckpoint( &s2 );

	if ( _CrtMemDifference( &s3, &s1, &s2) ) _CrtMemDumpStatistics( &s3 );
	system("pause");
    return 0;
};
