# cocos2dx_sqlite3
sqlite3 DBUtil
```cpp
// Test Database
    string fullDBPath = FileUtils::getInstance()->getWritablePath() + "save.db";
    
    CCLOG("fullPath : %s",fullDBPath.c_str());
    
    //打开数据库
    DBUtil::initDB(fullDBPath.c_str());
    
    string createTableSql = "create table zuma (id integer primary key autoincrement,level integer,lastscore integer,bestscore integer,star integer);";
    DBUtil::createTable(createTableSql.c_str(),"zuma");
    
    //向表格中插入数据
    string sqlss = "insert into zuma (level,lastscore,bestscore,star) values (200,300,500,1)";
    
    /* 插入一条数据 */
    DBUtil::insertData(sqlss);
    
    // 更新
    string updateString = "update zuma set star = 5;";
    
    DBUtil::updateData(updateString);
    
    /* 查询数据 */
    string selectStr = "select * from zuma";
    
    mapStrVecstr resData;
    
    DBUtil::getDataInfo(selectStr,&resData);
    for (auto map_it : resData) {
        CCLOG("key:%s",map_it.first.c_str());
        for (auto vec_it : map_it.second) {
            CCLOG("values:%s",vec_it.c_str());
        }
    }
    
    resData.clear();
    
    /* 不能忘记关闭数据库 */
    DBUtil::closeDB();
```
