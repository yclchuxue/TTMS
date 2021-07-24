#include "Schedule_UI.h"
#include "Play_UI.h"

#include "../Common/List.h"
#include "../Service/Schedule.h"
#include "../Service/Play.h"
#include "../Service/Studio.h"
#include "../Persistence/EntityKey_Persit.h"

#include <stdio.h>
#include <stdlib.h>

//显示与ID号为play_id的剧目相关联的所有演出计划，并提供增，删，改演出计划的功能
void Schedule_UI_MgtEntry(int play_id)
{
	int i,ID;
    char choice;
	schedule_t *buf = (schedule_t *)malloc(sizeof(schedule_t));
    schedule_list_t head;
	schedule_node_t *pos;
	Pagination_t paging;
    play_t *buf_1 = (play_t *)malloc(sizeof(play_t));
    int ret = Play_Srv_FetchByID(play_id, buf_1);            //获取剧目信息
    if(ret != 1)
    {
        printf("Play_Srv_FetchByID 获取剧目信息失败！！！\n");
        exit(1);
    }

    List_Init(head, schedule_node_t);                   //初始化演出计划链表

    paging.offset = 0;
	paging.pageSize = 5;
	
	paging.totalRecords = Schedule_Srv_FetchByPlay(head, play_id);  //显示与ID号为play_id的剧目相关联的所有演出计划
	Paging_Locate_FirstPage(head, paging);              //将分页器paging定位到链表list的第一页
	do 
	{
		
		system("cls");            //清空页面
		printf("\n==================================================================================\n");
		printf("====================================演出计划======================================\n");
		printf("\n");
		printf("%-10s %-10s %-15s %-15s %-15s %-15s \n", "计划", "剧目", "演出厅","放映日期","放映时间","座位数");
		printf("----------------------------------------------------------------------------------\n");
		//显示数据
		//对于链表list及分页器paging,使用指针pos依次遍历paging指向页面中每个结点
		//这里i为整型计数器变量
		if(paging.totalRecords > 0)
		{
			//printf("BBBBB\n");
			//printf("BBBB %d BBB\n", paging.totalRecords);
			Paging_ViewPage_ForEach(head, paging, schedule_node_t, pos, i)
			{
				//printf("AAAA\n");
				printf("%-10d  %-10d  %-10d    %d年%2d月%2d日    %d:%d    %10d\n", pos->data.id,
				pos->data.play_id, pos->data.studio_id, pos->data.date.year,pos->data.date.month,pos->data.date.day,pos->data.time.hour,pos->data.time.minute/*,pos->data.time.second*/,pos->data.seat_count);
			}
			printf("\n------- 共 %2d 项 ----------------------------------- 第 %2d/%2d 页 ----------\n",
			paging.totalRecords, Pageing_CurPage(paging),
			Pageing_TotalPages(paging));
		}
		else
		{
				printf("\t暂无记录!!\n");
		}
		printf("\n********************************************************************************\n" );
		printf("[P]上一页\n");
		printf("[N]下一页\n");
		printf("[A]添加\n");
		printf("[D]删除\n");
		printf("[U]修改\n");
		printf("[T]查询\n");
		printf("[R]返回\n");
		printf("\n================================================================================\n");
		printf("\t按字母选择功能:");
		fflush(stdin);
		scanf("%c", &choice);

		switch (choice) 
		{
			case 'a':       //添加
			case 'A':
				if(Schedule_UI_Add(play_id)) //新添加成功，跳到最后一页显示
				{
					List_Init(head, schedule_node_t);                   //初始化演出计划链表
					paging.totalRecords = Schedule_Srv_FetchAll(head);
					Paging_Locate_LastPage(head, paging, schedule_node_t);
				}
				break;
			case 'd':       //删除
			case 'D': 
			{
				printf("\n==================================================================\n");
				printf("\t\t删除演出计划\n");
				printf("\n------------------------------------------------------------------\n");
				printf("请输入要删除的演出计划ID:");
				scanf("%d",&ID);
				system("cls");
				if (Schedule_UI_Delete(ID)) 	//从新载入数据
				{
					
					//List_Init(head, schedule_node_t);                   //初始化演出计划链表
					//paging.totalRecords = Schedule_Srv_FetchAll(head);
					//Paging_Locate_FirstPage(head, paging);              //将分页器paging定位到链表list的第一页
					List_Init(head, schedule_node_t);                   //初始化演出计划链表
					paging.totalRecords = Schedule_Srv_FetchAll(head);
					List_Paging(head, paging, schedule_node_t);
				}
				return ;
				break;
			}
			case 'u':        //更新演出计划
			case 'U': 
			{
				printf("\n==================================================================\n");
				printf("\t\t更新演出计划\n");
				printf("------------------------------------------------------------------\n");
				printf("请输入要修改的演出计划ID:");
				scanf("%d",&ID);
				if (Schedule_UI_Modify(ID)) 	//从新载入数据
				{
					List_Init(head, schedule_node_t);                   //初始化演出计划链表
					paging.totalRecords = Schedule_Srv_FetchAll(head);
					List_Paging(head, paging, schedule_node_t);
				}
				break;
			}
			case 't':        //查询演出计划
			case 'T':
			{
				printf("\n==================================================================\n");
				printf("\t查询演出计划\n");
				printf("------------------------------------------------------------------\n");
				printf("请输入要查询的演出计划ID:");
				scanf("%d",&ID);
				if(Schedule_Srv_FetchByID(ID, buf))        //查到了该演出计划
				{
					Ticket_UI_MgtEntry(ID);	
				}
				else                                       //没有查询到该演出计划
				{
					printf("\t未查询到该演出计划ID");
				}
				break;
			}
			case 'p':       //上一页
			case 'P':
				if (!Pageing_IsFirstPage(paging)) 
				{
					Paging_Locate_OffsetPage(head, paging, -1, schedule_node_t);
				}
				break;
			case 'n':        //下一页
			case 'N':
				if (!Pageing_IsLastPage(paging)) 
				{
					Paging_Locate_OffsetPage(head, paging, 1, schedule_node_t);
				}
				break;

		}
	} while (choice != 'r' && choice != 'R');

	//释放链表空间
	List_Destroy(head, schedule_node_t);
}

//接收键盘输入的演出计划信息，通过调用业务逻辑层函数添加新演出计划
int Schedule_UI_Add(int play_id)
{
	schedule_t rec;
	int newRecCount = 0;
	char choice;

	do 
	{
		system("cls");
        printf("\n==================================================================\n");
    	printf("\t添加放映室\n");
        printf("------------------------------------------------------------------\n");
		fflush(stdin);
		rec.id = EntKey_Perst_GetNewKeys("schedule", 1);
		printf("\t剧目ID:");
		scanf("%d",&(rec.play_id));                                                           //输入剧目ID
		printf("\t演出厅ID:");
		scanf("%d", &(rec.studio_id));                                                        //输入演出厅ID
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
			printf("该演出厅不存在!\n");
			printf("请重新输入演出厅ID:");
			setbuf(stdin,NULL);
			scanf("%d",&rec.studio_id);
			getchar();
		}
		printf("\t放映日期:");
		scanf("%d%d%d", &(rec.date.year),&(rec.date.month),&(rec.date.day));                  //输入放映日期
		printf("\t放映时间:");                    
		getchar();
		scanf("%d%d",&(rec.time.hour),&(rec.time.minute)/*,&(rec.time.second)*/);              //输入放映时间
		rec.seat_count=buf.seatsCount;
        printf("\n==================================================================\n");

		if (Schedule_Srv_Add(&rec))            //调用逻辑层函数实现添加功能
		{
			rec.seat_count += 1;
			newRecCount += 1;
			printf("\t添加成功！\n");
		} 
		else
			printf("\t添加失败!\n");
			
        printf("****************************************************************\n" );
		printf("[A]继续添加\n");
		printf("[R]返回\n");
		fflush(stdin);
		scanf("%c", &choice);
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}


//根据演出计划的ID值，调用业务逻辑层函数修改演出计划数据
int Schedule_UI_Modify(int id)
{
	int rtn = 0;
	int sid;
	char choice;
	schedule_t data;
	studio_t sdata;

	if(Schedule_Srv_FetchByID(id, &data)) //将ID对应的信息存入data中
	{
		while(1)
		{
			system("cls");
			printf("\n==================================================================\n");
			printf("\t修改 演出计划\n");
       		printf("\n==================================================================\n");
    		printf("%5s \t %5s\t%8s \t %10s \t %10s \t %10s \n", "ID", "剧目ID", "演出厅ID",
			 "放映日期","放映时间","座位数");
			printf("------------------------------------------------------------------\n");
			
			printf("%5d %5d %5d ", data.id, data.play_id,data.studio_id);
			
			printf("%4d-%02d-%02d ",data.date.year, data.date.month, data.date.day);
			printf("%2d:%02d:%02d ", data.time.hour, data.time.minute, data.time.second);
			printf("%5d\n", data.seat_count);

			printf("\n==================================================================\n");
			printf("[A]修改演出厅\n");
			printf("[B]修改放映时间");
			printf("[R]返回\n");
			printf("\n==================================================================\n");
			printf("\t请选择:");
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
					printf("\t请输入演播厅ID：");
					while(1)
					{
						if(scanf("%d", &sid) == 1)
						{
							while(!Studio_Srv_FetchByID(sid, &sdata))
							{
								printf("\n\tID为%d的演播厅不存在，请重新输入：", sid);
								printf("请重新输入演出厅ID:");
								setbuf(stdin, NULL);
								scanf("%d", &sid);
							}

							data.studio_id = sdata.id;  //演播厅ID
							data.seat_count = sdata.seatsCount;  //演播厅座位总数

							break;
						}
						else
						{
							printf("\t您的输入有误请重新输入：");
						}
					}

					break;
				}
				
				case 'b':
				case 'B':
				{
					printf("\t请输入放映日期：（年 月 日）");
					while(1)
					{
						if(scanf("%d %d %d", &(data.date.year), &(data.date.month), &(data.date.day)) == 3)
						{
							if(data.date.month > 12 || data.date.day > 31)
							{
								printf("\t您输入的年月日有误请重新输入：");
								continue;
							}
							if(DateCmp(data.date, DateNow()) < 0)
							{
								printf("\t您输入的日期在今天之前,请重新输入：");
								continue;
							}

							break;
						}
						else
						{
							printf("\t您的输入有误！请重新输入：");
						}
					}

					printf("\t请输入放映时间:(时 分) ");
					while(1)
					{
						if(scanf("%d %d", &(data.time.hour), &(data.time.minute)) == 2)
						{
							data.time.second = 0;
							if(data.time.hour > 24 || data.time.second > 60)
							{
								printf("\t您输入的时或分有误！请重新输入: ");
								continue;
							}

							break;
						}
						else
						{
							printf("\t您的输入有误！请重新输入：");
						}
					}
					break;
				}
			}//switch

			if(Schedule_Srv_Modify(&data))
			{
				printf("\t修改成功!\n");
				rtn = 1;
			}
			else
			{
				printf("\t修改失败！\n");
			}

			printf("\t按任意键继续修改！\n");

			getchar();
		}
	}
	else
	{
		printf("\t未找到ID未%d的演出计划!\n", id);
		printf("\t按任意键返回！\n");

		getchar();
		return 0;
	}

	return rtn;
}


//根据参数ID，调用业务逻辑层函数删除演出计划
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

	//载入数据
	paging.totalRecords = Schedule_Srv_FetchAll(head);

	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		printf("\n==================================================================\n");
		printf("\t演出计划 列表\n");
		printf("------------------------------------------------------------------\n");
		printf("%5s%5s%8s%10s%10s%10s\n", "计划", "剧目", "演出厅",
			"放映日期", "放映时间", "座位数");
		//显示数据
		Paging_ViewPage_ForEach(head, paging, schedule_node_t, p, i) {
			printf("%2d %2d %2d", p->data.id, p->data.play_id, p->data.studio_id);

			printf("%4d-%02d-%02d ", p->data.date.year, p->data.date.month, p->data.date.day);
			printf("%2d:%02d:%02d ", p->data.time.hour, p->data.time.minute, p->data.time.second);
			printf("%5d\n", p->data.seat_count);
		}


		printf("\n-------- 共 %2d 项 --------------------------- %2d/%2d 页 --------\n",
			paging.totalRecords, Pageing_CurPage(paging),
			Pageing_TotalPages(paging));
		printf("******************************************************************\n");
		printf("[P]上页\n");
		printf("[N]下页\n");
		printf("[R]返回\n");

		printf("\n==================================================================\n");
		printf("\t请选择:");

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
	//释放链表空间
	List_Destroy(head, schedule_node_t);
}

//按照ID号查询演出计划信息,输入待查找的剧目ID号，输出查找到的剧目信息。
//返回 0表示未找到，1表示找到了
int Schedule_UI_Query(int id) 
{

	schedule_t rec;
	/*int rtn = 0,seatcount;*/

	/*Load record*/
	if (!Schedule_Srv_FetchByID(id, &rec)) {
		printf("\t放映计划不存在\n按回车键返回\n");
		getchar();
		return 0;
	}

	printf("\n==================================================================\n");
	printf("\t更新放映计划\n");
	printf("------------------------------------------------------------------\n");
	printf("\t演出计划ID:%d\n", rec.id);
	printf("\t剧目ID:%d\n", rec.play_id);
	printf("\t演出厅ID:%d\n", rec.studio_id);
	printf("\t放映日期:%d年%2d月%2d日\n", rec.date.year, rec.date.month, rec.date.day);
	printf("\t放映时间:%2d:%2d\n", rec.time.hour, rec.time.minute/*,rec.time.second*/);
	printf("\t座位数:%d\n", rec.seat_count);
	return 0;
}
