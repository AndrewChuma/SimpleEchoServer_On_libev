cmake_minimum_required(VERSION 3.5)

project(EchoServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(EchoServer main.cpp 
                          C_BaseAbstractSocket.cpp 
                          C_Log.cpp 
                          C_Server.cpp 
                          I_Connection.cpp 
                          C_TcpSocketServer.cpp 
                          C_BaseAbstractSocket.h 
                          C_Log.h 
                          C_Server.h 
                          C_TcpSocketServer.h
                          I_Connection.h
               )

target_link_libraries(EchoServer libev.a)
