#include "Sale_Persist.h"
#include "../Common/List.h"
#include "../Service/Sale.h"
#include"../Common/list.h"
#include"Sale_Persist.h"
#include "EntityKey_Persit.h"
#include"../Service/Sale.h"
#include<stdio.h>


static const char SALE_DATA_FILE[] = "Sale.dat";
static const char SALE_DATA_TEMP_FILE[] = "SaleTmp.dat";
static const char SALE_KEY_NAME[] = "Sale";

int Sale_Perst_Insert(sale_t *data)
{

	int rtn = 0;
	int key = EntKey_Perst_GetNewKeys(SALE_KEY_NAME,1);
	if(key <= 0)     return 0;
	data->id = key;
	FILE *fp = fopen(SALE_DATA_FILE,"ab");

	if(NULL == fp)
	{
		printf( "cannot open file %s!!\n",SALE_DATA_FILE);
		system("pause"); 
		return 0;
	}

	rtn = fwrite(data,sizeof(sale_t),1,fp);

	fclose(fp);

	return rtn;

}

//根据ID载入销售记录
int Sale_Perst_SelByID(sale_list_t list,int usrID){
	sale_list_t newNode;
	sale_t data;

	int rtn = 0;

	assert(NULL != list);

	List_Free(list,sale_node_t);

	FILE *fp = fopen(SALE_DATA_FILE,"rb");
	if(NULL == fp)   return 0;

	while(!feof(fp))
	{
		if(fread(&data,sizeof(sale_t),1,fp))
		{
			newNode = (sale_list_t)malloc(sizeof(sale_node_t));
			newNode->data = data;

			if(!newNode)
			{
				printf( "Waring,Memory OverFlow!!!\nCannot Load more Data into memory!!!\n");
				break;
			}
			if(usrID == data.user_id)
			{
				List_AddTail(list,newNode);
				rtn++;
			}

		}
	}
	
	fclose(fp);

	return rtn;
}

//根据票ID载入销售记录
int Sale_Perst_SelByTicketID (int ticket_id, sale_t *sale){
	assert(NULL!=sale);

	FILE *fp = fopen(SALE_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	sale_t data;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&data, sizeof(sale_t), 1, fp)) {
			if (ticket_id == data.ticket_id) {
				*sale = data;
				found = 1;
				break;
			};

		}
	}
	fclose(fp);

	return found;
}
int Select_Price_By_Playid(int id, sale_t *buf)
{

	assert(NULL != buf);

	FILE *fp = fopen(SALE_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}
	sale_t data;
	int found = 0;
	while (!feof(fp)) {
		if (fread(&data, sizeof(sale_t), 1, fp)) {
			if (id == data.ticket_id) {
				*buf = data;
				found = 1;
				break;
			}
		}
	}
	fclose(fp);
	return found;
}
int Sale_Perst_DeleteByID(int saleID) {
	//将原始文件重命名，然后读取数据重新写入到数据文件中，并将要删除的实体过滤掉。（借用另一个文件）
	//对原始数据文件重命名
	if (rename(SALE_DATA_FILE, SALE_DATA_TEMP_FILE)<0) {
		printf("Cannot open file %s!\n", SALE_DATA_FILE);
		return 0;
	}

	FILE *fpSour, *fpTarg;
	fpSour = fopen(SALE_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour) {
		printf("Cannot open file %s!\n", SALE_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(SALE_DATA_FILE, "wb");
	if (NULL == fpTarg) {
		printf("Cannot open file %s!\n", SALE_DATA_TEMP_FILE);
		return 0;
	}


	sale_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(sale_t), 1, fpSour)) {
			if (saleID == buf.id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(sale_t), 1, fpTarg);//一个数据一个数据写入，当遇到要删除的数据就跳过
		}
	}
	fclose(fpTarg);
	fclose(fpSour);
	//删除临时文件
	remove(SALE_DATA_TEMP_FILE); 
	return found;
}
