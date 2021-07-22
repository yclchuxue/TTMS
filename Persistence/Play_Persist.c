#include "Play_Persist.h"
#include "../Service/Play.h"
#include "../Common/List.h"
#include <stdio.h>
#include <stdlib.h>

/*
��ʶ����TTMS_SCU_Play_Perst_SelAll
�������ܣ����ļ�Play.dat���������о�Ŀ��Ϣ�����ص�����list��
����˵����listΪplay_list_t����ָ�룬��ʾ��¼���о�Ŀ��Ϣ������ͷָ��
����ֵ������ֵ>0��ʾ�����Ŀ������ Ϊ-1��ʾʧ��
*/
int Play_Perst_SelectAll(play_list_t list)
{
    FILE *fp;
    play_t data;
    int recCount = 0;
     
    fp = fopen("Play.dat", "rb");
    if(fp == NULL)
    {
        printf("�ļ�Play.dat ������\n");
        return -1;
    }
     
    while(!feof(fp))
    {
        if(fread(&data, sizeof(play_t), 1, fp))
        {
        	play_list_t newNode = (play_list_t) malloc (sizeof(play_node_t));
	        newNode->data = data;
	        List_AddTail(list, newNode);
	        recCount++; 
    	}
    }
     
    fclose(fp);
    return recCount;
} 
 
/*
��ʶ����TTMS_SCU_Play_Perst_Insert
�������ܣ����Ŀ�����ļ�Play.dat��ĩβ���һ���µľ�Ŀ��Ϣ
����˵����dataΪplay_t����ָ�룬��ʾ���洢�ľ�Ŀ��Ϣ
����ֵ�����ͣ�����1��ʾ�洢��Ŀ�ɹ�������洢��Ŀʧ�� 
*/
int Play_Perst_Insert(play_t *data)
{
    FILE *fp;
    int rtn;
     
    data->id = EntKey_Perst_GetNewKeys("Play", 1);
    if(data->id == 0)
    {
        printf("��ȡ����ʧ��\n");
        return -1;
    }
     
    fp = fopen("Play.dat", "ab");
    if(fp == NULL)
    {
        printf("���ļ�Play.datʧ��\n");
        return -1;
    }
    
    rtn = fwrite(data, sizeof(play_t), 1, fp);
    fclose(fp);
     
    return 1;
}
 
/*
��ʶ����TTMS_SCU_Play_Perst_Update
�������ܣ������ļ�play.dat�е�һ����Ŀ��Ϣ
����˵����dataΪplay_t����ָ�룬��ʾ���洢�ľ�Ŀ��Ϣ
����ֵ������1��ʾ���¾�Ŀ�ɹ��������ʾʧ��
*/
int Play_Perst_Update(const play_t *data)
{
    FILE *fp;
    play_t buf;
    int found = 0;
     
    fp = fopen("Play.dat", "rb+");
    if(fp == NULL)
    {
        printf("�ļ�Play.dat ������\n");
        return found;
    }
	
	 
    while(!feof(fp))
    {
        if(fread(&buf, sizeof(buf), 1, fp))
        {
        	if(buf.id == data->id)
	        {
	            fseek(fp, -sizeof(play_t), SEEK_CUR);
	            fwrite(data, sizeof(play_t), 1, fp);
	            found = 1;
	            break;
	        }
		}
    }
    fclose(fp);
     
    return found;
}
 
/*
��ʶ����TTMS_SCU_Play_Perst_RemByID
�������ܣ�ɾ��Play.dat��ָ��ID�ľ�Ŀ��Ϣ
����˵��: idΪ��ɾ����Ŀid
����ֵ������1b=��ʾ�ɹ�������ʧ��
*/
int Play_Perst_RemByID(int id)
{
    FILE *fp1;
    FILE *fp2;
    play_t buf;
    
    rename("Play.dat", "PlayTmp.dat");
    
    fp1 = fopen("PlayTmp.dat", "rb");
    fp2 = fopen("Play.dat", "wb");
    if(fp1 == NULL)
    {
        printf("�ļ�Play.dat��ʧ��\n");
        return 0;
    }
     
    while(!feof(fp1))
    {
        if(fread(&buf, sizeof(play_t), 1, fp1))
        {
        	if(buf.id != id)
            	fwrite(&buf, sizeof(play_t), 1, fp2);
		}
    }
    
    fclose(fp1);
    fclose(fp2);
   
    remove("playTmp.dat");
    
    return 1;
}
 
/*
��ʶ����TTMS_SCU_Play_Perst_SelByID
�������ܣ����ļ�Play.dat������ָ��id����Ϣ���ص�buf��
����˵����idΪ�����Ŀid,buf���id����Ϣ
����ֵ: 1��ʾ�ɹ�������ʧ��
*/
int Play_Perst_SelectByID(int id, play_t *buf)
{
    FILE *fp;
    play_t data;
     
    fp = fopen("Play.dat", "rb");
    if(fp == NULL)
    {
        printf("�ļ�Play.dat��ʧ��\n");
        return -1;
    }
     
    while(!feof(fp))
    {
        if(fread(&data, sizeof(play_t), 1, fp))
        {
        	if(id == data.id)
	        {
	            *buf = data;
	            return 1;
	        }
		}
    }
    fclose(fp);
    return 0;
}

int Play_Perst_SelectByName(play_list_t list,char condt[])
{
	play_node_t *newNode;
	play_t data;

	int rtn = 0;
	int i,j=0;

	assert(NULL != list);

	List_Free(list,play_node_t);

	FILE *fp = fopen("Play.dat","r");
	if(NULL == fp)   return 0;

	while(!feof(fp))
	{
		int flag = 0;
		if(fread(&data,sizeof(play_t),1,fp))
		{
			newNode = (play_node_t *)malloc(sizeof(play_node_t));
			newNode->data = data;

			if(!newNode)
			{
				printf( "Waring,Memory OverFlow!!!\nCannot Load more Data into memory!!!\n");
				break;
			}
			char s1[100];
			char s2[100];
			
			for(i;i < strlen(condt);i++)
			{
				s1[i] = condt[i];
			}
			for(i;i < strlen(newNode->data.name);i++)
			{
				s2[i] = newNode->data.name[i];
			}




			for(i;i < strlen(s1);i++)
			{
				s1[i] = toupper(s1[i]);
			}
			for(i;i < strlen(s2);i++)
			{
				s2[i] = toupper(s2[i]);
			}
			int j;
			for(i;i < strlen(s2);i++)
			{
				int temp = i;
				for(j;j < strlen(s1);j++)
				{
					if(s2[temp] == s1[j])
					{
						temp++;
						if(j == (strlen(s1) - 1))
							flag = 1;
						continue;
					}
					else
					{
						break;
					}



				}

			}
			if(flag == 1)
			{
				List_AddTail(list,newNode);
				rtn++;
			}
			if(!strcmp(newNode->data.name,condt))    
			{
				List_AddTail(list,newNode);
				rtn++;
			}
		}
	}
	
	fclose(fp);

	return rtn;

}
