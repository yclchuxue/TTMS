#include "Schedule.h"
#include "../Common/List.h"
#include "../Persistence/Schedule_Persist.h"
//#include "Ticket.h"
#include <stdio.h>

 int Schedule_Srv_FetchByID(int id,schedule_t *buf)
{
	return Schedule_Perst_SelectByID(id,buf);
}

//根据剧目ID，查找剧目相关联的所有演出计划
int Schedule_Srv_FetchByPlay(schedule_list_t list, int play_id)
{
	int ret;
	ret = Schedule_Perst_SelectByPlay(list, play_id);

	return ret;
}

//取出所有演出计划信息，串成一条链表，输入 list演出计划信息链表的头指针
//无输出，返回查找到的记录数目
inline int Schedule_Srv_FetchAll(schedule_list_t list) {
	return Schedule_Perst_SelectAll(list);
}

//按照剧目ID号，将相关演出计划信息取出，串成一条链表
//输入 list为查找到的演出计划信息链表的头指针，play_id为剧目的ID号，无输出
//返回查找到的记录数目
//int Schedule_Srv_FetchByPlay(schedule_list_t list, int play_id) {
//	return Schedule_Perst_SelectByPlay(list, play_id);
//}

//添加新演出计划
//将参数date，作为实参调用持久化层储存新演出计划函数，并将持久化层函数返回值传递给界面层函数
int Schedule_Srv_Add(schedule_t * date)
{
	int ret;
	ret = Schedule_Perst_Insert(date);   //向演出计划文件Schedule.dat的末尾添加一条新演出计划

	return ret;
}

//修改演出计划
//用参数date中的演出计划数据为实参，调用持久化层函数来修改文件中旧的演出计划信息
int Schedule_Srv_Modify(const schedule_t * date)
{
	int ret = 0;
	ret = Schedule_Perst_Update(date);

	return ret;
}


//根据剧目ID获取票房 
int Schedule_Srv_StatRevByPlay(int play_id, int *soldCount)
{
	int value = 0;//存储票房
	int sold = 0; //存储有效售票数量
	schedule_list_t list;
	schedule_node_t *p;
	*soldCount = 0;
	List_Init(list,schedule_node_t);
	Schedule_Perst_SelectByPlay(list,play_id);//构建演出计划链表list
	List_ForEach(list,p)
	{
		value += Ticket_Srv_StatRevSchID(p->data.id, &sold);     //
		*soldCount = *soldCount+sold;
	}
	List_Destroy(list,schedule_node_t);//销毁链表
	return value;
}


//根据ID删除演出计划
//更具ID，调用调用持久化层函数删除演出计划
int Schedule_Srv_DeleteByID(int id)
{
	int ret = 0;
	ret = Schedule_Perst_RemByID(id);

	return ret;
}
