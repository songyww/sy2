#ifndef FILETHINGS_H
#define FILETHINGS_H

#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <io.h>  
#include <direct.h>  
#include <errno.h>

//判断是否是".."目录和"."目录  
inline bool is_special_dir(const char *path) ;

//判断文件属性是目录还是文件  
inline bool is_dir(int attrib) ;

//显示删除失败原因  
inline void show_error(const char *file_name ) ;

inline void get_file_path(const char *path, const char *file_name, char *file_path) ;

//递归搜索目录中文件并删除  
inline void delete_file(char *path, char *removeshot);




#endif

