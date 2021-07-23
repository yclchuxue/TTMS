#ifndef PLAY_H
#define PLAY_H

#include "../View/Play_UI.h"

//标识符：TTMS_SCU_Play_Srv_Add
//函数功能：调用Play_Perst_Insert添加剧目 
int Play_Srv_Add(play_t *data);

//标识符：TTMS_SCU_Play_Srv_FetchAll
//函数功能：调用Play_Perst_SelectAll获取全部剧目信息 
int Play_Srv_FetchAll(play_list_t list);

//标识符：TTMS_SCU_Play_Srv_FetchByID
//函数功能：调用 Play_Perst_SelectByID获取剧目信息
int Play_Srv_FetchByID(int id, play_t *buf);

//标识符：TTMS_SCU_Play_Srv_Mod
//函数功能：调用 Play_Perst_Modify修改剧目信息
int Play_Srv_Modify(const play_t *data);

//标识符：TTMS_SCU_Play_Srv_DelByID
//函数功能：调用Play_Perst_RemByID删除剧目
int Play_Srv_DelectByID(int id); 

int Play_Srv_FetchByName(play_list_t list,char condt[]);

#endif PLAY_H
