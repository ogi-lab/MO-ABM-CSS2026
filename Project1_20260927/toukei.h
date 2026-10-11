/*基本統計量の解析 average.cpp*/
#include<stdio.h>
#include<math.h>
struct BasicToukei
{
	double sum = 0.0, ave = 0.0, s = 0.0, var = 0.0, sigma = 0.0, kukan = 0.0, max = 0.0, min = 0.0, range = 0.0, semean = 0.0;
	int n = 0;
};
class Toukei{
	struct BasicToukei* tk;
	double sum = 0.0, ave = 0.0, s = 0.0, var = 0.0, sigma = 0.0, kukan = 0.0, max = 0.0, min = 0.0, range = 0.0, semean = 0.0;
public:
	void average(int n,double data[],struct BasicToukei* tk);
	void average(int n,double data[]);
	void print_tkdata();
	/*toukei* Get_tk(){
		return tk;
	}*/
};
void Toukei::average(int n,double data[])
{
	tk=new struct BasicToukei;
	int i = 0;
	//基本統計解析(平均値、分散、標準偏差)
	/**最大値、最小値、合計、平均値、最大最小差*/
	sum=0.0;
	max=min=data[0];
	for(i=0;i<n;i++)
	{
		sum+=data[i];
		if(max<data[i])	max=data[i];
		if(min>data[i])	min=data[i];
	}
	ave=sum/n;
	range=max-min;
	/**偏差平方差、分散、標準偏差、標準誤差*/
	s=0.0;
	for(i=0;i<n;i++)	s+=(data[i]-ave)*(data[i]-ave);
	var=s/(n-1);
	sigma=sqrt(var);
	semean=sqrt(var/n);
	
//構造体データへの変換
		tk->n=n;//データ数
		tk->sum=sum;//合計
		tk->ave=ave;//平均値
		tk->max=max;//最大値
		tk->min=min;//最小値
		tk->range=range;//範囲
		tk->s=s;//偏差平方和
		tk->var=var;//分散
		tk->sigma=sigma;//標準偏差
		tk->semean=semean;//標準誤差
//計算結果のディスプレー上での表示
	//	print_tkdata();
}
/*void Toukei::print_tkdata(){
	printf("\n");
	printf("データ数  =%d\n",n);
	printf("総和      =%f\n",sum);
	printf("平均      =%f\n",ave);
	printf("最大値　　=%f\n",max);
	printf("最小値    =%f\n",min);
	printf("範囲      =%f\n",range);
	printf("不遍分散  =%f\n",var);
	printf("標準偏差  =%f\n",sigma);
	printf("標準誤差  =%f\n",semean);
}*/

void Toukei::average(int n,double data[],struct BasicToukei* tk)
{

	int i = 0;
	//基本統計解析(平均値、分散、標準偏差)
	/**最大値、最小値、合計、平均値、最大最小差*/
	sum=0.0;
	max=min=data[0];
	for(i=0;i<n;i++)
	{
		sum+=data[i];
		if(max<data[i])	max=data[i];
		if(min>data[i])	min=data[i];
	}
	ave=sum/n;
	range=max-min;
	/**偏差平方差、分散、標準偏差、標準誤差*/
	s=0.0;
	for(i=0;i<n;i++)	s+=(data[i]-ave)*(data[i]-ave);
	var=s/(n-1);
	sigma=sqrt(var);
	semean=sqrt(var/n);
	
//構造体データへの変換
		tk->n=n;//データ数
		tk->sum=sum;//合計
		tk->ave=ave;//平均値
		tk->max=max;//最大値
		tk->min=min;//最小値
		tk->range=range;//範囲
		tk->s=s;//偏差平方和
		tk->var=var;//分散
		tk->sigma=sigma;//標準偏差
		tk->semean=semean;//標準誤差
//計算結果のディスプレー上での表示
		//print_tkdata();
}

