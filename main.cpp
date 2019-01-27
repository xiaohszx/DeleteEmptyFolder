#include <stdint.h>
#include <io.h>
#include <string>
#include <vector>
#include <windows.h>
#include <iostream>

// �����ǰĿ¼Ϊ�գ���ɾ����Ŀ¼
bool IsEmptyFolder(const std::string &folder_path)
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
					if (IsEmptyFolder(child)){
						BOOL b = RemoveDirectory(child.c_str());
						if (FALSE == b) ret = false;
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

bool DeleteEmptyFolder(const char *folder) { return IsEmptyFolder(folder); }

// ɾ����ǰĿ¼�µĿ�Ŀ¼���ݹ�ɾ�����ӿ�Ŀ¼��
int main()
{
	char buf[64];
	printf("Ҫɾ����ǰĿ¼�µĿ�Ŀ¼,���ҵݹ�ɾ�������Ŀ¼��? y/n\n");
	std::cin >> buf;
	if (buf[0] == 'y' || buf[0] == 'Y')
	{
		char folder_path[_MAX_PATH], *p = folder_path;
		GetModuleFileNameA(NULL, folder_path, sizeof(folder_path));
		while (*p) ++p;
		while ('\\' != *p) --p;
		*p = 0;
		DeleteEmptyFolder(folder_path);
		system("PAUSE");
		return 0;
	}

	return -1;
}
