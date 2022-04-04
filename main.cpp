#include <iostream>
#include <direct.h>

using namespace std;

#include "t_str.h"

// �ļ����Ƶ��Զ���ţ�<0ʱ��ʹ���Զ����
int gd_index = -1;
bool detFile(char* targetPath)
{
	// ����ļ��Ƿ����,����ǿ��ļ���ɾ����������ڷǿ��ļ�����true
	FILE* outfile = fopen(targetPath, "rb");
	if (outfile != NULL)
	{
		fseek(outfile, 0, SEEK_END);   ///���ļ�ָ���ƶ��ļ���β
		long size;
		size = ftell(outfile); ///�����ǰ�ļ�ָ������ļ���ʼ���ֽ���
		fclose(outfile);
		if (size > 0)
		{
			return true;
		}
		else
		{
			remove(targetPath);
			printf("Det empety file: %s\n", targetPath);
		}
	}
	else
	{
		//            printf("Not find file: %s\n",targetPath);
	}
	return false;
}



/**
* char *fileName    ����log���ļ���
* char *content     д�������
* char *path        �ο�·������������log·����
*/

bool writeLogLine(const char* fileName, char* content, char* path)
{
	//    char* sample_name=get_name_from_url(content);
	//    printf("get name: %s\n",sample_name);

	int content_length = strlen(content);


	char* cache = new char[content_length + 1];

	//  ȥ��url�еĿո�
	int j = 0;
	for (size_t i = 0; i < strlen(content); i++)
	{
		if (content[i] != ' ' && content[i] != '\t' && content[i] != '\r' && content[i] != '\n' && content[i] != '\0')
		{
			cache[j] = content[i];
			j++;
		}
	}

	cache[j] = '\0';

	if (strlen(cache) < 8)
	{
		return false;
	}
	content = cache;

	char* full_name;
	char* name = NULL;
	if (gd_index > 0)
	{
		char* pos_dot = strrchr(cache, '.');

		if (pos_dot != NULL)
		{
			int len = strlen(pos_dot);
			if (len < 6 && len>1)
			{
				name = (char*)malloc(len + ceil(log10(gd_index)) + 2);
				len = sprintf(name, "%d%s", gd_index, pos_dot);
				//	name[len] = '\0';
			}
		}

		if (name == NULL)
		{
			full_name = url_to_local_path(content, path);
		}
		else
		{
			full_name = new_file_path(path, name);
			gd_index++;
		}
	}
	else
	{
		full_name = url_to_local_path(content, path);
	}

	if (detFile(full_name))
	{
		// ����Ѿ����ز��Ҳ�Ϊ�գ�����������
		free(full_name);
	}
	else
	{
		free(full_name);
		FILE* fp;									//�����ļ�ָ��
		if ((fp = fopen(fileName, "a")) == NULL)			//��ָ���ļ�������ļ����������½����ļ�
		{
			printf("Open Failed.\n");
			return false;
		}

		//��ʽ��д���ļ���׷�����ļ�ĩβ��
		if (name != NULL)
		{
			fprintf(fp, "%s\n out=%s\n", content, name);
			free(name);

		}
		else
		{
			fprintf(fp, "%s\n", content);
		}

		fclose(fp);
		return true;
	}
	return false;
}


// �����������ַ������Ƚϳ���Ҫ���ص�����
int cal_gd(char* gda, char* gdb)
{

	//   printf("����-gda-%s-\n����gdb-%s-\n",gda,gdb);
	printf("��ʼ����gda:\t%s\n��ֹ����gdb:\t%s\n", gda, gdb);
	char* a;
	char* b;

	int A = strlen(gda);
	int B = strlen(gdb);

	if (A > B)
	{
		a = gdb;
		b = gda;
	}
	else
	{
		a = gda;
		b = gdb;
	}

	A = strlen(a);
	B = strlen(b);

	int p = 0, q = 0;

	for (int i = 0; i < A; i++)
	{
		if (a[i] == b[i])
		{
			p++;
		}
		else
		{
			// �в��쵫�ǲ������߾�Ϊ���֣�˵���������쳣��
			if (det_char(a[i]) < 0 || det_char(a[i]) < 0)
			{
				p++;
				printf("�����gda-gdb��%s - %s\n", a + i, b + i);
				return 0;
			}
			else
			{
				q = 0;

				while (det_char(a[i + q - 1]) > -1)
				{
					q--;
					if (i + q < 1)
					{
						break;
					}
				}

				long long int count_a = det_longlongchars(a + i + q);


				long long int count_b = det_longlongchars(b + i + q);
				//  printf("�������%lld\n",count_b);

				//       printf("�������%lld,%lld\n",count_a, count_b);


				// ��Ϊ�в��죬���Բ����ܳ���counta=countb;
				if (count_a > -1 && count_b > -1)
				{
					if (b - a > 10000)
					{
						printf("����������������ܴ������ô���-gda-gdb-/count_a-count_b��\n-%s-%s- / %lld-%lld\n", a + i + q, b + i + q, count_a, count_b);
					}
					//       printf("�������-gda-gdb-/count_a-count_b/b-a��\n-%s-%s- / %lld-%lld/%d\n",a+i+q,b+i+q,count_a,count_b,count_b-count_a);

					return count_b - count_a + 1;
				}
				else
				{
					printf("��ֵת���쳣gda-gdb��%s - %s\n", a + i + q, a + i + q);
					return 0;
				}

			}
		}
	}
	return 0;
}

// �������ַ����Ƚϣ��������������������ӡ�

int run_gd(char* gda, char* gdb, char* log_path, char* gd_filename)
{


	int counter = 0;

	char* a;
	char* b;

	int A = strlen(gda);
	int B = strlen(gdb);

	if (A > B)
	{
		a = gdb;
		b = gda;
	}
	else
	{
		a = gda;
		b = gdb;
	}

	A = strlen(a);
	B = strlen(b);

	int p = 0, q = 0;

	for (int i = 0; i < A; i++)
	{
		if (a[i] == b[i])
		{
			p++;
		}
		else
		{
			// �в��쵫�ǲ������߾�Ϊ���֣�˵���������쳣��
			if (det_char(a[i]) < 0 || det_char(a[i]) < 0)
			{
				p++;
				printf("�����gda-gdb��%s - %s\n", a + i, b + i);
				return 0;
			}
			else
			{

				// i�Ǳ��������ַ��������������λ�ã�q����ǰ�������������λ�á�
				q = 0;

				while (det_char(a[i + q - 1]) > -1)
				{
					q--;
					if (i + q < 1)
					{
						break;
					}
				}

				// ���ڿ��ܴ���λ�����죬����ֱ��ʹ����ͬ�Ĳ���.�������Ϊ0�������ã�Ȼ����û�κ�����
				//     while(det_char(a+i+q)==0){
				//       q++;
				// }

				int n = 0;

				long long int count_a = det_longlongchars(a + i + q);
				long long int count_b = det_longlongchars(b + i + q);


				//    ��%03d ���Կ���C�������������ʽ��Ĭ�����λ��Ϊ3λ�������������
				//  longlongint�ķ�Χ -9223372036854775808��+9223372036854775807���������19λ��
				// ���ǵ����Ȳ�ȷ���������Ҫ���㣬����gda�����ֵĳ��ȼ�����������
				// �����Ǹ�λ�ӹ̶�����0�����⡣
				int print_length = det_chars_int_length(a + i + q);
				char* link = new char[B];
				char* link_x = new char[i + q + 1];
				link_x[i + q] = '\0';
				strncpy(link_x, a, i + q);


				// char link_y[21];
				char* link_z = new char[A - i - q + 1 - print_length];
				link_z[B - i - q - print_length] = '\0';
				strcpy(link_z, a + i + q + print_length);

				//     printf("run_gd:print_length%d\n",print_length);

				switch (print_length)
				{
				case 1:
					while (count_b - count_a >= n)
					{
						sprintf(link, "%s%01lld%s", link_x, count_a + n, link_z);
						//       sprintf(link_y,"%01d",count_a+n);
						//     sprintf(link,"%s%s%s",link_x,link_y,link_z);
						if (writeLogLine(log_path, link, gd_filename))
							counter++;
						n++;
					}

					break;
				case 2:
					while (count_b - count_a >= n)
					{
						sprintf(link, "%s%02lld%s", link_x, count_a + n, link_z);
						if (writeLogLine(log_path, link, gd_filename))
							counter++;
						n++;
					}
					break;
				case 3:
					while (count_b - count_a >= n)
					{
						sprintf(link, "%s%03lld%s", link_x, count_a + n, link_z);
						if (writeLogLine(log_path, link, gd_filename))
							counter++;
						n++;
					}
					break;
				case 4:
					while (count_b - count_a >= n)
					{
						sprintf(link, "%s%04lld%s", link_x, count_a + n, link_z);
						if (writeLogLine(log_path, link, gd_filename))
							counter++;
						n++;
					}
					break;
				case 5:
					while (count_b - count_a >= n)
					{
						//             printf("n=%d",n);
						sprintf(link, "%s%05lld%s", link_x, count_a + n, link_z);
						if (writeLogLine(log_path, link, gd_filename))
							counter++;
						n++;
					}

					break;
				case 6:
					while (count_b - count_a >= n)
					{
						sprintf(link, "%s%06lld%s", link_x, count_a + n, link_z);
						if (writeLogLine(log_path, link, gd_filename))
							counter++;
						n++;
					}
					break;
				case 7:
					while (count_b - count_a >= n)
					{
						sprintf(link, "%s%07lld%s", link_x, count_a + n, link_z);
						if (writeLogLine(log_path, link, gd_filename))
							counter++;
						n++;
					}
					break;
				case 8:
					while (count_b - count_a >= n)
					{
						sprintf(link, "%s%08lld%s", link_x, count_a + n, link_z);
						if (writeLogLine(log_path, link, gd_filename))
							counter++;
						n++;
					}
					break;
				case 9:
					while (count_b - count_a >= n)
					{
						sprintf(link, "%s%09lld%s", link_x, count_a + n, link_z);
						if (writeLogLine(log_path, link, gd_filename))
							counter++;
						n++;
					}
					break;
				case 10:
					while (count_b - count_a >= n)
					{
						sprintf(link, "%s%010lld%s", link_x, count_a + n, link_z);
						if (writeLogLine(log_path, link, gd_filename))
							counter++;
						n++;
					}
					break;
				case 11:
					while (count_b - count_a >= n)
					{
						sprintf(link, "%s%011lld%s", link_x, count_a + n, link_z);
						if (writeLogLine(log_path, link, gd_filename))
							counter++;
						n++;
					}
					break;
				case 12:
					while (count_b - count_a >= n)
					{
						sprintf(link, "%s%012lld%s", link_x, count_a + n, link_z);
						if (writeLogLine(log_path, link, gd_filename))
							counter++;
						n++;
					}
					break;
				case 13:
					while (count_b - count_a >= n)
					{
						sprintf(link, "%s%013lld%s", link_x, count_a + n, link_z);
						if (writeLogLine(log_path, link, gd_filename))
							counter++;
						n++;
					}
					break;
				case 14:
					while (count_b - count_a >= n)
					{
						sprintf(link, "%s%014lld%s", link_x, count_a + n, link_z);
						if (writeLogLine(log_path, link, gd_filename))
							counter++;
						n++;
					}
					break;
				case 15:
					while (count_b - count_a >= n)
					{
						sprintf(link, "%s%015lld%s", link_x, count_a + n, link_z);
						if (writeLogLine(log_path, link, gd_filename))
							counter++;
						n++;
					}
					break;
				case 16:
					while (count_b - count_a >= n)
					{
						sprintf(link, "%s%016lld%s", link_x, count_a + n, link_z);
						if (writeLogLine(log_path, link, gd_filename))
							counter++;
						n++;
					}
					break;
				case 17:
					while (count_b - count_a >= n)
					{
						sprintf(link, "%s%017lld%s", link_x, count_a + n, link_z);
						if (writeLogLine(log_path, link, gd_filename))
							counter++;
						n++;
					}
					break;
				case 18:
					while (count_b - count_a >= n)
					{
						sprintf(link, "%s%018lld%s", link_x, count_a + n, link_z);
						if (writeLogLine(log_path, link, gd_filename))
							counter++;
						n++;
					}
					break;
				case 19:
					while (count_b - count_a >= n)
					{
						sprintf(link, "%s%019lld%s", link_x, count_a + n, link_z);
						if (writeLogLine(log_path, link, gd_filename))
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

int  main(int argc, char* argv[])
{
	printf("Grid Downloader \nver0.1, by tumuyan \nhttps://github.com/tumuyan/Gird-Downloader\n\n");
	//  ��������ʱ���ٻ����2����������һ�������ǳ�����
	if (argc < 2)
	{
		printf("��������ȷ��gdԴ�ļ�\n");
		//  system("PAUSE");
		exit(-1);
	}

	// gdԴ���ļ�·��
	char* gd_filename = NULL;
	char* log_path;
	log_path = new_file_path(argv[0], (char*)"file.log");

	//char* aria_path;
	//aria_path = new_file_path(argv[0], (char*)"aria2c.exe");


	//char path[_MAX_PATH];
	//getcwd(path, _MAX_PATH);
	//printf("��ǰ����Ŀ¼:\n%s\n", path);

	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			for (size_t j = 1; j < strlen(argv[i]); j++)
			{
				if (argv[i][j] == 'r')
					gd_index = 1;
			}
		}
		else if (gd_filename == NULL)
		{
			gd_filename = argv[i]; //�ļ���
		}
	}

	if (gd_index < 0) {
		printf("�Ƿ��԰����ص��ļ����Ƹ�Ϊ���(Ĭ�ϲ�������)��[y/n]");
		char c = getchar();
		if (c == 'y') {
			printf(" =>��\n");
			gd_index = 1;
		}
		else {
			printf(" =>��\n");
		}
	}

	// ����gdԴ�ļ�

	FILE* fp;
	char StrLine[8000];             //ÿ������ȡ���ַ���
	if ((fp = fopen(gd_filename, "r")) == NULL) //�ж��ļ��Ƿ���ڼ��ɶ�
	{
		printf("��������Ч���ļ�!");
		return -1;
	}
	else
	{
		printf("Remove file.log\t%s\n", log_path);
		remove(log_path);
		printf("Analysis File %s\n", gd_filename);
		int m = 0, n = 0;

		char* gda_path;

		while (!feof(fp))
		{
			m++;
			fgets(StrLine, 8000, fp);  //��ȡһ��


			{
				int content_length = strlen(StrLine);

				if (content_length < 6)
				{
					m--;
					continue;
				}

				if (StrLine[0] == 'g' && StrLine[1] == 'd')
				{
					// ��̽���� gda / gdb
					// ����gda ��ʶ�Ӵ�url��ʼ������gdb��ʶ��ֹ��url��


					char* gd_link = new char[content_length - 3];
					//  gd_link[content_length-4]='\0';
					//ȥ��url�еĿո�
					int j = 0;
					//   for(int i=4; i<strlen(StrLine); i++)
					size_t i = 4;
					while (i < strlen(StrLine))
					{
						if (StrLine[i] != ' ' && StrLine[i] != '\t' && StrLine[i] != '\r' && StrLine[i] != '\n' && StrLine[i] != '\0')
						{
							gd_link[j] = StrLine[i];
							j++;
						}
						i++;
					}
					gd_link[j] = '\0';


					// printf("����gd-link��tpye[%c]��%s-\n��������%s\n\n",StrLine[2],gd_link,StrLine);



					if (StrLine[2] == 'a')
					{
						if (!gda_path)
							free(gda_path);
						gda_path = keepstr(gd_link);
					}
					else if (StrLine[2] == 'b')
					{
						int gd_range = cal_gd(gda_path, gd_link);
						printf("����������=%d\n", gd_range);
						if (gd_range > 0)
						{
							m += gd_range;
							n += run_gd(gda_path, gd_link, log_path, gd_filename);
						}

					}
					m--;
					continue;
				}
			}

			if (writeLogLine(log_path, StrLine, gd_filename))
			{
				//         printf("%d%s\n", n,StrLine); //���
				n++;
				//  printf("%s\n", StrLine); //���
			}
		}
		fclose(fp);


		printf("Analysis Finished, %d/%d need download\n", n, m);

		if (n > 0)
		{
			system("PAUSE");
			char* cmd_down = keep2str((char*)"aria2c.exe   -i file.log -d ", get_folder_from_path(gd_filename));
			system(cmd_down);
		}


		//       system( "A:\\ProjectC\\GirdDownloader\\aria2\\aria2c.exe   -i file.log");
	}


	system("PAUSE");
	return 0;

}
