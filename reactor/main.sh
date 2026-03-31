#!/bin/bash

ls
dir="$(pwd)/build"

if [ ! -d $dir ]; then
    mkdir $dir && cd build
else
    cd build
fi

ls
rm -rf *
cmake ..
make
echo "$(pwd) 构建完成！"

if [ -x "$(pwd)/server" ]
then
    echo "启动服务器..."
    gnome-terminal --working-directory=$dir --command=./server
    echo `date`": 服务器启动成功！" >> ./../server.log
    
else 
    echo server not found
fi


if [ -x "$(pwd)/client" ]
then
    # echo "启动客户端..."
    gnome-terminal --working-directory=$dir --command=./client
    echo `date`": 客户端启动成功！" >> ./../client.log
    
else 
    echo client not found
fi
