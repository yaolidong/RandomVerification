

# 声明要求的cmake最低版本
cmake_minimum_required(VERSION 3.17)

# 声明一个cmake工程
project(RandomValidation)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_BUILD_TYPE Debug)



include_directories(.)

# 添加一个可执行程序
# 语法为： add_executable( 程序名  源代码文件
add_executable(RandomValidation
        Blockchain.cpp
        Block.cpp
        Block.h
        Blockchain.h
        Cache.h
        Cache.cpp
        Sealer.h
        Sealer.cpp
        sha256.cpp
        sha256.h
        ViewState.h
        ViewState.cpp
        Message.h
        Message.cpp
        Network.h
        Network.cpp
        Node.h
        Node.cpp
        RandomValidation.h
        RandomValidation.cpp
        Committee.h
        Committee.cpp PBFT.h PBFT.cpp)
