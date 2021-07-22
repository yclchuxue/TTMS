#include "Account_Persist.h"
#include "../Service/Account.h"
#include "../Common/List.h"
#include "EntityKey_Persit.h"	
#include <stdlib.h>
#include <stdio.h>
#include<unistd.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
static const char ACCOUNT_DATA_FILE[] = "Account.dat";
static const char ACCOUNT_DATA_TEMP_FILE[] = "AccountTmp.dat";
int Account_Perst_CheckAccFile() 
{

	if(access("Account.dat",F_OK)!=-1)
	{
		return 1;
	}
	return 0;
}
int Account_Perst_Insert(account_t *data) 
{
	printf("%d %d %s %s\n", data->id, data->type, data->username, data->password);
	FILE *fp;
	fp=fopen("Account.dat","ab+");
	if(fp==NULL)
	{
		printf("打开文件失败!");
		return 0;
	}
	
	if(fwrite(data,sizeof(account_t),1,fp))
	{
		printf("####%d####\n", data->type); 
		printf("注册成功！");
		fclose(fp);
		return 1;
	}
	
	fclose(fp);
	return 0;
}


int Account_Perst_Update(const account_t * data) 
{

	assert(NULL!=data);

	FILE *fp = fopen("Account.dat", "rb+");
	if (NULL == fp) {
		printf("Cannot open file %s!\n", "Account.dat");
		return 0;
	}

	account_t buf;
	int found = 0;

	while (!feof(fp)) 
	{
		if (fread(&buf, sizeof(account_t), 1, fp)) 
		{
			if (buf.id == data->id) 
			{
				fseek(fp, -((int)sizeof(account_t)), SEEK_CUR);
				fwrite(data, sizeof(account_t), 1, fp);
				found = 1;
				break;
			}

		}
	}
	fclose(fp);

	return found;

}
int Account_Perst_SelectAll(account_list_t list)
{
	account_node_t *newNode;
	account_t data;
	int ans = 0;


	List_Free(list, account_node_t);
	FILE *fp = fopen("Account.dat", "rb");
	if (NULL == fp) { 
		return 0;
	}
	while (!feof(fp)) {
		if (fread(&data, sizeof(account_t), 1, fp)) 
		{
			newNode = (account_node_t*) malloc(sizeof(account_node_t));
			if (!newNode) 
			{
				printf(
						"Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
				break;
			}
			newNode->data = data;
			List_AddTail(list, newNode);
			ans++;
		}
	}
	fclose(fp);
	return ans;
 } 
 
 int Account_Perst_RemByID(int id) {
	if(rename(ACCOUNT_DATA_FILE, ACCOUNT_DATA_TEMP_FILE)<0){
		printf("Cannot open file %s!\n", ACCOUNT_DATA_FILE);
		return 0;
	}

	FILE *fp1, *fp2;
	fp1 = fopen(ACCOUNT_DATA_TEMP_FILE, "rb");
	if (NULL == fp1 ){
		printf("Cannot open file %s!\n", ACCOUNT_DATA_FILE);
		return 0;
	}

	fp2 = fopen(ACCOUNT_DATA_FILE, "wb");
	if ( NULL == fp2 ) {
		printf("Cannot open file %s!\n", ACCOUNT_DATA_TEMP_FILE);
		return 0;
	}


	account_t buf;

	int found = 0;
	while (!feof(fp1)) {
		if (fread(&buf, sizeof(account_t), 1, fp1)) {
			if (id == buf.id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(account_t), 1, fp2);
		}
	}

	fclose(fp2);
	fclose(fp1);
	remove(ACCOUNT_DATA_TEMP_FILE);
	return found;
}

int Account_Perst_SelByName(char usrName[], account_t *buf) 
{

	FILE *fp;
	account_t flag;
	fp=fopen("Account.dat","ab+");
	if(fp==NULL)
	{
		printf("打开文件失败！");
		return 0;
	}
	while(fread(&flag,sizeof(account_t),1,fp))
	{
		if(strcmp(flag.username,usrName)==0)
		{
			*buf=flag;
			return flag.type;
		}
	}
	return -1;

}

/*
int Account_Perst_DeleteByID(int id) 
{
	if(rename("Account.dat", "AccountTmp.dat")<0)
	{
		printf("改名失败!\n");
		return 0;
	}

	FILE *fp1, *fp2;
	fp1 = fopen("AccountTmp.dat", "rb");
	if (NULL == fp1 ){
		printf("Cannot open file %s!\n", "AccountTmp.dat");
		return 0;
	}

	fp2 = fopen("Account.dat", "wb");
	if ( NULL == fp2 ) {
		printf("Cannot open file %s!\n", "Account.dat");
		return 0;
	}


	account_t buf;

	int found = 0;
	while (!feof(fp1)) {
		if (fread(&buf, sizeof(account_t), 1, fp1)) 
		{
			if (id == buf.id) 
			{
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(account_t), 1, fp2);
		}
	}

	fclose(fp1);
	fclose(fp2);
	
	remove("AccountTmp.dat");
	
	return found;
}
*/
int Account_Perst_DeleteByID(int id) 
{

	FILE *fp1, *fp2;
	fp1 = fopen("Account.dat", "rb");
	if (NULL == fp1 ){
		printf("Cannot open file %s!\n", "Account.dat");
		return 0;
	}

	fp2 = fopen("AccountTmp.dat", "wab");
	if ( NULL == fp2 ) {
		printf("Cannot open file %s!\n", "AccountTmp.dat");
		return 0;
	}


	account_t buf;

	int found = 0;
	while (!feof(fp1)) {
		if (fread(&buf, sizeof(account_t), 1, fp1)) 
		{
			if (id == buf.id) 
			{
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(account_t), 1, fp2);
		}
	}

	fclose(fp1);
	fclose(fp2);
	
	if(remove("Account.dat"))
	{
		perror("remove");
	}	
	if(rename("AccountTmp.dat", "Account.dat")<0)
	{
		perror("rename");
		printf("改名失败！！！\n");
		return 0;
	}
	
	return found;
}
