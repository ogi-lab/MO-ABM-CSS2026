#pragma once
#include "ParameterData.h"
class RetailerParameter : public ParameterData {
public:
	RetailerParameter() {
		Set_parameter();
		printf("RetailerParameterのコンストラクタが呼ばれました\n");
	}
	void Set_parameter() {
		fopen_s(&Parameter_FILE_5, "..\\FolderForIOFiles\\parameter\\Set_parameter_5.csv", "r");
		fscanf_s(Parameter_FILE_5, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%lf,%lf",
			&init_cash_min, &init_cash_max, &qmax_min, &qmax_max,&qmin_min,&qmin_max, &price_min, &price_max,&min_price_min,&min_price_max,&price_delta,&min_utility_weight);
		fopen_s(&parameter5, "..\\FolderForIOFiles\\data\\parameter5.csv", "w");
		fprintf(parameter5, "RetailerParameterのパラメータ\n");
		fprintf(parameter5,"init_cash_min,init_cash_max,qmax_min,qmax_max,qmin_min,qmin_max,price_min,price_max,min_price_min,min_price_max,price_delta,min_utility_weight\n");
		fprintf(parameter5,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%lf,%lf,\n",
			init_cash_min, init_cash_max, qmax_min, qmax_max, qmin_min,qmin_max,price_min, price_max,min_price_min,min_price_max, price_delta,min_utility_weight);
	}
};