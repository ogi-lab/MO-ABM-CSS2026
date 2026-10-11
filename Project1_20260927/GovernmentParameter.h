#pragma once
#include "ParameterData.h"
class GovernmentParameter : public ParameterData {
public:
	GovernmentParameter() {
		Set_parameter();
		printf("GovernmentParameterのコンストラクタが呼ばれました\n");
	}
	void Set_parameter() {
		fopen_s(&Parameter_FILE_9, "..\\FolderForIOFiles\\parameter\\Set_parameter_9.csv", "r");
		fscanf_s(Parameter_FILE_9, "%lf,%lf,%lf,%lf,%d,%d", &tax_rate_income, &tax_rate_corporate, &market_purchasing_ratio, &firm_subsidy_ratio,&init_cash_min,&init_cash_max);
		fopen_s(&parameter9, "..\\FolderForIOFiles\\data\\parameter9.csv", "w");
		fprintf(parameter9, "GovernmentrParameterのパラメータ\n");
		fprintf(parameter9, "tax_rate_income,tax_rate_corporate,market_purchasing ratio, firm_subsidy_ratio,init_cash_min,init_cash_max\n");
		fprintf(parameter9, "%lf,%lf,%lf,%lf,%d,%d\n", tax_rate_income, tax_rate_corporate, market_purchasing_ratio, firm_subsidy_ratio,init_cash_min,init_cash_max);
	}
};
