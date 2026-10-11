#pragma once
#include "ParameterData.h"
class SoleproprietorParameter : public ParameterData {
public:
	SoleproprietorParameter() {
		Set_parameter();
		printf("SoleproprietorParameterのコンストラクタが呼ばれました\n");
	}
	void Set_parameter() {
		fopen_s(&Parameter_FILE_7, "..\\FolderForIOFiles\\parameter\\Set_parameter_7.csv", "r");
		fscanf_s(Parameter_FILE_7, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%lf", &init_cash_min, &init_cash_max, &qmax_min, &qmax_max,&qmin_min,&qmin_max, &price_min, &price_max,&min_price_min,&min_price_max, &price_delta);
		fopen_s(&parameter7, "..\\FolderForIOFiles\\data\\parameter7.csv", "w");
		fprintf(parameter7, "SoleproprietorParameterのパラメータ\n");
		fprintf(parameter7, "init_cash_min,init_cash_max,qmax_min,qmax_max,qmin_min,qmin_max,price_min,price_max,min_price_min,min_price_max,price_delta\n");
		fprintf(parameter7, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%lf,\n", init_cash_min, init_cash_max, qmax_min, qmax_max,qmin_min,qmin_max, price_min, price_max,min_price_min,min_price_max, price_delta);
	}

};