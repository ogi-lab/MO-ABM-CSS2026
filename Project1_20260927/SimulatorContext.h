#pragma once

// 双方向参照を避けるため、ヘッダファイルをインクルードしない
class ConsumerAggregator;
class RetailerAggregator;
class WholesalerAggregator;
class EquipmentMakerAggregator;
class SoleProprietorAggregator;
class RetailMarketNavigator;
class ShipmentNavigator;
class SimulatorConfig;
class DataAnalyzerNavigator;
class BankAggregator;
class GovernmentAggregator;
class Government;
class EnterpriseManager;
class RollAggregator;
class Accountant;
class LaborMarket;
class AgentAggregatorNavigator;
class ConsumerAggregatorNavigator;
class GovernmentAggregatorNavigator;
/** シミュレーターコンテキスト */
struct SimulatorContext {

    int agent_id_seq = 0; /**< エージェントＩＤシーケンス */
    int period_count = 0; /**< 現在期 */
	int soleproprietor_executive_count = 0;  //soleproprietorの役員数（個人事業主なので常に一人だが可変の変数としておく）
	long long withholdings0 = 0;
	

    SimulatorConfig* config;		//設定情報、パラメータ、及び書き込み用ファイルへのアクセスパスなどの管理者　20120523記　高島

    RetailMarketNavigator* retail_market_navi_for_customer;	//買い手側とリテイラー製品市場の仲介nav   20120523記　高島
	RetailMarketNavigator* material_market_navi_for_retailer; //retailerの原料購入用のmaterialMarketへのナビゲータ　20210905記　荻林
    ShipmentNavigator* shipment_navi_for_retailer;			//売り手側と市場の仲介nav、RA用  20120523記　高島
    ShipmentNavigator* shipment_navi_for_wholesaler;		//売り手側と市場の仲介nav、WA用  20120523記　高島
	ShipmentNavigator* shipment_navi_for_soleproprietor;
    //ConsumerAggregator* consumer_aggregator;				//CA集合の管理ob（基本はAgentAggregator) 20120523記　高島
	AgentAggregatorNavigator* consumerAggregatorNavigator;
	RetailerAggregator* retailer_aggregator;				//RA集合の管理ob（基本はAgentAggregator) 20120523記　高島
	WholesalerAggregator* wholesaler_aggregator;			//2012_12_31荻林
	EquipmentMakerAggregator* equipmentmaker_aggregator;		//2012_12_31荻林
	SoleProprietorAggregator* soleproprietor_aggregator;
	BankAggregator* bank_aggregator;                            //2013_1_5荻林
	//Government* government;                                     //2013_1_5荻林
	//GovernmentAggregator* government_aggregator;                //20210731 荻林
	AgentAggregatorNavigator* governmentAggregatorNavigator; //20251228 荻林
	EnterpriseManager* enterprise_manager;                      //倒産起業管理者2013_1_5荻林
	DataAnalyzerNavigator* dataAnalyzer_navi_for_retailMarket;	//データ解析用（？）のobへのnav 20120523記　高島
	DataAnalyzerNavigator* dataAnalyzer_navi_for_materialMarket;
	RollAggregator* rollAggregator;
	Accountant* accountant;
	LaborMarket* labormarket;
};
