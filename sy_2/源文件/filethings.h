#ifndef FILETHINGS_H
#define FILETHINGS_H

#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <io.h>  
#include <direct.h>  
#include <errno.h>

//�ж��Ƿ���".."Ŀ¼��"."Ŀ¼  
inline bool is_special_dir(const char *path) ;

//�ж��ļ�������Ŀ¼�����ļ�  
inline bool is_dir(int attrib) ;

//��ʾɾ��ʧ��ԭ��  
inline void show_error(const char *file_name ) ;

inline void get_file_path(const char *path, const char *file_name, char *file_path) ;

//�ݹ�����Ŀ¼���ļ���ɾ��  
inline void delete_file(char *path, char *removeshot);




#endif

