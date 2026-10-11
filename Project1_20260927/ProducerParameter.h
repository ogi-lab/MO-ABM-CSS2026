#pragma once
#include "ParameterData.h"
class ProducerParameter : public ParameterData {
public:
	ProducerParameter(){
		printf("before set_parameter of producerParameter\n");
		Set_parameter();
		printf("ProducerParameterのコンストラクタが呼ばれました\n");
	}
	void Set_parameter() {
		fopen_s(&Parameter_FILE_3, "..\\FolderForIOFiles\\parameter\\Set_parameter_3.csv", "r");
		fscanf_s(Parameter_FILE_3, "%d,%lf,%lf,%lf,%lf", &critical_period_for_demand,&beta,&critical_stock_ratio_price,&critical_stock_ratio_quantity,&initial_quantity_ratio);
		fopen_s(&parameter3, "..\\FolderForIOFiles\\data\\parameter3.csv", "w");
		fprintf(parameter3, "ProducerParameterのパラメータ\n");
		fprintf(parameter3, "critical_period_for_demand,beta,criticfal_stock_ratio_price,critical_stock_ratio_quantity,initial_quantity_ratio,\n");
		fprintf(parameter3, "%d,%lf,%lf,%lf,%lf\n",critical_period_for_demand, beta,critical_stock_ratio_price,critical_stock_ratio_quantity,initial_quantity_ratio);
	}

};