cmake_minimum_required(VERSION 3.5)

project(evt-handler LANGUAGES C)

add_executable(evt-handler main.c evt-common.h evt-common.c evt-functions.c evt-functions.h)

