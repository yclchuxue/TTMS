#include "Schedule_Persist.h"
#include "../Service/Schedule.h"
#include "../Common/List.h"
//#include "../Service/ticket.h"
//#include "../Persist/Common_Persist.h"
//#include "../View/Ticket_UI.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>


static const char SCHEDULE_DATA_FILE[] = "Schedule.dat";
static const char SCHEDULE_DATA_TEMP_FILE[] = "ScheduleTmp.dat";
static const char SCHEDULE_KEY_NAME[]="Schedule";

//����ID�����ݳ��ƻ�
//���ݳ��ƻ������ļ���������ָ����Ŀ��������ݳ��ƻ���Ϣ��������list
int Schedule_Perst_SelectByPlay(schedule_list_t list, int play_id)
{
	FILE *fp = fopen("Schedule.dat", "rb");
	if (NULL == fp) {
		return 0;
	}

	List_Free(list, schedule_node_t);
	schedule_t data;
	int recCount = 0;
	schedule_node_t *newNode;

	while (!feof(fp)) {
		if (fread(&data, sizeof(schedule_t), 1, fp)) {
			newNode = (schedule_node_t*)malloc(sizeof(schedule_node_t));
			if (!newNode) {
				printf("Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
				break;
			}
			if (data.play_id==play_id)
			{
				printf("%d\n", data.id);
				newNode->data = data;
				List_AddTail(list, newNode);
				recCount++;
			}
		}
	}
	fclose(fp);
	return recCount;
}

//�������ݳ��ƻ�
//���ݳ��ƻ��ļ�Schedule.dat��ĩβ���һ�����ݳ��ƻ�
int Schedule_Perst_Insert(schedule_t * date)
{
	//assert(NULL!=data);
	FILE *fp=fopen("Schedule.dat", "ab");
	int rtn=0;
	if (NULL==fp)
	{
		printf("�޷����ļ� %s!\n", "Schedule.dat");
		return 0;
	}
	rtn=fwrite(date, sizeof(schedule_t),1,fp);
	fclose(fp);
	return rtn;
}

//�������ݳ��ƻ���Ϣ������һ����������listΪ�ݳ��ƻ���Ϣ�����ͷָ��
//��� ��ʾ��������ʱ������ռ�ʧ�ܣ����ز��ҵ��ļ�¼��Ŀ
int Schedule_Perst_SelectAll(schedule_list_t list) {
	schedule_node_t* newNode;
	schedule_t data;
	int recCount = 0;
	char *new = "Schedule.dat";

	assert(NULL != list);

	if (access(new, 0))
	{
		printf("�޷����ļ� %s!\n", new);
		return 0;
	}

	List_Free(list, schedule_node_t);

	FILE* fp = fopen(new, "rb");
	if (NULL == fp)
	{
		printf("�޷����ļ� %s!\n", new);
		return 0;
	}

	while (!feof(fp))
	{
		if (fread(&data, sizeof(schedule_t), 1, fp))
		{
			newNode = (schedule_node_t*)malloc(sizeof(schedule_node_t));
			if (!newNode)
			{
				printf("���棬�ڴ����!!!\n���ܽ���������ݼ��ص��ڴ���!!!\n");
				break;
			}
			newNode->data = data;
			List_AddTail(list, newNode);
			recCount++;
		}
	}
	fclose(fp);
	return recCount;
}



//�����ݳ��ƻ�
//�����ݳ��ƻ������ļ�Schedule.dat�е�һ���ݳ��ƻ���Ϣ
int Schedule_Perst_Update(const schedule_t * date)
{
	int ret = 0;
	FILE *fd = fopen("Schedule.dat", "rb+");
	if(fd == NULL)
	{
		printf("�޷����ļ� %s !\n", "Schedule.dat");
		return 0;
	}

	schedule_t buf;
	
	while(!feof(fd))
	{
		if(fread(&buf, sizeof(schedule_t), 1, fd));
		{
			if(buf.id == date->id)
			{
				fseek(fd, -sizeof(schedule_t), SEEK_CUR);
				fwrite(date, sizeof(schedule_t), 1, fd);
				ret = 1;
				break;
			}
		}
	}

	fclose(fd);
	return ret;
}

int Schedule_Perst_SelectByID(int ID, schedule_t *buf)
{
	char *new = "Schedule.dat";
	assert(NULL != buf);

	FILE *fp = fopen(new, "rb");
	if (NULL == fp) {
		return 0;
	}

	schedule_t data;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&data, sizeof(schedule_t), 1, fp)) {
			if (ID == data.id) {
				*buf = data;
				found = 1;
				break;
			};

		}
	}
	fclose(fp);

	return found;
}


//����IDȥ���ݳ��ƻ�
//ȥ���ݳ��ƻ������ļ�Schedule.dat��ָ��ID���ݳ��ƻ�
int Schedule_Perst_RemByID(int id)
{
	int ret = 0;
	char cmd[100];
	schedule_t buf;
	char *new = "Schedule.dat";
	char *old = "ScheduleTmp.dat";

	FILE *fp = fopen("Schedule.dat", "rb");
	FILE *fp1 = fopen("ScheduleTmp.dat", "wab");
	if(fp == NULL)
	{
		printf("%s�޷��򿪣�\n", new);
		return 0;
	}

	if(fp1 == NULL)
	{
		printf("%s�޷��򿪣�\n", old);
		return 0;
	}
	
	while(!feof(fp))
	{
		if(fread(&buf, sizeof(schedule_t), 1, fp))
		{
			if(buf.id != id)
			{
				//printf("AAA\n");
				fwrite(&buf, sizeof(schedule_t), 1, fp1);
			}
		}
	}

	fclose(fp);
	fclose(fp1);
	
	remove("Schedule.dat");

	if(rename("ScheduleTmp.dat", "Schedule.dat")<0)
	{
		printf("����ʧ�ܣ�����\n");
	}
	return ret;
}


int Schedule_Perst_SelectByID_ticket(int id,schedule_t *buf)
{
	assert(NULL != buf);

	int found = 0;
	schedule_t data;
	FILE *fp = fopen(SCHEDULE_DATA_FILE,"rb");
	

	if(NULL == fp)
	{
		printf( "the file cannot open!\n");
		return 0;
	}
	while(!feof(fp))
	{
		if(fread(&data,sizeof(schedule_t),1,fp))
		{
			if(data.id == id)
			{
				*buf = data;
				found = 1;
				break;
			}
		}
	}
	
	fclose(fp);

	return found;
}
