#pragma once
#include "ParameterData.h"
class ConsumerParameter : public ParameterData {
public:
	ConsumerParameter() {
		Set_parameter();
		printf("ConsumerParameterのコンストラクタが呼ばれました\n");
	}
	void Set_parameter() {
		fopen_s(&Parameter_FILE_4, "..\\FolderForIOFiles\\parameter\\Set_parameter_4.csv", "r");
		fscanf_s(Parameter_FILE_4, "%lf,%lf,%I64d,%I64d,%I64d,%I64d,%I64d", &rwd,&rpc,&min_consumption,&init_cash_min, &init_cash_max,&fixed_wage_min,&fixed_wage_max);
		fopen_s(&parameter4, "..\\FolderForIOFiles\\data\\parameter4.csv", "w");
		fprintf(parameter4, "ConsumerParameterのパラメータ,\n");
		fprintf(parameter4, "rwd,rpc,min_consumption,init_cash_min,init_cash_max,fixed_wage_min,fixed_wage_max,\n");
		fprintf(parameter4, "%lf,%lf,%d,%d,%d,%d,%d,\n", rwd,rpc,min_consumption,init_cash_min, init_cash_max,fixed_wage_min,fixed_wage_max);
	}
};
