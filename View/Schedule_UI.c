#include "Schedule_UI.h"
#include "Play_UI.h"

#include "../Common/List.h"
#include "../Service/Schedule.h"
#include "../Service/Play.h"
#include "../Service/Studio.h"
#include "../Persistence/EntityKey_Persit.h"

#include <stdio.h>
#include <stdlib.h>

//��ʾ��ID��Ϊplay_id�ľ�Ŀ������������ݳ��ƻ������ṩ����ɾ�����ݳ��ƻ��Ĺ���
void Schedule_UI_MgtEntry(int play_id)
{
	int i,ID;
    char choice;
	schedule_t *buf = (schedule_t *)malloc(sizeof(schedule_t));
    schedule_list_t head;
	schedule_node_t *pos;
	Pagination_t paging;
    play_t *buf_1 = (play_t *)malloc(sizeof(play_t));
    int ret = Play_Srv_FetchByID(play_id, buf_1);            //��ȡ��Ŀ��Ϣ
    if(ret != 1)
    {
        printf("Play_Srv_FetchByID ��ȡ��Ŀ��Ϣʧ�ܣ�����\n");
        exit(1);
    }

    List_Init(head, schedule_node_t);                   //��ʼ���ݳ��ƻ�����

    paging.offset = 0;
	paging.pageSize = 5;
	
	paging.totalRecords = Schedule_Srv_FetchByPlay(head, play_id);  //��ʾ��ID��Ϊplay_id�ľ�Ŀ������������ݳ��ƻ�
	Paging_Locate_FirstPage(head, paging);              //����ҳ��paging��λ������list�ĵ�һҳ
	do 
	{
		
		system("cls");            //���ҳ��
		printf("\n==================================================================================\n");
		printf("====================================�ݳ��ƻ�======================================\n");
		printf("\n");
		printf("%-10s %-10s %-15s %-15s %-15s %-15s \n", "�ƻ�", "��Ŀ", "�ݳ���","��ӳ����","��ӳʱ��","��λ��");
		printf("----------------------------------------------------------------------------------\n");
		//��ʾ����
		//��������list����ҳ��paging,ʹ��ָ��pos���α���pagingָ��ҳ����ÿ�����
		//����iΪ���ͼ���������
		if(paging.totalRecords > 0)
		{
			//printf("BBBBB\n");
			//printf("BBBB %d BBB\n", paging.totalRecords);
			Paging_ViewPage_ForEach(head, paging, schedule_node_t, pos, i)
			{
				//printf("AAAA\n");
				printf("%-10d  %-10d  %-10d    %d��%2d��%2d��    %d:%d    %10d\n", pos->data.id,
				pos->data.play_id, pos->data.studio_id, pos->data.date.year,pos->data.date.month,pos->data.date.day,pos->data.time.hour,pos->data.time.minute/*,pos->data.time.second*/,pos->data.seat_count);
			}
			printf("\n------- �� %2d �� ----------------------------------- �� %2d/%2d ҳ ----------\n",
			paging.totalRecords, Pageing_CurPage(paging),
			Pageing_TotalPages(paging));
		}
		else
		{
				printf("\t���޼�¼!!\n");
		}
		printf("\n********************************************************************************\n" );
		printf("[P]��һҳ\n");
		printf("[N]��һҳ\n");
		printf("[A]���\n");
		printf("[D]ɾ��\n");
		printf("[U]�޸�\n");
		printf("[T]��ѯ\n");
		printf("[R]����\n");
		printf("\n================================================================================\n");
		printf("\t����ĸѡ����:");
		fflush(stdin);
		scanf("%c", &choice);

		switch (choice) 
		{
			case 'a':       //���
			case 'A':
				if(Schedule_UI_Add(play_id)) //����ӳɹ����������һҳ��ʾ
				{
					List_Init(head, schedule_node_t);                   //��ʼ���ݳ��ƻ�����
					paging.totalRecords = Schedule_Srv_FetchAll(head);
					Paging_Locate_LastPage(head, paging, schedule_node_t);
				}
				break;
			case 'd':       //ɾ��
			case 'D': 
			{
				printf("\n==================================================================\n");
				printf("\t\tɾ���ݳ��ƻ�\n");
				printf("\n------------------------------------------------------------------\n");
				printf("������Ҫɾ�����ݳ��ƻ�ID:");
				scanf("%d",&ID);
				system("cls");
				if (Schedule_UI_Delete(ID)) 	//������������
				{
					
					//List_Init(head, schedule_node_t);                   //��ʼ���ݳ��ƻ�����
					//paging.totalRecords = Schedule_Srv_FetchAll(head);
					//Paging_Locate_FirstPage(head, paging);              //����ҳ��paging��λ������list�ĵ�һҳ
					List_Init(head, schedule_node_t);                   //��ʼ���ݳ��ƻ�����
					paging.totalRecords = Schedule_Srv_FetchAll(head);
					List_Paging(head, paging, schedule_node_t);
				}
				return ;
				break;
			}
			case 'u':        //�����ݳ��ƻ�
			case 'U': 
			{
				printf("\n==================================================================\n");
				printf("\t\t�����ݳ��ƻ�\n");
				printf("------------------------------------------------------------------\n");
				printf("������Ҫ�޸ĵ��ݳ��ƻ�ID:");
				scanf("%d",&ID);
				if (Schedule_UI_Modify(ID)) 	//������������
				{
					List_Init(head, schedule_node_t);                   //��ʼ���ݳ��ƻ�����
					paging.totalRecords = Schedule_Srv_FetchAll(head);
					List_Paging(head, paging, schedule_node_t);
				}
				break;
			}
			case 't':        //��ѯ�ݳ��ƻ�
			case 'T':
			{
				printf("\n==================================================================\n");
				printf("\t��ѯ�ݳ��ƻ�\n");
				printf("------------------------------------------------------------------\n");
				printf("������Ҫ��ѯ���ݳ��ƻ�ID:");
				scanf("%d",&ID);
				if(Schedule_Srv_FetchByID(ID, buf))        //�鵽�˸��ݳ��ƻ�
				{
					Ticket_UI_MgtEntry(ID);	
				}
				else                                       //û�в�ѯ�����ݳ��ƻ�
				{
					printf("\tδ��ѯ�����ݳ��ƻ�ID");
				}
				break;
			}
			case 'p':       //��һҳ
			case 'P':
				if (!Pageing_IsFirstPage(paging)) 
				{
					Paging_Locate_OffsetPage(head, paging, -1, schedule_node_t);
				}
				break;
			case 'n':        //��һҳ
			case 'N':
				if (!Pageing_IsLastPage(paging)) 
				{
					Paging_Locate_OffsetPage(head, paging, 1, schedule_node_t);
				}
				break;

		}
	} while (choice != 'r' && choice != 'R');

	//�ͷ�����ռ�
	List_Destroy(head, schedule_node_t);
}

//���ռ���������ݳ��ƻ���Ϣ��ͨ������ҵ���߼��㺯��������ݳ��ƻ�
int Schedule_UI_Add(int play_id)
{
	schedule_t rec;
	int newRecCount = 0;
	char choice;

	do 
	{
		system("cls");
        printf("\n==================================================================\n");
    	printf("\t��ӷ�ӳ��\n");
        printf("------------------------------------------------------------------\n");
		fflush(stdin);
		rec.id = EntKey_Perst_GetNewKeys("schedule", 1);
		printf("\t��ĿID:");
		scanf("%d",&(rec.play_id));                                                           //�����ĿID
		printf("\t�ݳ���ID:");
		scanf("%d", &(rec.studio_id));                                                        //�����ݳ���ID
		getchar();
		studio_t buf;
		int i = 0; 
		while( !Studio_Srv_FetchByID(rec.studio_id, &buf))
		{
			i++;
			if(i >= 3)
			{
				return 0;
			}
			printf("���ݳ���������!\n");
			printf("�����������ݳ���ID:");
			setbuf(stdin,NULL);
			scanf("%d",&rec.studio_id);
			getchar();
		}
		printf("\t��ӳ����:");
		scanf("%d%d%d", &(rec.date.year),&(rec.date.month),&(rec.date.day));                  //�����ӳ����
		printf("\t��ӳʱ��:");                    
		getchar();
		scanf("%d%d",&(rec.time.hour),&(rec.time.minute)/*,&(rec.time.second)*/);              //�����ӳʱ��
		rec.seat_count=buf.seatsCount;
        printf("\n==================================================================\n");

		if (Schedule_Srv_Add(&rec))            //�����߼��㺯��ʵ����ӹ���
		{
			rec.seat_count += 1;
			newRecCount += 1;
			printf("\t��ӳɹ���\n");
		} 
		else
			printf("\t���ʧ��!\n");
			
        printf("****************************************************************\n" );
		printf("[A]�������\n");
		printf("[R]����\n");
		fflush(stdin);
		scanf("%c", &choice);
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}


//�����ݳ��ƻ���IDֵ������ҵ���߼��㺯���޸��ݳ��ƻ�����
int Schedule_UI_Modify(int id)
{
	int rtn = 0;
	int sid;
	char choice;
	schedule_t data;
	studio_t sdata;

	if(Schedule_Srv_FetchByID(id, &data)) //��ID��Ӧ����Ϣ����data��
	{
		while(1)
		{
			system("cls");
			printf("\n==================================================================\n");
			printf("\t�޸� �ݳ��ƻ�\n");
       		printf("\n==================================================================\n");
    		printf("%5s \t %5s\t%8s \t %10s \t %10s \t %10s \n", "ID", "��ĿID", "�ݳ���ID",
			 "��ӳ����","��ӳʱ��","��λ��");
			printf("------------------------------------------------------------------\n");
			
			printf("%5d %5d %5d ", data.id, data.play_id,data.studio_id);
			
			printf("%4d-%02d-%02d ",data.date.year, data.date.month, data.date.day);
			printf("%2d:%02d:%02d ", data.time.hour, data.time.minute, data.time.second);
			printf("%5d\n", data.seat_count);

			printf("\n==================================================================\n");
			printf("[A]�޸��ݳ���\n");
			printf("[B]�޸ķ�ӳʱ��");
			printf("[R]����\n");
			printf("\n==================================================================\n");
			printf("\t��ѡ��:");
			setbuf(stdin,NULL);
			scanf("%c", &choice);

			if(choice == 'r' || choice == 'R')
			{
				break;
			}
			switch (choice)
			{
				case 'a':
				case 'A':
				{
					printf("\t�������ݲ���ID��");
					while(1)
					{
						if(scanf("%d", &sid) == 1)
						{
							while(!Studio_Srv_FetchByID(sid, &sdata))
							{
								printf("\n\tIDΪ%d���ݲ��������ڣ����������룺", sid);
								printf("�����������ݳ���ID:");
								setbuf(stdin, NULL);
								scanf("%d", &sid);
							}

							data.studio_id = sdata.id;  //�ݲ���ID
							data.seat_count = sdata.seatsCount;  //�ݲ�����λ����

							break;
						}
						else
						{
							printf("\t���������������������룺");
						}
					}

					break;
				}
				
				case 'b':
				case 'B':
				{
					printf("\t�������ӳ���ڣ����� �� �գ�");
					while(1)
					{
						if(scanf("%d %d %d", &(data.date.year), &(data.date.month), &(data.date.day)) == 3)
						{
							if(data.date.month > 12 || data.date.day > 31)
							{
								printf("\t��������������������������룺");
								continue;
							}
							if(DateCmp(data.date, DateNow()) < 0)
							{
								printf("\t������������ڽ���֮ǰ,���������룺");
								continue;
							}

							break;
						}
						else
						{
							printf("\t���������������������룺");
						}
					}

					printf("\t�������ӳʱ��:(ʱ ��) ");
					while(1)
					{
						if(scanf("%d %d", &(data.time.hour), &(data.time.minute)) == 2)
						{
							data.time.second = 0;
							if(data.time.hour > 24 || data.time.second > 60)
							{
								printf("\t�������ʱ�����������������: ");
								continue;
							}

							break;
						}
						else
						{
							printf("\t���������������������룺");
						}
					}
					break;
				}
			}//switch

			if(Schedule_Srv_Modify(&data))
			{
				printf("\t�޸ĳɹ�!\n");
				rtn = 1;
			}
			else
			{
				printf("\t�޸�ʧ�ܣ�\n");
			}

			printf("\t������������޸ģ�\n");

			getchar();
		}
	}
	else
	{
		printf("\tδ�ҵ�IDδ%d���ݳ��ƻ�!\n", id);
		printf("\t����������أ�\n");

		getchar();
		return 0;
	}

	return rtn;
}


//���ݲ���ID������ҵ���߼��㺯��ɾ���ݳ��ƻ�
int Schedule_UI_Delete(int id)
{
	int rtn = 0;

	rtn = Schedule_Srv_DeleteByID(id);

	return rtn;
}

void Schedule_UI_ListAll(void) 
{
	int i, id;
	char choice;

	schedule_list_t head;
	schedule_node_t* p;
	Pagination_t paging;

	List_Init(head, schedule_node_t);

	paging.offset = 0;
	paging.pageSize = SCHEDULE_PAGE_SIZE;

	//��������
	paging.totalRecords = Schedule_Srv_FetchAll(head);

	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		printf("\n==================================================================\n");
		printf("\t�ݳ��ƻ� �б�\n");
		printf("------------------------------------------------------------------\n");
		printf("%5s%5s%8s%10s%10s%10s\n", "�ƻ�", "��Ŀ", "�ݳ���",
			"��ӳ����", "��ӳʱ��", "��λ��");
		//��ʾ����
		Paging_ViewPage_ForEach(head, paging, schedule_node_t, p, i) {
			printf("%2d %2d %2d", p->data.id, p->data.play_id, p->data.studio_id);

			printf("%4d-%02d-%02d ", p->data.date.year, p->data.date.month, p->data.date.day);
			printf("%2d:%02d:%02d ", p->data.time.hour, p->data.time.minute, p->data.time.second);
			printf("%5d\n", p->data.seat_count);
		}


		printf("\n-------- �� %2d �� --------------------------- %2d/%2d ҳ --------\n",
			paging.totalRecords, Pageing_CurPage(paging),
			Pageing_TotalPages(paging));
		printf("******************************************************************\n");
		printf("[P]��ҳ\n");
		printf("[N]��ҳ\n");
		printf("[R]����\n");

		printf("\n==================================================================\n");
		printf("\t��ѡ��:");

		setbuf(stdin,NULL);
		scanf("%c", &choice);


		switch (choice) 
		{


		case 'p':
		case 'P':
			if (!Pageing_IsFirstPage(paging)) 
			{
				Paging_Locate_OffsetPage(head, paging, -1, schedule_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (!Pageing_IsLastPage(paging)) 
			{
				Paging_Locate_OffsetPage(head, paging, 1, schedule_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	//�ͷ�����ռ�
	List_Destroy(head, schedule_node_t);
}

//����ID�Ų�ѯ�ݳ��ƻ���Ϣ,��������ҵľ�ĿID�ţ�������ҵ��ľ�Ŀ��Ϣ��
//���� 0��ʾδ�ҵ���1��ʾ�ҵ���
int Schedule_UI_Query(int id) 
{

	schedule_t rec;
	/*int rtn = 0,seatcount;*/

	/*Load record*/
	if (!Schedule_Srv_FetchByID(id, &rec)) {
		printf("\t��ӳ�ƻ�������\n���س�������\n");
		getchar();
		return 0;
	}

	printf("\n==================================================================\n");
	printf("\t���·�ӳ�ƻ�\n");
	printf("------------------------------------------------------------------\n");
	printf("\t�ݳ��ƻ�ID:%d\n", rec.id);
	printf("\t��ĿID:%d\n", rec.play_id);
	printf("\t�ݳ���ID:%d\n", rec.studio_id);
	printf("\t��ӳ����:%d��%2d��%2d��\n", rec.date.year, rec.date.month, rec.date.day);
	printf("\t��ӳʱ��:%2d:%2d\n", rec.time.hour, rec.time.minute/*,rec.time.second*/);
	printf("\t��λ��:%d\n", rec.seat_count);
	return 0;
}
