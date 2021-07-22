#ifndef PLAY_PER_H
#define PLAY_PER_H

#include "../View/Play_UI.h"

//标识符：TTMS_SCU_Play_Perst_SelAll
//函数功能：从文件Play.dat中载入所有剧目信息，加载到链表list上
int Play_Perst_SelectAll(play_list_t list);

//标识符：TTMS_SCU_Play_Perst_Insert
//函数功能：向剧目数据文件Play.dat的末尾添加一条新的剧目信息
int Play_Perst_Insert(play_t *data);

//标识符：TTMS_SCU_Play_Perst_Update
//函数功能：更新文件play.dat中的一条剧目信息 
int Play_Perst_Update(const play_t *data);

//标识符：TTMS_SCU_Play_Perst_RemByID
//函数功能：删除Play.dat中指定ID的剧目信息
int Play_Perst_RemByID(int id);

//标识符；TTMS_SCU_Play_Perst_SelByID
//函数功能：在文件Play.dat中载入指定id的信息加载到buf中
int Play_Perst_SelectByID(int id, play_t *data); 

int Play_Perst_SelectByName(play_list_t list,char condt[]);

#endif PLAY_PER_H
