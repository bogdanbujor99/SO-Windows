#include <iostream>
#include <windows.h>
#include <accctrl.h>
#include <aclapi.h>
#include <sddl.h>
#pragma warning (disable : 4996)
using namespace std;

int createRestrictedWrite()
{
    DWORD computerSize = 256;
    LPSTR computerName = new char[256];
    GetComputerName(computerName, &computerSize);
    DWORD userSize = 256;
    LPSTR userName = new char[256];
    GetUserName(userName, &userSize);
    char* computerAndUser = new char[computerSize + userSize + 5];
    strcpy(computerAndUser, computerName);
    strcat(computerAndUser, "\\");
    strcat(computerAndUser, userName);
    char domain[256];
    DWORD  domainSize = strlen(domain);
    BYTE userSid[SECURITY_MAX_SID_SIZE];
    DWORD sidSize = SECURITY_MAX_SID_SIZE;
    SID_NAME_USE nameUser;
    LookupAccountNameA(NULL, computerAndUser, userSid, &sidSize, domain, &domainSize, &nameUser);

    PSID everyoane = malloc(SECURITY_MAX_SID_SIZE);
    PSECURITY_DESCRIPTOR sD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
    DWORD dwRes;
    SECURITY_ATTRIBUTES sA;
    DWORD dpsidEveryoane;
    CreateWellKnownSid(WinWorldSid, NULL, everyoane, &dpsidEveryoane);

    EXPLICIT_ACCESS ea[2];
    ea[0].grfAccessMode = SET_ACCESS;
    ea[0].grfAccessPermissions = FILE_GENERIC_READ | FILE_GENERIC_WRITE;
    ea[0].grfInheritance = NO_INHERITANCE;
    ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
    ea[0].Trustee.ptstrName = (char*)userSid;

    ea[1].grfAccessMode = SET_ACCESS;
    ea[1].grfAccessPermissions = FILE_GENERIC_READ;
    ea[1].grfInheritance = NO_INHERITANCE;
    ea[1].Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea[1].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
    ea[1].Trustee.ptstrName = (char*)everyoane;

    PACL pA = NULL;
    dwRes = SetEntriesInAcl(2, ea, NULL, &pA);

    if (!InitializeSecurityDescriptor(sD, SECURITY_DESCRIPTOR_REVISION))
    {
        printf("error to InitializeSecurityDescriptor");
        return 0;
    }
    if (!SetSecurityDescriptorDacl(sD, true, pA, false))
    {
        printf("error to SetSecurityDescriptorDacl");
        return 0;
    }
    if (!SetSecurityDescriptorSacl(sD, false, NULL, false))
    {
        printf("error to SetSecurityDescriptorSacl");
        return 0;
    }

    sA.nLength = sizeof(SECURITY_ATTRIBUTES);
    sA.lpSecurityDescriptor = sD;
    sA.bInheritHandle = TRUE;
    if (!(CreateDirectory("D:\\RESTRICTED WRITE", &sA)))
    {
        printf("error to CreateDirectory: %d", GetLastError());
        return 0;
    }
    return 1;
}

int createLOGS()
{
    PSID everyoane = malloc(SECURITY_MAX_SID_SIZE);
    PSECURITY_DESCRIPTOR sD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
    DWORD dwRes;
    SECURITY_ATTRIBUTES sA;
    DWORD dpsidEveryoane;
    CreateWellKnownSid(WinWorldSid, NULL, everyoane, &dpsidEveryoane);
    EXPLICIT_ACCESS ea[1];
    ea[0].grfAccessMode = SET_ACCESS;
    ea[0].grfAccessPermissions = FILE_GENERIC_READ | FILE_GENERIC_WRITE;
    ea[0].grfInheritance = NO_INHERITANCE;
    ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
    ea[0].Trustee.ptstrName = (char*)everyoane;
    PACL pA = NULL;
    dwRes = SetEntriesInAcl(1, ea, NULL, &pA);
    if (dwRes != ERROR_SUCCESS)
    {
        printf("error to SetEntriesInAcl: %d", dwRes);
        return 0;
    }
    if (!InitializeSecurityDescriptor(sD, SECURITY_DESCRIPTOR_REVISION))
    {
        printf("error to InitializeSecurityDescriptor");
        return 0;
    }
    if (!SetSecurityDescriptorDacl(sD, true, pA, false))
    {
        printf("error to SetSecurityDescriptorDacl");
        return 0;
    }
    if (!SetSecurityDescriptorSacl(sD, false, NULL, false))
    {
        printf("error to SetSecurityDescriptorSacl");
        return 0;
    }

    sA.nLength = sizeof(SECURITY_ATTRIBUTES);
    sA.lpSecurityDescriptor = sD;
    sA.bInheritHandle = TRUE;
    if (!(CreateDirectory("D:\\LOGS", &sA)))
    {
        printf("error to CreateDirectory: %d", GetLastError());
        return 0;
    }
    return 1;
}

int createKeyP1()
{
    DWORD computerSize = 256;
    LPSTR computerName = new char[256];
    GetComputerName(computerName, &computerSize);
    DWORD userSize = 256;
    LPSTR userName = new char[256];
    GetUserName(userName, &userSize);
    char* computerAndUser = new char[computerSize + userSize + 5];
    strcpy(computerAndUser, computerName);
    strcat(computerAndUser, "\\");
    strcat(computerAndUser, userName);
    char domain[256];
    DWORD  domainSize = strlen(domain);
    BYTE userSid[SECURITY_MAX_SID_SIZE];
    DWORD sidSize = SECURITY_MAX_SID_SIZE;
    SID_NAME_USE nameUser;
    LookupAccountNameA(NULL, computerAndUser, userSid, &sidSize, domain, &domainSize, &nameUser);

    HKEY hkey;
    DWORD d;
    PSID everyoane = malloc(SECURITY_MAX_SID_SIZE);
    PSECURITY_DESCRIPTOR sD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
    DWORD dwRes;
    SECURITY_ATTRIBUTES sA;
    DWORD dpsidEveryoane;
    CreateWellKnownSid(WinWorldSid, NULL, everyoane, &dpsidEveryoane);

    EXPLICIT_ACCESS ea[2];
    ea[0].grfAccessMode = SET_ACCESS;
    ea[0].grfAccessPermissions = KEY_READ | KEY_WRITE;
    ea[0].grfInheritance = NO_INHERITANCE;
    ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
    ea[0].Trustee.ptstrName = (char*)userSid;

    ea[1].grfAccessMode = SET_ACCESS;
    ea[1].grfAccessPermissions = 0;
    ea[1].grfInheritance = NO_INHERITANCE;
    ea[1].Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea[1].Trustee.TrusteeType = TRUSTEE_IS_USER;
    ea[1].Trustee.ptstrName = (char*)everyoane;

    PACL pA = NULL;
    dwRes = SetEntriesInAcl(2, ea, NULL, &pA);
    if (dwRes != ERROR_SUCCESS)
    {
        printf("error to SetEntriesInAcl: %d", dwRes);
        return 0;
    }

    if (!InitializeSecurityDescriptor(sD, SECURITY_DESCRIPTOR_REVISION))
    {
        printf("error to InitializeSecurityDescriptor");
        return 0;
    }
    if (!SetSecurityDescriptorDacl(sD, true, pA, false))
    {
        printf("error to SetSecurityDescriptorDacl");
        return 0;
    }
    if (!SetSecurityDescriptorSacl(sD, false, NULL, false))
    {
        printf("error to SetSecurityDescriptorSacl");
        return 0;
    }

    sA.nLength = sizeof(SECURITY_ATTRIBUTES);
    sA.lpSecurityDescriptor = sD;
    sA.bInheritHandle = TRUE;
    RegCreateKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\CSSO\\Tema5", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, &sA, &hkey, &d);
    return 1;
}

int main()
{
    if (createRestrictedWrite() == 0)
    {
        exit(0);
    }
    if (createLOGS() == 0)
    {
        exit(0);
    }
    if (createKeyP1() == 0)
    {
        exit(0);
    }
}
