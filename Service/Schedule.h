#ifndef SCHEDULE_H_
#define SCHEDULE_H_

#include "../Common/Common.h"



typedef struct {//演出计划类型定义
	long int id;            //演出计划ID号
	int play_id;       //演出的剧目ID号
	int studio_id;     //演出的放映厅ID号
	user_date_t date;  //演出日期
	user_time_t time;  //演出时间
	int seat_count;    //座位数量
}schedule_t;

//定义演出计划信息的双向链表结构
typedef struct schedule_node {
	schedule_t data;
	struct schedule_node *next, *prev;
} schedule_node_t, *schedule_list_t;


//取出所有演出计划信息，串成一条链表，输入 list演出计划信息链表的头指针
//无输出，返回查找到的记录数目
int Schedule_Srv_FetchAll(schedule_list_t list);

//根据剧目ID，查找剧目相关联的所有演出计划
//int Schedule_Srv_FetchByPlay(schedule_list_t list, int play_id);



//添加新演出计划
//将参数date，作为实参调用持久化层储存新演出计划函数，并将持久化层函数返回值传递给界面层函数
int Schedule_Srv_Add(schedule_t * date);
//修改演出计划
//用参数date中的演出计划数据为实参，调用持久化层函数来修改文件中旧的演出计划信息
int Schedule_Srv_Modify(const schedule_t * date);

//根据ID删除演出计划
//更具ID，调用调用持久化层函数删除演出计划
int Schedule_Srv_DeleteByID(int id);

//按照剧目ID号，将相关演出计划信息取出，串成一条链表
//输入 list为查找到的演出计划信息链表的头指针，play_id为剧目的ID号，无输出
//返回查找到的记录数目
int Schedule_Srv_FetchByPlay(schedule_list_t list, int play_id);

int Schedule_Srv_FetchByID(int id,schedule_t *buf);
 
 int Schedule_Srv_StatRevByPlay(int play_id, int *soldCount);
 

#endif //SCHEDULE_H_
