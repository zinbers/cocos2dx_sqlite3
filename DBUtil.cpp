#include "./sqlite-amalgamation-3080803/sqlite3.h"
#include "DBUtil.h"
#include "cocos2d.h"
#include "TestVO.h"
using namespace std;
USING_NS_CC;

sqlite3 *pDB = nullptr;//数据库指针
char * errMsg = nullptr;//错误信息
std::string sqlstr;//SQL指令
int result;//sqlite3_exec返回值


//创建数据库
void DBUtil::initDB(const char *db)
{
	//打开一个数据库，如果该数据库不存在，则创建一个数据库文件
	result = sqlite3_open(db, &pDB);
	if (result != SQLITE_OK)
		("打开数据库失败，错误码:%d ，错误原因:%s\n", result, errMsg);
}


//tableIsExist的回调函数
int isExisted(void * para, int n_column, char ** column_value, char ** column_name)
{
	bool *isExisted_ = (bool*)para;
	*isExisted_ = (**column_value) != '0';
	return 0;
}



//判断表格是否存在
bool DBUtil::tableIsExist(string name)
{
	if (pDB != NULL)
	{
		//判断表是否存在
		bool tableIsExisted;
		sqlstr = "select count(type) from sqlite_master where type='table' and name ='" + name + "'";
		result = sqlite3_exec(pDB, sqlstr.c_str(), isExisted, &tableIsExisted, &errMsg);
		return tableIsExisted;
	}
	return false;
}



//在数据库中判断名为name的表示否存在，如果不存在则创建这张表
//@示例语句string sqls = "create table user(id integer,username text,password text)";
void DBUtil::createTable(string sql, string name)
{
	if (!tableIsExist(name))
	{
		//创建表，设置ID为主键，且自动增加
		result = sqlite3_exec(pDB, sql.c_str(), NULL, NULL, &errMsg);
		if (result != SQLITE_OK)
			CCLog("创建表失败，错误码:%d ，错误原因:%s\n", result, errMsg);
	}

}


//删除表格
//@示例语句sqlstr="drop table name";
void DBUtil::deleteTable(string sql, string name)
{
	if (tableIsExist(name))
	{
		result = sqlite3_exec(pDB, sql.c_str(), NULL, NULL, &errMsg);
		if (result != SQLITE_OK)
			CCLog("创建表失败，错误码:%d ，错误原因:%s\n", result, errMsg);
	}
}


//插入数据
//@示例语句sqlstr=" insert into MyTable_1( name ) values ( '擎天柱' ) ";
void DBUtil::insertData(string sql){
	result = sqlite3_exec(pDB, sql.c_str(), NULL, NULL, &errMsg);
	if (result != SQLITE_OK)
		CCLog("插入记录失败，错误码:%d ，错误原因:%s\n", result, errMsg);
}


//删除数据
//@示例语句sqlstr="delete from MyTable_1 where ID = 2";
void DBUtil::deleteData(string sql)
{
	result = sqlite3_exec(pDB, sql.c_str(), NULL, NULL, &errMsg);
	if (result != SQLITE_OK)
		CCLog("插入记录失败，错误码:%d ，错误原因:%s\n", result, errMsg);
}


//修改数据
//@示例语句        sqlstr="update MyTable_1 set name='威震天' where ID = 3";
void DBUtil::updateData(string sql)
{
	result = sqlite3_exec(pDB, sql.c_str(), NULL, NULL, &errMsg);
	if (result != SQLITE_OK)
		CCLog("插入记录失败，错误码:%d ，错误原因:%s\n", result, errMsg);
}


//getDataCount的回调函数
int loadRecordCount(void * para, int n_column, char ** column_value, char ** column_name)
{
	int *count = (int*)para;
	*count = n_column;
	return 0;
}
//获取记录的条数
//@示例语句string sqlsssss = "select count(*) from user";
//@示例语句  取得表格字段的语句string sqlsssss = "select * from user";
int DBUtil::getDataCount(string sql)
{
	int count = 0;
	sqlite3_exec(pDB, sql.c_str(), loadRecordCount, &count, &errMsg);
	return count;
}


//getDataInfo的回调函数
int loadRecord(void * para, int n_column, char ** column_value, char ** column_name)
{
	CCLOG("n_column:%d", n_column);

    TestVO* testVO = (TestVO*)para;

	testVO->mId = atoi(column_value[0]);
	testVO->level = atoi(column_value[1]);
	testVO->lastscore = atoi(column_value[2]);
	testVO->bestscore = atoi(column_value[3]);
	testVO->star = atoi(column_value[4]);


	/* 可能有5个字段 */
	// id level lastscore bestscore star
	//    CCLOG("c[0]:%s,c[1]:%s,c[2]:%s,c[3]:%s,c[4]:%s",column_name[0],column_name[1],column_name[2],column_name[3],column_name[4]);
	//    
	CCLog("id=%s,level=%s,lastscore=%s,bestscore=%s,star=%s", column_value[0], column_value[1], column_value[2], column_value[3], column_value[4]);
	return 0;
}
//获取一条记录的信息 其中的pSend是一个实体类我们以后可以自定义一个继承了CCObject的类来代替他保存数据库中取出来的数据
/*
*  这里最好扩展下，让  pSend  是一个vector
*/
void DBUtil::getDataInfo(string sql, void *pSend)
{
	sqlite3_exec(pDB, sql.c_str(), loadRecord, pSend, &errMsg);
}


//关闭数据库
void DBUtil::closeDB()
{
	sqlite3_close(pDB);
}
