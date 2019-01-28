#include <stdint.h>
#include <io.h>
#include <string>
#include <vector>
#include <windows.h>
#include <iostream>
#include <time.h>

// �����ǰĿ¼Ϊ�գ���ɾ����Ŀ¼
bool IsEmptyFolder(const std::string &folder_path, int &count)
{
	bool ret = true;
	//�ļ����
	intptr_t hFile = 0;
	//�ļ���Ϣ  
	struct _finddata_t fileinfo;
	std::string p;
	try
	{
		if ((hFile = _findfirst(p.assign(folder_path).append("\\*.*").c_str(), &fileinfo)) != -1)
		{
			do{
				if (0==strcmp(fileinfo.name, ".") || 0==strcmp(fileinfo.name, ".."))
					continue;
				if (FILE_ATTRIBUTE_DIRECTORY & fileinfo.attrib) //��Ŀ¼
				{
					std::string child = folder_path + "\\" + fileinfo.name;
					if (IsEmptyFolder(child, count)){
						BOOL b = RemoveDirectory(child.c_str());
						if (FALSE == b) ret = false;
						else ++count;
						printf("ɾ��\"%s\"%s\n", child.c_str(), b ? "�ɹ�" : "ʧ��");
					}else ret = false;
				}else if (ret) // �ǿ�Ŀ¼
				{
					ret = false;
				}
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}
	}catch (std::exception e){ if(hFile) _findclose(hFile); }

	return ret;
}

bool DeleteEmptyFolder(const char *folder, int &count) { return IsEmptyFolder(folder, count); }

// ɾ����ǰĿ¼�µĿ�Ŀ¼���ݹ�ɾ�����ӿ�Ŀ¼��
int main()
{
	// ��ȡ��ǰĿ¼
	char folder_path[_MAX_PATH], *p = folder_path;
	GetModuleFileNameA(NULL, folder_path, sizeof(folder_path));
	while (*p) ++p;
	while ('\\' != *p) --p;
	*p = 0;
	while (1)
	{
		// ��ȡ�û�����Ŀ¼
		std::string folder;
		do{
			folder = folder_path;
			char path[_MAX_PATH];
			printf("������Ŀ¼:");
			std::cin >> path;
			_strlwr(path);
			if (path[0] == 'q' && strlen(path) == 1) // ����q�˳�
				return -1;
			if (!('a' <= path[0] && path[0] <= 'z'))
				folder.append("\\").append(path);
			else
				folder = path;
		}while (_access(folder.c_str(), 0) == -1);
		// ���ж���ȷ��
		char buf[64];
		printf("��ѡ����Ŀ¼��%s\n", folder.c_str());
		printf("Ҫɾ����Ŀ¼�µĿ�Ŀ¼,���ҵݹ�ɾ�������Ŀ¼��? y/n\n");
		std::cin >> buf;
		if (buf[0] == 'y' || buf[0] == 'Y')
		{
			int count = 0;
			clock_t tick = clock();
			DeleteEmptyFolder(folder.c_str(), count);
			printf("ɾ����Ŀ¼[%d��]��ϣ���ʱ[%d]ms\n", count, int(clock()-tick));
			system("PAUSE");
		}
	}

	return 0;
}
