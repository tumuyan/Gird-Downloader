#ifndef T_STR_H_INCLUDED
#define T_STR_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

char *keepstr(char *str)
{
    char *out;
    out=(char *)malloc(strlen(str)+1);
    strcpy(out,str);
    return out;
}

char *keep2str(char *s1,char *s2)
{
    char *out;
    out=(char *)malloc(strlen(s1)+strlen(s2)+1);
    strcpy(out,s1);
    strcpy(out+strlen(s1),s2);
    return out;
}

char *keep3str(char *s1,char *s2,char *s3)
{
    char *out;
    out=(char *)malloc(strlen(s1)+strlen(s2)+strlen(s3)+1);
    strcpy(out,s1);
    strcpy(out+strlen(s1),s2);
    strcpy(out+strlen(s1)+strlen(s2),s3);
    return out;
}


char *keepstrs(int r,char *s[])
{
    int length=0,p=0;
    int *len=new int[r];
    for(int i=0; i<r; i++)
    {
        len[i]=strlen(s[i]);
        length+=len[i];
    }
    char *str;
    str=  (char *)malloc(length+1);

    str[length]='\0';

    for(int i=0; i<r; i++)
    {
        for(int j=0; j<len[r]; j++)
        {
            str[p]=s[i][j];
            p++;
        }
    }

    return str;
}

char *new_file_path (char *old_file_path,char *new_file_name)
{
    char *old_file_name;
    old_file_name=strrchr(old_file_path,'\\');
    if(old_file_name!=NULL)
    {
        int folder_length=strlen(old_file_path)-strlen(old_file_name);
        char *path=new char[folder_length+1];
        int i;
        for( i=0; i<folder_length; i++)
        {
            path[i]=old_file_path[i];
        }
        //    printf("get name: %s\n",sample_name);
        path[i]='\0';
        return keep3str(path,(char*)"\\",new_file_name);
    }
    else
    {
        return keepstr(new_file_name);
    }
}

char *get_folder_from_path(char *path)
{
    char *pos_slash;

    {
        char* cache=new char[strlen(path)+1];
        strcpy(cache,path);

        pos_slash=strrchr(cache,'\\');

        if(pos_slash!=NULL)
        {
            cache[pos_slash-cache+1]='\0';
            return keepstr(cache);
        }
        else
        {
            printf("Error: not find slash from path, %s\n",cache);
        }
    }
    return path;
}

char *get_name_from_url(char *url,char *old_path)
{

    char *pos_dot,*pos_quer,*pos_slash;
    pos_dot=strrchr(url,'.');

    if(url!=NULL)
    {
        char* cache=new char[strlen(url)+1];
        strcpy(cache,url);

        if(strlen(cache)<1)
        {
            return NULL;
        }

        pos_quer=strchr(pos_dot,'?');
        if(pos_quer!=NULL)
        {
            cache[pos_quer-url]='\0';
        }
        pos_slash=strrchr(cache,'/');

        if(pos_slash!=NULL)
        {
            // pos_slash+1 就是取回的文件名称
            return keepstr(pos_slash+1);
        }
        else
        {
            printf("Error: not find slash from url, %s, %s\n",cache,url);
        }
    }
    else
    {
        printf("Error: not find dot from url, %s\n",url);
    }
    return NULL;
}

char *url_to_local_path(char *url,char *old_path)
{

    char *pos_dot,*pos_quer,*pos_slash;
    pos_dot=strrchr(url,'.');

    if(url!=NULL)
    {
        char *cache=new char[strlen(url)+1];
        strcpy(cache,url);

        pos_quer=strchr(pos_dot,'?');
        if(pos_quer!=NULL)
        {
            cache[pos_quer-url]='\0';
        }
        pos_slash=strrchr(cache,'/');

        if(pos_slash!=NULL)
        {
            // pos_slash+1 就是取回的文件名称
            url=cache;

            //                    printf("url_to_local_path:  oldpath:%s,name:%s\n",old_path,pos_slash+1);
            return new_file_path(old_path,pos_slash+1);
        }
        else
        {
            printf("Error: not find slash from url, %s, %s\n",cache,url);
        }
    }
    else
    {
        printf("Error: not find dot from url, %s\n",url);
    }
    return NULL;
}


// 由于路径包含了文件名，返回的是指针，没有复制内容
char *get_file_name(char *path)
{

    char *file_name;

    file_name=strrchr(path,'\\');
    if(file_name!=NULL)
    {
        return file_name+1;
    }
    return file_name;
}


// 由于路径不包含后缀了文件名，返回的是指针，没有复制内容
char *get_file_name2(char *path)
{

    char *suffix;
    char *file_name;

    file_name=strrchr(path,'\\');
    if(file_name!=NULL)
    {
        file_name++;
    }
    else
    {
        file_name=path;
    }


    suffix=strrchr(file_name,'.');

    int length=strlen(file_name);
    if(suffix!=NULL)
    {
        if(strlen(suffix)<6)
        {
            // 包含后缀
            length=length-strlen(suffix);
        }
    }

    char *name2;
    name2=(char *)malloc(length+1);
    name2[length]='\0';

    for(int i=0; i<length; i++)
    {
        name2[i]=file_name[i];
    }

    return name2;



}


char* get_suffix(char* str)
{
    char* result= strrchr(str,'.');
    if(result==NULL)
    {
        return str;
    }
    return result;
}


int det_char(char i)
{
    switch(i)
    {
    case '1':
        return 1;
    case '2':
        return 2;
    case '3':
        return 3;
    case '4':
        return 4;
    case '5':
        return 5;
    case '6':
        return 6;
    case '7':
        return 7;
    case '8':
        return 8;
    case '9':
        return 9;
    case '0':
        return 0;
    case '.':
        return -1;
    case ':':
        return -2;
    case '\0':
        return -5;
    default:
        return -6;
    }
    return -9;
}

int det_chars(char *str)
{
    // 小数点前，小数点后，指针，取回的值
    int main=0;

    for(size_t i=0; i<strlen(str); i++)
    {
        int  j=det_char(str[i]);
        if(j>=0)
        {
            main=main*10+j;
        }
        else
        {
            break;
        }
    }

    return main;
}


long long int det_longlongchars(char *str)
{
    // 字符串转为长长整形
    long long int main=0;

    for(size_t i=0; i<strlen(str); i++)
    {
        int  j=det_char(str[i]);
        if(j>=0)
        {
            main=main*10+j;
        }
        else
        {
            break;
        }
    }

    return main;
}


int det_chars_int_length(char *str)
{
    // 字符串里的数字的长度
    int length=0;

    for(size_t i=0; i<strlen(str); i++)
    {
        int  j=det_char(str[i]);
        if(j>=0)
        {
            length++;
        }
        else
        {
            break;
        }
    }

    return length;
}

int str2ms(char *str)
{
    size_t p=0, hour=0,minute=0,second=0,milesecond=0;
    size_t length=strlen(str);

    for(p=0; p<length; p++)
    {
        int i=det_char(str[p]);
        if(i>=0)
        {
            break;
        }
    }

    if(p>=strlen(str))
    {
        printf("str2ms() error,input %s\n",str);
        return 0;
    }


    hour=det_chars(str+p);

    char *s1,*s2;//,*s3;
    s1= strchr(str+p,':');
    if(s1==NULL)
    {
        printf("str2ms() error,find 1st : %s\n",str);
        return 0;
    }

    minute=det_chars(s1+1);

//    printf("hour %s\nmin %s\n \n",str+p,s1+1);

    s2= strchr(s1+1,':');
    if(s2==NULL)
    {
        printf("str2ms() error,find 2nd : %s\n",s1+1);
        return 0;
    }

    //  printf("\nhour %s\nmin %s\nsecond %s\n",str+p,s1+1,s2+1);

    second=det_chars(s2+1);
    milesecond=det_char(s2[4])*100+det_char(s2[5])*10;

    /*    s3= strchr(s2+1,'.');
        if(s3==NULL)
        {
            printf("str2ms() error,find . %s\n",s2+1);
            return 0;
        }
        milesecond=det_char(s3[1])*100+det_char(s3[2])*10;
    */

    int result=((hour*60+minute)*60+second)*1000+milesecond;

    printf("\t%dh %dm %d.%03ds (fun return:%d)\t",(int)hour,(int)minute,(int)second,(int)milesecond,result);
    return result;
}


char* time_ms_to_str(int time)
{
    char *str;
//   123:56:78:012
    str=(char *)malloc(13);
    //  str[12]='\0';

    if(time<0)
    {
        time=0;
    }
    int h=0,m=0,s=0,ms=0,left=0,ms2=0;

    ms=time%1000;
    ms2=ms/10;
    left=(time-ms)/1000;

    s=left%60;
    left=(left-s)/60;

    m=left%60;
    h=(left-m)/60;

    sprintf(str, "%02d", h);
    sprintf(str+strlen(str),":%02d",m);
    sprintf(str+strlen(str),":%02d",s);
    sprintf(str+strlen(str),".%02d",ms2);

    return str;
}


// 75*588=44100
// 第一段是分钟，第二段是秒，第三段是帧数（frame）。一秒钟的音乐包含75个帧，数字从零开始，到74为止

char* time_ms_to_str2(int time)
{

    int m=time/60000;
    int s=time/1000-60*m;
    int f=(time-1000*s-60000*m)*75/1000;
    int length=9;
    int L=log10(m);
    if(m>1)
    {
        length=L+8;
    }

    char *str;
    str=(char *)malloc(length);

    sprintf(str, "%02d", m);
    sprintf(str+strlen(str),":%02d",s);
    sprintf(str+strlen(str),":%02d",f);
    return str;
}



#endif // T_STR_H_INCLUDED


