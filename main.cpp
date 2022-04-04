#include <iostream>

using namespace std;

#include "t_str.h"

/* GirdDownloader说明
gd是一个调用aria2来进行批量下载任务的工具
用一个包含下载链接的文本文件（定义这个文件为“源”）作为参数来打开gd，gd会下载链接中的文件到源所在的目录，同时自动跳过目录中已有的文件。
源的每一行都是一个下载链接；同时gd会自动忽略 每一行中的空字符。
源的某一行开头以`gda `，且下一行开头以`gdb `，则自动比较两个url中的数字的关联性，并下载一系列文件
如果已经下载并且不为空，则不重新下载
程序调用时允许接收参数 `-r`，下载文件时自动重命名文件为数字编号
*/

//str * url_cache;


int gd_index=-1;
bool detFile(char *targetPath)
{
    // 检查文件是否存在,如果是空文件就删除；如果存在非空文件返回true
    FILE *outfile = fopen(targetPath, "rb" );
    if(outfile != NULL)
    {
        fseek (outfile, 0, SEEK_END);   ///将文件指针移动文件结尾
        long size;
        size=ftell (outfile); ///求出当前文件指针距离文件开始的字节数
        fclose(outfile);
        if(size>0)
        {
            return true;
        }
        else
        {
            remove(targetPath);
            printf("Det empety file: %s\n",targetPath);
        }
    }
    else
    {
        //            printf("Not find file: %s\n",targetPath);
    }
    return false;
}



/**
* char *fileName    保存log的文件名
* char *content     写入的内容
* char *path        参考路径（用于生成log路径）
*/

bool writeLogLine(const char *fileName, char *content,char *path)
{
//    char* sample_name=get_name_from_url(content);
//    printf("get name: %s\n",sample_name);

    int content_length=strlen(content);


    char cache[content_length+1];

    //  去除url中的空格
    int j=0;
    for(size_t i=0; i<strlen(content); i++)
    {
        if(content[i]!=' ' && content[i]!='\t'&& content[i]!='\r'&& content[i]!='\n' && content[i]!='\0')
        {
            cache[j]= content[i];
            j++;
        }
    }

    cache[j]='\0';

    if(strlen(cache)<8)
    {
        return false;
    }
    content=cache;

    char* full_name;
    char* name = NULL;
    if(gd_index>0)
    {
     //   name = url_to_local_name(content,gd_index);


        char *pos_dot=strrchr(cache,'.');

        if(pos_dot != NULL)
        {
            int len = strlen(pos_dot);
            if(len<6 &&len>1)
            {
                // len =len+ ceil(log10(index))+1;
                name = (char *)malloc(len +  ceil(log10(gd_index)) + 1);
                len = sprintf(name,"%d%s",gd_index,pos_dot);
                name[len] = '\0';
            }
        }

        //     sprintf(name, "")
        if(name==NULL)
        {
            full_name=url_to_local_path(content,path);
        }
        else
        {
            full_name = new_file_path(path,name);
            gd_index++;
        }
    }
    else
    {
        full_name=url_to_local_path(content,path);
    }

    if(detFile(full_name))
    {
        // 如果已经下载并且不为空，则不重新下载
        free(full_name);
    }
    else
    {
        free(full_name);
        FILE *fp;									//定义文件指针
        if ((fp=fopen(fileName,"a"))==NULL)			//打开指定文件，如果文件不存在则新建该文件
        {
            printf("Open Failed.\n");
            return false;
        }

        //格式化写入文件（追加至文件末尾）
        if(name != NULL)
        {
            fprintf(fp,"%s\n out=%s\n",content, name );
            free(name);
        }
        else
        {
            fprintf(fp,"%s\n",content);
        }

        fclose(fp);
        return true;
    }
    return false;
}


// 从两个链接字符串，比较出需要下载的数量
int cal_gd(char *gda,char *gdb)
{

//   printf("输入-gda-%s-\n输入gdb-%s-\n",gda,gdb);
    printf("起始链接gda:\t%s\n终止链接gdb:\t%s\n",gda,gdb);
    char *a;
    char *b;

    int A=strlen(gda);
    int B=strlen(gdb);

    if(A>B)
    {
        a=gdb;
        b=gda;
    }
    else
    {
        a=gda;
        b=gdb;
    }

    A=strlen(a);
    B=strlen(b);

    int p=0,q=0;

    for(int i=0; i<A; i++)
    {
        if(a[i]==b[i])
        {
            p++;
        }
        else
        {
            // 有差异但是并非两者均为数字，说明出现了异常。
            if(det_char(a[i])<0 || det_char(a[i])<0 )
            {
                p++;
                printf("错误的gda-gdb：%s - %s\n",a+i,b+i);
                return 0;
            }
            else
            {
                q=0;

                while(det_char(a[i+q-1])>-1)
                {
                    q--;
                    if(i+q<1)
                    {
                        break;
                    }
                }

                long long int count_a=det_longlongchars(a+i+q);


                long long int count_b=det_longlongchars(b+i+q);
                //  printf("解析结果%lld\n",count_b);

                //       printf("解析结果%lld,%lld\n",count_a, count_b);


                // 因为有差异，所以不可能出现counta=countb;
                if (count_a>-1 && count_b>-1 )
                {
                    if(b-a>10000)
                    {
                        printf("过大的下载量，可能存在配置错误-gda-gdb-/count_a-count_b：\n-%s-%s- / %lld-%lld\n",a+i+q,b+i+q,count_a,count_b);
                    }
                    //       printf("解析结果-gda-gdb-/count_a-count_b/b-a：\n-%s-%s- / %lld-%lld/%d\n",a+i+q,b+i+q,count_a,count_b,count_b-count_a);

                    return count_b-count_a+1;
                }
                else
                {
                    printf("数值转换异常gda-gdb：%s - %s\n",a+i+q,a+i+q);
                    return 0;
                }

            }
        }
    }
    return 0;
}

// 用两个字符串比较，产生出批量的下载链接。

int run_gd(char *gda,char *gdb,char *log_path,char *gd_filename)
{


    int counter=0;

    char *a;
    char *b;

    int A=strlen(gda);
    int B=strlen(gdb);

    if(A>B)
    {
        a=gdb;
        b=gda;
    }
    else
    {
        a=gda;
        b=gdb;
    }

    A=strlen(a);
    B=strlen(b);

    int p=0,q=0;

    for(int i=0; i<A; i++)
    {
        if(a[i]==b[i])
        {
            p++;
        }
        else
        {
            // 有差异但是并非两者均为数字，说明出现了异常。
            if(det_char(a[i])<0 || det_char(a[i])<0 )
            {
                p++;
                printf("错误的gda-gdb：%s - %s\n",a+i,b+i);
                return 0;
            }
            else
            {

                // i是遍历两个字符串，发生差异的位置；q是向前搜索发生差异的位置。
                q=0;

                while(det_char(a[i+q-1])>-1)
                {
                    q--;
                    if(i+q<1)
                    {
                        break;
                    }
                }

                // 由于可能存在位数差异，不可直接使用相同的部分.但是如果为0可以利用，然而并没任何意义
                //     while(det_char(a+i+q)==0){
                //       q++;
                // }

                int n=0;

                long long int count_a=det_longlongchars(a+i+q);
                long long int count_b=det_longlongchars(b+i+q);


                //    用%03d 可以控制C语言输出整数格式，默认输出位数为3位，如果不够补零
                //  longlongint的范围 -9223372036854775808～+9223372036854775807，长度最大19位。
                // 考虑到长度不确定，如果需要补零，补到gda的数字的长度即可满足需求
                // 不考虑高位加固定个的0的问题。
                int print_length=det_chars_int_length(a+i+q);
                char link[B];
                char link_x[i+q+1];
                link_x[i+q]='\0';
                strncpy(link_x,a,i+q);


                // char link_y[21];
                char link_z[A-i-q+1-print_length];
                link_z[B-i-q-print_length]='\0';
                strcpy(link_z,a+i+q+print_length);

                //     printf("run_gd:print_length%d\n",print_length);

                switch(print_length)
                {
                case 1:
                    while(count_b-count_a>=n)
                    {
                        sprintf(link,"%s%01lld%s",link_x,count_a+n,link_z);
                        //       sprintf(link_y,"%01d",count_a+n);
                        //     sprintf(link,"%s%s%s",link_x,link_y,link_z);
                        if(writeLogLine(log_path,link,gd_filename))
                            counter++;
                        n++;
                    }

                    break;
                case 2:
                    while(count_b-count_a>=n)
                    {
                        sprintf(link,"%s%02lld%s",link_x,count_a+n,link_z);
                        if(writeLogLine(log_path,link,gd_filename))
                            counter++;
                        n++;
                    }
                    break;
                case 3:
                    while(count_b-count_a>=n)
                    {
                        sprintf(link,"%s%03lld%s",link_x,count_a+n,link_z);
                        if(writeLogLine(log_path,link,gd_filename))
                            counter++;
                        n++;
                    }
                    break;
                case 4:
                    while(count_b-count_a>=n)
                    {
                        sprintf(link,"%s%04lld%s",link_x,count_a+n,link_z);
                        if(writeLogLine(log_path,link,gd_filename))
                            counter++;
                        n++;
                    }
                    break;
                case 5:
                    while(count_b-count_a>=n)
                    {
                        //             printf("n=%d",n);
                        sprintf(link,"%s%05lld%s",link_x,count_a+n,link_z);
                        if(writeLogLine(log_path,link,gd_filename))
                            counter++;
                        n++;
                    }

                    break;
                case 6:
                    while(count_b-count_a>=n)
                    {
                        sprintf(link,"%s%06lld%s",link_x,count_a+n,link_z);
                        if(writeLogLine(log_path,link,gd_filename))
                            counter++;
                        n++;
                    }
                    break;
                case 7:
                    while(count_b-count_a>=n)
                    {
                        sprintf(link,"%s%07lld%s",link_x,count_a+n,link_z);
                        if(writeLogLine(log_path,link,gd_filename))
                            counter++;
                        n++;
                    }
                    break;
                case 8:
                    while(count_b-count_a>=n)
                    {
                        sprintf(link,"%s%08lld%s",link_x,count_a+n,link_z);
                        if(writeLogLine(log_path,link,gd_filename))
                            counter++;
                        n++;
                    }
                    break;
                case 9:
                    while(count_b-count_a>=n)
                    {
                        sprintf(link,"%s%09lld%s",link_x,count_a+n,link_z);
                        if(writeLogLine(log_path,link,gd_filename))
                            counter++;
                        n++;
                    }
                    break;
                case 10:
                    while(count_b-count_a>=n)
                    {
                        sprintf(link,"%s%010lld%s",link_x,count_a+n,link_z);
                        if(writeLogLine(log_path,link,gd_filename))
                            counter++;
                        n++;
                    }
                    break;
                case 11:
                    while(count_b-count_a>=n)
                    {
                        sprintf(link,"%s%011lld%s",link_x,count_a+n,link_z);
                        if(writeLogLine(log_path,link,gd_filename))
                            counter++;
                        n++;
                    }
                    break;
                case 12:
                    while(count_b-count_a>=n)
                    {
                        sprintf(link,"%s%012lld%s",link_x,count_a+n,link_z);
                        if(writeLogLine(log_path,link,gd_filename))
                            counter++;
                        n++;
                    }
                    break;
                case 13:
                    while(count_b-count_a>=n)
                    {
                        sprintf(link,"%s%013lld%s",link_x,count_a+n,link_z);
                        if(writeLogLine(log_path,link,gd_filename))
                            counter++;
                        n++;
                    }
                    break;
                case 14:
                    while(count_b-count_a>=n)
                    {
                        sprintf(link,"%s%014lld%s",link_x,count_a+n,link_z);
                        if(writeLogLine(log_path,link,gd_filename))
                            counter++;
                        n++;
                    }
                    break;
                case 15:
                    while(count_b-count_a>=n)
                    {
                        sprintf(link,"%s%015lld%s",link_x,count_a+n,link_z);
                        if(writeLogLine(log_path,link,gd_filename))
                            counter++;
                        n++;
                    }
                    break;
                case 16:
                    while(count_b-count_a>=n)
                    {
                        sprintf(link,"%s%016lld%s",link_x,count_a+n,link_z);
                        if(writeLogLine(log_path,link,gd_filename))
                            counter++;
                        n++;
                    }
                    break;
                case 17:
                    while(count_b-count_a>=n)
                    {
                        sprintf(link,"%s%017lld%s",link_x,count_a+n,link_z);
                        if(writeLogLine(log_path,link,gd_filename))
                            counter++;
                        n++;
                    }
                    break;
                case 18:
                    while(count_b-count_a>=n)
                    {
                        sprintf(link,"%s%018lld%s",link_x,count_a+n,link_z);
                        if(writeLogLine(log_path,link,gd_filename))
                            counter++;
                        n++;
                    }
                    break;
                case 19:
                    while(count_b-count_a>=n)
                    {
                        sprintf(link,"%s%019lld%s",link_x,count_a+n,link_z);
                        if(writeLogLine(log_path,link,gd_filename))
                            counter++;
                        n++;
                    }
                    break;

                }


                return counter;

            }
        }
    }
    return counter;
}


main( int argc, char *argv[] )
{

    //  正常启动时至少会接受2个参数。第一个参数是程序名
    if(argc<2)
    {
        printf("请输入正确的gd源文件\n" );
        //  system("PAUSE");
        exit(-1);
    }

    // 是否自动重命名
    // bool mode_rename;
    // gd源的文件路径
    char *gd_filename =NULL;
    //char *dl_path;
    char *log_path;
    // char keyin_str[1024];
    log_path=new_file_path(argv[0],(char*)"file.log");
    for(int i=1; i<argc; i++)
    {
        if(argv[i][0]=='-')
        {
            for(size_t j=1; j<strlen(argv[i]); j++)
            {
                if(argv[i][j]=='r')
                    gd_index=1;
            }
        }
        else if(gd_filename==NULL)
        {
            gd_filename = argv[i]; //文件名
        }
    }

    // 解析gd源文件

    FILE *fp;
    char StrLine[8000];             //每行最大读取的字符数
    if((fp = fopen(gd_filename,"r")) == NULL) //判断文件是否存在及可读
    {
        printf("输入了无效的文件!");
        return -1;
    }
    else
    {
        //dl_path=get_folder_from_path(gd_filename);

        printf("Remove file.log\t%s\n",log_path);
        remove(log_path);
        //remove("file.log");
        //      char *folder;
        //      folder=get_folder_from_path(gd_filename);
        printf("Analysis File %s\n",gd_filename);
        int m=0,n=0;

        char *gda_path;

        while (!feof(fp))
        {
            m++;
            fgets(StrLine,8000,fp);  //读取一行


            {
                int content_length=strlen(StrLine);

                if(content_length<6)
                {
                    m--;
                    continue;
                }

                if(StrLine[0]=='g' && StrLine[1]=='d')
                {
                    // 侦探命令 gda / gdb
                    // 其中gda 标识从此url开始产生，gdb标识终止的url；


                    char gd_link[content_length-3];
                    //  gd_link[content_length-4]='\0';
                    //去除url中的空格
                    int j=0;
                    //   for(int i=4; i<strlen(StrLine); i++)
                    size_t i=4;
                    while(i<strlen(StrLine))
                    {
                        if(StrLine[i]!=' ' && StrLine[i]!='\t'&& StrLine[i]!='\r'&& StrLine[i]!='\n' && StrLine[i]!='\0')
                        {
                            gd_link[j]= StrLine[i];
                            j++;
                        }
                        i++;
                    }
                    gd_link[j]='\0';


// printf("发现gd-link，tpye[%c]，%s-\n来自内容%s\n\n",StrLine[2],gd_link,StrLine);



                    if(StrLine[2]=='a')
                    {
                        if(!gda_path )
                            free(gda_path);
                        gda_path=keepstr(gd_link);
                    }
                    else if(StrLine[2]=='b')
                    {
                        int gd_range=cal_gd(gda_path,gd_link);
                        printf("生成链接数=%d\n",gd_range);
                        if(gd_range>0)
                        {
                            m+=gd_range;
                            n+=run_gd(gda_path,gd_link,log_path,gd_filename);
                        }

                    }
                    m--;
                    continue;
                }
            }

            if(writeLogLine(log_path,StrLine,gd_filename))
            {
                //         printf("%d%s\n", n,StrLine); //输出
                n++;
                //  printf("%s\n", StrLine); //输出
            }
        }
        fclose(fp);


        printf("Analysis Finished, %d/%d need download\n",n,m);

        if(n>0)
        {
            system("PAUSE");
            char* cmd_down = keep2str((char*)"aria2c.exe   -i file.log -d ",get_folder_from_path(gd_filename));
            system( cmd_down);
        }


//       system( "A:\\ProjectC\\GirdDownloader\\aria2\\aria2c.exe   -i file.log");
    }


    system("PAUSE");
    return 0;

// A:\ProjectC\GirdDownloader\aria2\aria2c.exe  -d download -i A:\ProjectC\GirdDownloader\file.log
//    Download URIs found in text file:
// A:\ProjectC\GirdDownloader\aria2-1.35.0-win-64bit-build1\aria2c.exe  -d ./download -i uris.txt
}
