#include "Play_UI.h"
#include "../service/Play.h"
#include "../Common/List.h"
#include "Schedule_UI.h"
#include <stdio.h>

/*
标识符：TTMS_SCU_Play_UI_MgtEnt
函数功能：显示管理剧目界面
参数说明：无
返回值：无
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
			printf("暂无记录\n");
		else
		{
			printf("=========================================================================================================\n");
			printf("========================================剧目信息=========================================================\n");
			printf("ID  名称                类型     出品地区          等级      时长      开映时间       结映时间       票价\n"); 
			printf("---------------------------------------------------------------------------------------------------------\n");
			Paging_ViewPage_ForEach(head, paging, play_node_t, pos, i)
			{
				printf("%d  %-20s", pos->data.id, pos->data.name);
				switch(pos->data.type)
				{
					case 1:
						printf("电影     ");
						break;
					case 2:
						printf("歌剧     ");
						break;
					case 3:
						printf("音乐会   ");
						break;
				}
				printf("%-18s", pos->data.area);
				switch(pos->data.rating)
				{
					case 1:
						printf("儿童      ");
						break;
					case 2:
						printf("青少年    ");
						break;
					case 3:
						printf("成人      ");
						break;
				}
				printf("%-10d",pos->data.duration);
				printf("%4d.%2d.%-7d",pos->data.start_date.year,pos->data.start_date.month,pos->data.start_date.day);
				printf("%4d.%2d.%-7d",pos->data.end_date.year,pos->data.end_date.month,pos->data.end_date.day);
				printf("%d\n",pos->data.price);
			}
			printf("========================共 %d 项 ============================== 第 %d/%d 页 ================================\n",paging.totalRecords, Pageing_CurPage(paging),Pageing_TotalPages(paging));
		}
		
		printf("[p/P]显示上一页\n");
		printf("[n/N]显示下一页\n");
		printf("[a/A]添加新剧目\n");
		printf("[u/U]修改剧目\n");
		printf("[d/D]删除剧目\n");
		printf("[s/S]安排演出\n");
		printf("[r/R]退出\n");
		printf("---------------------------------------\n");
		printf("输入相应字母："); 
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
			printf("输入剧目id:");
			scanf("%d", &id);
			if (Play_UI_Modify(id))
				{	
					List_Init(head, play_node_t);
					paging.totalRecords = Play_Srv_FetchAll(head);
					List_Paging(head, paging, play_node_t);
				}
		}else if((choice == 'd') || (choice == 'D'))
		{
			printf("输入剧目id:");
			scanf("%d", &id);
			if (Play_UI_Delect(id))
			{
				printf("删除成功\n");
				List_Init(head, play_node_t);
				paging.totalRecords = Play_Srv_FetchAll(head);
				List_Paging(head, paging, play_node_t);
			}
			else
				printf("删除失败\n");
		}else if((choice == 'p') || (choice == 'P'))
		{
			if(!Pageing_IsFirstPage(paging))
			{
				paging.totalRecords = Play_Srv_FetchAll(head);
				Paging_Locate_OffsetPage(head, paging, -1, play_node_t);
			}
			else
				printf("无法跳转\n");
		}else if((choice == 'n') || (choice == 'N'))
		{
			if(!Pageing_IsLastPage(paging))
			{
				paging.totalRecords = Play_Srv_FetchAll(head);
				Paging_Locate_OffsetPage(head, paging, 1, play_node_t);
			}
			else
				printf("无法跳转\n");
		}else if((choice == 's') || (choice == 'S'))
		{
			printf("输入剧目id:");
			scanf("%d", &id);
			if(Play_Srv_FetchByID(id, &buf) == 0)
				printf("该剧目id不存在\n");
			else
				Schedule_UI_MgtEntry(id);
		}else if((choice == 'r') || (choice == 'R'))
			break;
	}while(1);
	List_Destroy(head, play_node_t);
	return ;
}

/*
标识符：TTMS_SCU_Play_UI_Add
函数功能：显示添加新剧目界面
参数说明：无
返回值：返回值>0表示添加成功， 为0表示添加失败
*/
int Play_UI_Add(void)
{
    char choice;
    play_t data;
    int newCount = 0, i;
     
    while(1)
    {
    	printf("===================================\n");
        printf("输入剧目名称：");
        scanf("%s", data.name);
        printf("--------------------\n");
        while(1)
        {
	        printf("1.电影\n");
	        printf("2.歌剧\n");
	        printf("3.音乐会\n");
	        printf("--------------------\n");
	        printf("选择剧目类型:");
	        scanf("%d", &data.type);
	        if(data.type <= 3 && data.type >= 1)
		        break;
	   }
        printf("输入剧目出品地区:");
        scanf("%s", data.area);
        printf("--------------------\n");
        while(1)
    	{
	        printf("--------------------\n");
	        printf("1.适合儿童\n");
	        printf("2.适合青少年\n");
	        printf("3.适合成人\n");
	        printf("--------------------\n");
	        printf("选择新的剧目等级：");
	        scanf("%d", &data.rating); 
	        if(data.rating <= 3 && data.rating >= 1)
	        	break;
        }
        scanf("%d", &data.rating);
		printf("输入剧目时长(以分钟为单位,输入数字):");
		scanf("%d",&data.duration); 
		printf("输入开始放映日期(如2020 2 22):");
		scanf("%d %d %d",&data.start_date.year,&data.start_date.month,&data.start_date.day);
		printf("输入放映结束日期(如2020 2 22):");
		scanf("%d %d %d",&data.end_date.year,&data.end_date.month,&data.end_date.day);
		printf("输入票价:");
		scanf("%d",&data.price);
         
        i = Play_Srv_Add(&data); 
        if(i == 1)
        {
            newCount++;
            printf("添加成功\n");
        }
        else
            printf("添加失败\n");
         
        while(1)
        {
            printf("----------------------\n");
            printf("[a/A]继续添加\n");
            printf("[r/R]返回\n");
            printf("----------------------\n");
            printf("输入相应字母:");
            fflush(stdin);
            choice = getchar();
             
            if((choice == 'a') || (choice == 'A'))
                break;
            else if((choice == 'r') || (choice == 'R'))
                return newCount;
            else
                printf("请输入正确内容!\n");
        }
    }
}
 
/*
标识符：TTMS_SCU_Play_UI_Mod
函数功能：修改剧目
参数说明：id为待修改剧目id
返回值；返回1表示成功，否则失败
*/
int Play_UI_Modify(int id) 
{
    play_t buf;
    int i;
    char choice;
     
    i = Play_Srv_FetchByID(id, &buf);
    if(i != 1)
    {
        printf("获取信息失败\n");
        return -1;
	}
	
    system("cls"); 
    printf("========================================剧目信息====================================================\n");
	printf("id:%d\n名称:%s\n", buf.id, buf.name);
	switch(buf.type){
		case 1:printf("类型:电影\n","电影");break;
		case 2:printf("类型:歌剧\n","歌剧");break;
		case 3:printf("类型:音乐会\n","音乐会");break;
	}
	printf("出品地区:%s\n", buf.area);
	switch(buf.rating){
	case 1:printf("等级:儿童\n");break;
	case 2:printf("等级:青少年\n");break;
	case 3:printf("等级:成人\n");break;
	}
	printf("时长：%d\n",buf.duration);
	printf("开始放映日期:%d.%d.%d\n",buf.start_date.year,buf.start_date.month,buf.start_date.day);
	printf("结束放映日期:%d.%d.%d\n",buf.end_date.year,buf.end_date.month,buf.end_date.day);
	printf("票价:%d\n",buf.price);
	
     
    printf("-----------------------------------------------------------------------\n");
    printf("剧目名称(n/N)\t剧目类型(t/T)\t剧目出品地区(a/A)\t剧目等级(r/R)\n");
    printf("剧目时长(d/D)\t开始放映日期(s/S)\t结束放映日期(e/E)\t建议票价(p/P)\n"); 
    printf("-----------------------------------------------------------------------\n");
    printf("选择需要修改的项:");
    fflush(stdin);
    choice = getchar();
     
    if((choice == 'n') || (choice == 'N'))
    {
        printf("输入新的剧目名称:");
        memset(buf.name, 0, sizeof(buf.name));
        scanf("%s", buf.name);
    }else if((choice == 't') || (choice == 'T'))
    {
        while(1)
        {
	        printf("1.电影\n");
	        printf("2.歌剧\n");
	        printf("3.音乐会\n");
	        printf("--------------------\n");
	        printf("选择剧目类型:");
	        scanf("%d", &buf.type);
	        if(buf.type <= 3 && buf.type >= 1)
		        break;
	   }
    }else if((choice == 'a') || (choice == 'A'))
    {
        printf("输入新的剧目出品地区:");
        memset(buf.area, 0, sizeof(buf.area));
        scanf("%s", buf.area);
    }else if((choice == 'r') || (choice == 'R'))
    {
    	while(1)
    	{
	        printf("--------------------\n");
	        printf("1.适合儿童\n");
	        printf("2.适合青少年\n");
	        printf("3.适合成人\n");
	        printf("--------------------\n");
	        printf("选择新的剧目等级：");
	        scanf("%d", &buf.rating); 
	        if(buf.rating <= 3 && buf.rating >= 1)
	        	break;
        }
    }else if((choice == 'd') || (choice == 'D'))
    {
        printf("输入新的时长：");
        scanf("%d", &buf.duration); 
    }else if((choice == 's') || (choice == 'S'))
    {
		printf("输入新的开始放映日期(如2021 10 22):");
		scanf("%d %d %d",&buf.start_date.year,&buf.start_date.month,&buf.start_date.day); 
    }else if((choice == 'e') || (choice == 'E'))
    {
		printf("输入新的放映结束日期(如2021 10 22):");
		scanf("%d %d %d",&buf.end_date.year,&buf.end_date.month,&buf.end_date.day);
    }else if((choice == 'p') || (choice == 'P'))
    {
        printf("输入新的票价：");
        scanf("%d", &buf.price); 
	} 
    return Play_Srv_Modify(&buf);
}
 
/*
标识符：TTMS_SCU_Play_UI_Del
函数功能：调用 Play_Srv_DelectByID删除剧目
参数说明：id为待删除剧目id
返回值：1表示成功，否则失败
*/
int Play_UI_Delect(int id)
{
    return Play_Srv_DelectByID(id);
}
 
/*
标识符：TTMS_SCU_Play_UI_Qry
函数功能：调用 Play_Srv_FetchByID查询
参数说明：id为待查询剧目id
返回值：1表示成功，否则失败 
*/
int Play_UI_Query(int id)
{
    play_t buf;
    int i = Play_Srv_FetchByID(id, &buf);
    if(i != 1)
        return -1;
    
    printf("============================================剧目信息================================================\n");  
    printf("id:%d\n名称:%d\n", buf.id, buf.name);
	switch(buf.type){
		case 1:printf("类型:%s\n","电影");break;
		case 2:printf("类型:%s\n","歌剧");break;
		case 3:printf("类型:%s\n","音乐会");break;
	}
	printf("出品地区:%s\n", buf.area);
	switch(buf.rating){
	case 1:printf("等级:%s\n","儿童");break;
	case 2:printf("等级:%s\n","青少年");break;
	case 3:printf("等级:%s\n","成人");break;
	}
	printf("时长：%d\n",buf.duration);
	printf("开映日期:%d.%d.%d\n",buf.start_date.year,buf.start_date.month,buf.start_date.day);
	printf("结映日期:%d.%d.%d\n",buf.end_date.year,buf.end_date.month,buf.end_date.day);
	printf("票价:%d\n",buf.price);
    printf("====================================================================================================\n"); 
    return 1;
}
