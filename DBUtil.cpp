#include "sqlite3.h"
#include "DBUtil.h"
#include "cocos2d.h"
#include <map>
#include <vector>
#include <string>
using namespace std;
USING_NS_CC;

sqlite3 *pDB = nullptr;//���ݿ�ָ��
char * errMsg = nullptr;//������Ϣ
std::string sqlstr;//SQLָ��
int result;//sqlite3_exec����ֵ


//�������ݿ�
void DBUtil::initDB(const char *db)
{
	//��һ�����ݿ⣬��������ݿⲻ���ڣ��򴴽�һ�����ݿ��ļ�
	result = sqlite3_open(db, &pDB);
	if (result != SQLITE_OK)
		CCLog("�����ݿ�ʧ�ܣ�������:%d ������ԭ��:%s\n", result, errMsg);
}


//tableIsExist�Ļص�����
int isExisted(void * para, int n_column, char ** column_value, char ** column_name)
{
	bool *isExisted_ = (bool*)para;
	*isExisted_ = (**column_value) != '0';
	return 0;
}



//�жϱ���Ƿ����
bool DBUtil::tableIsExist(string name)
{
	if (pDB != NULL)
	{
		//�жϱ��Ƿ����
		bool tableIsExisted;
		sqlstr = "select count(type) from sqlite_master where type='table' and name ='" + name + "'";
		result = sqlite3_exec(pDB, sqlstr.c_str(), isExisted, &tableIsExisted, &errMsg);
		return tableIsExisted;
	}
	return false;
}



//�����ݿ����ж���Ϊname�ı�ʾ����ڣ�����������򴴽����ű�
//@ʾ�����string sqls = "create table user(id integer,username text,password text)";
void DBUtil::createTable(string sql, string name)
{
	if (!tableIsExist(name))
	{
		//����������IDΪ���������Զ�����
		result = sqlite3_exec(pDB, sql.c_str(), NULL, NULL, &errMsg);
		if (result != SQLITE_OK)
			CCLog("������ʧ�ܣ�������:%d ������ԭ��:%s\n", result, errMsg);
	}

}


//ɾ�����
//@ʾ�����sqlstr="drop table name";
void DBUtil::deleteTable(string sql, string name)
{
	if (tableIsExist(name))
	{
		result = sqlite3_exec(pDB, sql.c_str(), NULL, NULL, &errMsg);
		if (result != SQLITE_OK)
			CCLog("������ʧ�ܣ�������:%d ������ԭ��:%s\n", result, errMsg);
	}
}


//��������
//@ʾ�����sqlstr=" insert into MyTable_1( name ) values ( '������' ) ";
void DBUtil::insertData(string sql){
	result = sqlite3_exec(pDB, sql.c_str(), NULL, NULL, &errMsg);
	if (result != SQLITE_OK)
		CCLog("�����¼ʧ�ܣ�������:%d ������ԭ��:%s\n", result, errMsg);
}


//ɾ������
//@ʾ�����sqlstr="delete from MyTable_1 where ID = 2";
void DBUtil::deleteData(string sql)
{
	result = sqlite3_exec(pDB, sql.c_str(), NULL, NULL, &errMsg);
	if (result != SQLITE_OK)
		CCLog("�����¼ʧ�ܣ�������:%d ������ԭ��:%s\n", result, errMsg);
}


//�޸�����
//@ʾ�����        sqlstr="update MyTable_1 set name='������' where ID = 3";
void DBUtil::updateData(string sql)
{
	result = sqlite3_exec(pDB, sql.c_str(), NULL, NULL, &errMsg);
	if (result != SQLITE_OK)
		CCLog("�����¼ʧ�ܣ�������:%d ������ԭ��:%s\n", result, errMsg);
}


//getDataCount�Ļص�����
int loadRecordCount(void * para, int n_column, char ** column_value, char ** column_name)
{
	int *count = (int*)para;
	*count = n_column;
	return 0;
}
//��ȡ��¼������
//@ʾ�����string sqlsssss = "select count(*) from user";
//@ʾ�����  ȡ�ñ���ֶε����string sqlsssss = "select * from user";
int DBUtil::getDataCount(string sql)
{
	int count = 0;
	sqlite3_exec(pDB, sql.c_str(), loadRecordCount, &count, &errMsg);
	return count;
}


//getDataInfo�Ļص�����
int loadRecord(void * para, int n_column, char ** column_value, char ** column_name)
{
	 CCLOG("n_column:%d", n_column);

    mapStrVecstr* pData = (mapStrVecstr*)para;
    
    for (int i = 0 ; i < n_column; i++) {
        (*pData)[column_name[i]].push_back(column_value[i]);
    }

	return 0;
}
//��ȡһ����¼����Ϣ ���е�pSend��һ��ʵ���������Ժ�����Զ���һ���̳���CCObject�������������������ݿ���ȡ����������
/*
*  ���������չ�£���  pSend  ��һ��vector
*/
void DBUtil::getDataInfo(string sql, void *pSend)
{
	sqlite3_exec(pDB, sql.c_str(), loadRecord, pSend, &errMsg);
}


//�ر����ݿ�
void DBUtil::closeDB()
{
	sqlite3_close(pDB);
}
