#include "Play_UI.h"
#include "../service/Play.h"
#include "../Common/List.h"
#include "Schedule_UI.h"
#include <stdio.h>

/*
��ʶ����TTMS_SCU_Play_UI_MgtEnt
�������ܣ���ʾ�����Ŀ����
����˵������
����ֵ����
*/  
void Play_UI_MgtEntry(void)
{
	int id, num, i, PageNum;
	char choice;
	play_list_t head, pos;
	play_t buf;
	Pagination_t paging;
	
	List_Init(head, play_node_t);
	num = Play_Srv_FetchAll(head);
	paging.totalRecords = num;
	paging.offset = 0;
	paging.pageSize = 5;
	Paging_Locate_FirstPage(head, paging);
	PageNum = Pageing_TotalPages(paging);
		
	do	
	{

		system("cls"); 
		if(paging.totalRecords == 0)
			printf("���޼�¼\n");
		else
		{
			printf("=========================================================================================================\n");
			printf("========================================��Ŀ��Ϣ=========================================================\n");
			printf("ID  ����                ����     ��Ʒ����          �ȼ�      ʱ��      ��ӳʱ��       ��ӳʱ��       Ʊ��\n"); 
			printf("---------------------------------------------------------------------------------------------------------\n");
			Paging_ViewPage_ForEach(head, paging, play_node_t, pos, i)
			{
				printf("%d  %-20s", pos->data.id, pos->data.name);
				switch(pos->data.type)
				{
					case 1:
						printf("��Ӱ     ");
						break;
					case 2:
						printf("���     ");
						break;
					case 3:
						printf("���ֻ�   ");
						break;
				}
				printf("%-18s", pos->data.area);
				switch(pos->data.rating)
				{
					case 1:
						printf("��ͯ      ");
						break;
					case 2:
						printf("������    ");
						break;
					case 3:
						printf("����      ");
						break;
				}
				printf("%-10d",pos->data.duration);
				printf("%4d.%2d.%-7d",pos->data.start_date.year,pos->data.start_date.month,pos->data.start_date.day);
				printf("%4d.%2d.%-7d",pos->data.end_date.year,pos->data.end_date.month,pos->data.end_date.day);
				printf("%d\n",pos->data.price);
			}
			printf("========================�� %d �� ============================== �� %d/%d ҳ ================================\n",paging.totalRecords, Pageing_CurPage(paging),Pageing_TotalPages(paging));
		}
		
		printf("[p/P]��ʾ��һҳ\n");
		printf("[n/N]��ʾ��һҳ\n");
		printf("[a/A]����¾�Ŀ\n");
		printf("[u/U]�޸ľ�Ŀ\n");
		printf("[d/D]ɾ����Ŀ\n");
		printf("[s/S]�����ݳ�\n");
		printf("[r/R]�˳�\n");
		printf("---------------------------------------\n");
		printf("������Ӧ��ĸ��"); 
		fflush(stdin);
		choice = getchar();
		
		if((choice == 'a') || (choice == 'A'))
		{
			if (Play_UI_Add())
				{
					List_Init(head, play_node_t);
					paging.totalRecords = Play_Srv_FetchAll(head);
					List_Paging(head, paging, play_node_t);
					Paging_Locate_LastPage(head, paging, play_node_t);
				}	
		}else if((choice == 'u') || (choice == 'U'))
		{
			printf("�����Ŀid:");
			scanf("%d", &id);
			if (Play_UI_Modify(id))
				{	
					List_Init(head, play_node_t);
					paging.totalRecords = Play_Srv_FetchAll(head);
					List_Paging(head, paging, play_node_t);
				}
		}else if((choice == 'd') || (choice == 'D'))
		{
			printf("�����Ŀid:");
			scanf("%d", &id);
			if (Play_UI_Delect(id))
			{
				printf("ɾ���ɹ�\n");
				List_Init(head, play_node_t);
				paging.totalRecords = Play_Srv_FetchAll(head);
				List_Paging(head, paging, play_node_t);
			}
			else
				printf("ɾ��ʧ��\n");
		}else if((choice == 'p') || (choice == 'P'))
		{
			if(!Pageing_IsFirstPage(paging))
			{
				paging.totalRecords = Play_Srv_FetchAll(head);
				Paging_Locate_OffsetPage(head, paging, -1, play_node_t);
			}
			else
				printf("�޷���ת\n");
		}else if((choice == 'n') || (choice == 'N'))
		{
			if(!Pageing_IsLastPage(paging))
			{
				paging.totalRecords = Play_Srv_FetchAll(head);
				Paging_Locate_OffsetPage(head, paging, 1, play_node_t);
			}
			else
				printf("�޷���ת\n");
		}else if((choice == 's') || (choice == 'S'))
		{
			printf("�����Ŀid:");
			scanf("%d", &id);
			if(Play_Srv_FetchByID(id, &buf) == 0)
				printf("�þ�Ŀid������\n");
			else
				Schedule_UI_MgtEntry(id);
		}else if((choice == 'r') || (choice == 'R'))
			break;
	}while(1);
	List_Destroy(head, play_node_t);
	return ;
}

/*
��ʶ����TTMS_SCU_Play_UI_Add
�������ܣ���ʾ����¾�Ŀ����
����˵������
����ֵ������ֵ>0��ʾ��ӳɹ��� Ϊ0��ʾ���ʧ��
*/
int Play_UI_Add(void)
{
    char choice;
    play_t data;
    int newCount = 0, i;
     
    while(1)
    {
    	printf("===================================\n");
        printf("�����Ŀ���ƣ�");
        scanf("%s", data.name);
        printf("--------------------\n");
        while(1)
        {
	        printf("1.��Ӱ\n");
	        printf("2.���\n");
	        printf("3.���ֻ�\n");
	        printf("--------------------\n");
	        printf("ѡ���Ŀ����:");
	        scanf("%d", &data.type);
	        if(data.type <= 3 && data.type >= 1)
		        break;
	   }
        printf("�����Ŀ��Ʒ����:");
        scanf("%s", data.area);
        printf("--------------------\n");
        while(1)
    	{
	        printf("--------------------\n");
	        printf("1.�ʺ϶�ͯ\n");
	        printf("2.�ʺ�������\n");
	        printf("3.�ʺϳ���\n");
	        printf("--------------------\n");
	        printf("ѡ���µľ�Ŀ�ȼ���");
	        scanf("%d", &data.rating); 
	        if(data.rating <= 3 && data.rating >= 1)
	        	break;
        }
        scanf("%d", &data.rating);
		printf("�����Ŀʱ��(�Է���Ϊ��λ,��������):");
		scanf("%d",&data.duration); 
		printf("���뿪ʼ��ӳ����(��2020 2 22):");
		scanf("%d %d %d",&data.start_date.year,&data.start_date.month,&data.start_date.day);
		printf("�����ӳ��������(��2020 2 22):");
		scanf("%d %d %d",&data.end_date.year,&data.end_date.month,&data.end_date.day);
		printf("����Ʊ��:");
		scanf("%d",&data.price);
         
        i = Play_Srv_Add(&data); 
        if(i == 1)
        {
            newCount++;
            printf("��ӳɹ�\n");
        }
        else
            printf("���ʧ��\n");
         
        while(1)
        {
            printf("----------------------\n");
            printf("[a/A]�������\n");
            printf("[r/R]����\n");
            printf("----------------------\n");
            printf("������Ӧ��ĸ:");
            fflush(stdin);
            choice = getchar();
             
            if((choice == 'a') || (choice == 'A'))
                break;
            else if((choice == 'r') || (choice == 'R'))
                return newCount;
            else
                printf("��������ȷ����!\n");
        }
    }
}
 
/*
��ʶ����TTMS_SCU_Play_UI_Mod
�������ܣ��޸ľ�Ŀ
����˵����idΪ���޸ľ�Ŀid
����ֵ������1��ʾ�ɹ�������ʧ��
*/
int Play_UI_Modify(int id) 
{
    play_t buf;
    int i;
    char choice;
     
    i = Play_Srv_FetchByID(id, &buf);
    if(i != 1)
    {
        printf("��ȡ��Ϣʧ��\n");
        return -1;
	}
	
    system("cls"); 
    printf("========================================��Ŀ��Ϣ====================================================\n");
	printf("id:%d\n����:%s\n", buf.id, buf.name);
	switch(buf.type){
		case 1:printf("����:��Ӱ\n","��Ӱ");break;
		case 2:printf("����:���\n","���");break;
		case 3:printf("����:���ֻ�\n","���ֻ�");break;
	}
	printf("��Ʒ����:%s\n", buf.area);
	switch(buf.rating){
	case 1:printf("�ȼ�:��ͯ\n");break;
	case 2:printf("�ȼ�:������\n");break;
	case 3:printf("�ȼ�:����\n");break;
	}
	printf("ʱ����%d\n",buf.duration);
	printf("��ʼ��ӳ����:%d.%d.%d\n",buf.start_date.year,buf.start_date.month,buf.start_date.day);
	printf("������ӳ����:%d.%d.%d\n",buf.end_date.year,buf.end_date.month,buf.end_date.day);
	printf("Ʊ��:%d\n",buf.price);
	
     
    printf("-----------------------------------------------------------------------\n");
    printf("��Ŀ����(n/N)\t��Ŀ����(t/T)\t��Ŀ��Ʒ����(a/A)\t��Ŀ�ȼ�(r/R)\n");
    printf("��Ŀʱ��(d/D)\t��ʼ��ӳ����(s/S)\t������ӳ����(e/E)\t����Ʊ��(p/P)\n"); 
    printf("-----------------------------------------------------------------------\n");
    printf("ѡ����Ҫ�޸ĵ���:");
    fflush(stdin);
    choice = getchar();
     
    if((choice == 'n') || (choice == 'N'))
    {
        printf("�����µľ�Ŀ����:");
        memset(buf.name, 0, sizeof(buf.name));
        scanf("%s", buf.name);
    }else if((choice == 't') || (choice == 'T'))
    {
        while(1)
        {
	        printf("1.��Ӱ\n");
	        printf("2.���\n");
	        printf("3.���ֻ�\n");
	        printf("--------------------\n");
	        printf("ѡ���Ŀ����:");
	        scanf("%d", &buf.type);
	        if(buf.type <= 3 && buf.type >= 1)
		        break;
	   }
    }else if((choice == 'a') || (choice == 'A'))
    {
        printf("�����µľ�Ŀ��Ʒ����:");
        memset(buf.area, 0, sizeof(buf.area));
        scanf("%s", buf.area);
    }else if((choice == 'r') || (choice == 'R'))
    {
    	while(1)
    	{
	        printf("--------------------\n");
	        printf("1.�ʺ϶�ͯ\n");
	        printf("2.�ʺ�������\n");
	        printf("3.�ʺϳ���\n");
	        printf("--------------------\n");
	        printf("ѡ���µľ�Ŀ�ȼ���");
	        scanf("%d", &buf.rating); 
	        if(buf.rating <= 3 && buf.rating >= 1)
	        	break;
        }
    }else if((choice == 'd') || (choice == 'D'))
    {
        printf("�����µ�ʱ����");
        scanf("%d", &buf.duration); 
    }else if((choice == 's') || (choice == 'S'))
    {
		printf("�����µĿ�ʼ��ӳ����(��2021 10 22):");
		scanf("%d %d %d",&buf.start_date.year,&buf.start_date.month,&buf.start_date.day); 
    }else if((choice == 'e') || (choice == 'E'))
    {
		printf("�����µķ�ӳ��������(��2021 10 22):");
		scanf("%d %d %d",&buf.end_date.year,&buf.end_date.month,&buf.end_date.day);
    }else if((choice == 'p') || (choice == 'P'))
    {
        printf("�����µ�Ʊ�ۣ�");
        scanf("%d", &buf.price); 
	} 
    return Play_Srv_Modify(&buf);
}
 
/*
��ʶ����TTMS_SCU_Play_UI_Del
�������ܣ����� Play_Srv_DelectByIDɾ����Ŀ
����˵����idΪ��ɾ����Ŀid
����ֵ��1��ʾ�ɹ�������ʧ��
*/
int Play_UI_Delect(int id)
{
    return Play_Srv_DelectByID(id);
}
 
/*
��ʶ����TTMS_SCU_Play_UI_Qry
�������ܣ����� Play_Srv_FetchByID��ѯ
����˵����idΪ����ѯ��Ŀid
����ֵ��1��ʾ�ɹ�������ʧ�� 
*/
int Play_UI_Query(int id)
{
    play_t buf;
    int i = Play_Srv_FetchByID(id, &buf);
    if(i != 1)
        return -1;
    
    printf("============================================��Ŀ��Ϣ================================================\n");  
    printf("id:%d\n����:%d\n", buf.id, buf.name);
	switch(buf.type){
		case 1:printf("����:%s\n","��Ӱ");break;
		case 2:printf("����:%s\n","���");break;
		case 3:printf("����:%s\n","���ֻ�");break;
	}
	printf("��Ʒ����:%s\n", buf.area);
	switch(buf.rating){
	case 1:printf("�ȼ�:%s\n","��ͯ");break;
	case 2:printf("�ȼ�:%s\n","������");break;
	case 3:printf("�ȼ�:%s\n","����");break;
	}
	printf("ʱ����%d\n",buf.duration);
	printf("��ӳ����:%d.%d.%d\n",buf.start_date.year,buf.start_date.month,buf.start_date.day);
	printf("��ӳ����:%d.%d.%d\n",buf.end_date.year,buf.end_date.month,buf.end_date.day);
	printf("Ʊ��:%d\n",buf.price);
    printf("====================================================================================================\n"); 
    return 1;
}
