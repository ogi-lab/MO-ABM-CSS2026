#pragma once
#include "ParameterData.h"
class WholesalerParameter : public ParameterData {
public:
	WholesalerParameter() {
		Set_parameter();
		printf("WholesalerParameterのコンストラクタが呼ばれました\n");
	}
	void Set_parameter() {
		fopen_s(&Parameter_FILE_6, "..\\FolderForIOFiles\\parameter\\Set_parameter_6.csv", "r");
		fscanf_s(Parameter_FILE_6, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%lf", &init_cash_min, &init_cash_max, &qmax_min, &qmax_max, &qmin_min,&qmin_max,&price_min, &price_max, &min_price_min,&min_price_max,&price_delta);
		fopen_s(&parameter6, "..\\FolderForIOFiles\\data\\parameter6.csv", "w");
		fprintf(parameter6, "WholesalerParameterのパラメータ\n");
		fprintf(parameter6, "init_cash_min,init_cash_max,qmax_min,qmax_max,qmin_min,qmin_max,price_min,price_max,min_price_min,min_price_max,price_delta\n");
		fprintf(parameter6, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%lf,\n", init_cash_min, init_cash_max, qmax_min, qmax_max,qmin_min,qmin_max, price_min, price_max, min_price_min,min_price_max,price_delta);
	}
		
};
