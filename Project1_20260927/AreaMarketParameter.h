#pragma once
#include "ParameterData.h"
class AreaMarketParameter : public ParameterData {
public:
	AreaMarketParameter() {
		//printf("before set_parameter of producerParameter\n");
		Set_parameter();
		//printf("ProducerParameterのコンストラクタが呼ばれました\n");
	}
    /*
    void Set_parameter() {
		fopen_s(&Parameter_FILE_8, "..\\FolderForIOFiles\\parameter\\Set_parameter_8.csv", "r");
		fscanf_s(Parameter_FILE_8, "%d,%lf", &case_monopolistic,&price_range_ratio);
		fopen_s(&parameter8, "..\\FolderForIOFiles\\data\\parameter8.csv", "w");
		fprintf(parameter8, "AreaMarketParameterのパラメータ\n");
		fprintf(parameter8, "case_monopolistic,price_range_ratio,\n");
		fprintf(parameter8,"%d,%lf,\n",case_monopolistic, price_range_ratio);
	}
    */
  /*
	//:::::::::::::::::::::::::::::
    void Set_parameter() {

        // --- 読み込みファイル ---
        errno_t err = fopen_s(&Parameter_FILE_8,
            "..\\FolderForIOFiles\\parameter\\Set_parameter_8.csv", "r");

        if (err != 0 || Parameter_FILE_8 == NULL) {
            printf("Error: cannot open Set_parameter_8.csv\n");
            return;   // ここで抜けるのが安全
        }

        fscanf_s(Parameter_FILE_8, "%d,%lf", &case_monopolistic, &price_range_ratio);
        fclose(Parameter_FILE_8);


        // --- 書き込みファイル ---
        err = fopen_s(&parameter8,
            "..\\FolderForIOFiles\\data\\parameter8.csv", "w");

        if (err != 0 || parameter8 == NULL) {
            printf("Error: cannot open parameter8.csv\n");
            return;
        }

        fprintf(parameter8, "AreaMarketParameterのパラメータ\n");
        fprintf(parameter8, "case_monopolistic,price_range_ratio,\n");
        fprintf(parameter8, "%d,%lf,\n", case_monopolistic, price_range_ratio);
        fclose(parameter8);
    }

	
	//:::::::::::::::::::::::::::::
   */
    void Set_parameter() {

        // --- 読み込みファイル ---
        errno_t err = fopen_s(&Parameter_FILE_8,
            "..\\FolderForIOFiles\\parameter\\Set_parameter_8.csv", "r");

        if (err != 0 || Parameter_FILE_8 == nullptr) {
            printf("Error: cannot open Set_parameter_8.csv\n");
            return;
        }

        // ★ Visual Studio が理解できる NULL チェック
        if (Parameter_FILE_8) {
            fscanf_s(Parameter_FILE_8, "%d,%lf,%d", &case_monopolistic, &price_range_ratio,&case_wihtout_expense_initial_deposit_and_cash);
            fclose(Parameter_FILE_8);
        }
        else {
            return;
        }


        // --- 書き込みファイル ---
        err = fopen_s(&parameter8,
            "..\\FolderForIOFiles\\data\\parameter8.csv", "w");

        if (err != 0 || parameter8 == nullptr) {
            printf("Error: cannot open parameter8.csv\n");
            return;
        }

        if (parameter8) {
            fprintf(parameter8, "AreaMarketParameterのパラメータ\n");
            fprintf(parameter8, "case_monopolistic,price_range_ratio,case_without_expense_initial_deposit_and_cash_\n");
            fprintf(parameter8, "%d,%lf,%d,\n", case_monopolistic, price_range_ratio,case_wihtout_expense_initial_deposit_and_cash);
            fclose(parameter8);
        }
    }




};