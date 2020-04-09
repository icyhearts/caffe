#!/usr/bin/env python
'''

概况地讲，操作LMDB的流程是：

通过env = lmdb.open()打开环境
通过txn = env.begin()建立事务
通过txn.put(key, value)进行插入和修改
通过txn.delete(key)进行删除
通过txn.get(key)进行查询
通过txn.cursor()进行遍历
通过txn.commit()提交更改
'''

import lmdb

env = lmdb.open("students");
txn = env.begin(write=True);

txn.put(str(1).encode('ascii'), "Alice".encode('ascii'));
txn.put(str(2).encode('ascii'), "Bob".encode('ascii'));
txn.put(str(3).encode('ascii'), "Peter".encode('ascii'));

txn.delete(str(1).encode('ascii'));

txn.put(str(3).encode('ascii'), "Mark".encode('ascii'));


for key, value in txn.cursor():
    print (key, value);

txn.commit();
