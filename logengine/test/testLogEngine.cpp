// testLogEngine.cpp : Defines the entry point for the console application.
//

#include <exception>
//#include "StdAfx.h"
#include "testProperties.h"
#include "testLine.h"
#include "testLog.h"
#include "testUtils.h"
#include "testThreadLog.h"
#include "LogEngine.h"

/*
#ifdef _WIN32
#include <windows.h>

BOOL DisplaySystemVersion(const char *filename)
{
   OSVERSIONINFOEX osvi;
   BOOL bOsVersionInfoEx;

   // Try calling GetVersionEx using the OSVERSIONINFOEX structure.
   // If that fails, try using the OSVERSIONINFO structure.

   ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

   if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
   {
      // If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
      osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
      if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
         return FALSE;
   }

   switch (osvi.dwPlatformId)
   {
      // Tests for Windows NT product family.
      case VER_PLATFORM_WIN32_NT:

      // Test for the product.

         if ( osvi.dwMajorVersion <= 4 )
            printf("Microsoft Windows NT ");

         if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
            printf ("Microsoft Windows 2000 ");


         if( bOsVersionInfoEx )  // Use information from GetVersionEx.
         { 
         // Test for the workstation type.
         /*   if ( osvi.wProductType == VER_NT_WORKSTATION )
            {
               if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
                  printf ("Microsoft Windows XP ");

               if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
                  printf ( "Home Edition " );
               else
                  printf ( "Professional " );
            }

         // Test for the server type.
            else if ( osvi.wProductType == VER_NT_SERVER )
            {
               if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
                  printf ("Microsoft Windows .NET ");

               if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                  printf ( "DataCenter Server " );
               else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                  if( osvi.dwMajorVersion == 4 )
                     printf ("Advanced Server " );
                  else
                     printf ( "Enterprise Server " );
               else if ( osvi.wSuiteMask == VER_SUITE_BLADE )
                  printf ( "Web Server " );
               else
                  printf ( "Server " );
            }*/
  /*       }
         else   // Use the registry on early versions of Windows NT.
         {
            HKEY hKey;
            char szProductType[80];
            DWORD dwBufLen;

            RegOpenKeyEx( HKEY_LOCAL_MACHINE,
               "SYSTEM\\CurrentControlSet\\Control\\ProductOptions",
               0, KEY_QUERY_VALUE, &hKey );
            RegQueryValueEx( hKey, "ProductType", NULL, NULL,
               (LPBYTE) szProductType, &dwBufLen);
            RegCloseKey( hKey );
            if ( lstrcmpi( "WINNT", szProductType) == 0 )
               printf( "Professional " );
            if ( lstrcmpi( "LANMANNT", szProductType) == 0 )
               printf( "Server " );
            if ( lstrcmpi( "SERVERNT", szProductType) == 0 )
				printf( "Advanced Server " );
         }
		 
		 // Display version, service pack (if any), and build number.
		 
         if ( osvi.dwMajorVersion <= 4 )
         {
			 printf ("version %d.%d %s (Build %d)\n",
				 osvi.dwMajorVersion,
				 osvi.dwMinorVersion,
				 osvi.szCSDVersion,
				 osvi.dwBuildNumber & 0xFFFF);
         }
         else
         { 
			 printf ("%s (Build %d)\n",
				 osvi.szCSDVersion,
				 osvi.dwBuildNumber & 0xFFFF);
         }
         break;
		 
		 // Test for the Windows 95 product family.
      case VER_PLATFORM_WIN32_WINDOWS:
		  
		  if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
		  {
			  printf ("Microsoft Windows 95 ");
			  if ( osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B' )
				  printf("OSR2 " );
		  } 
		  
		  if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
		  {
			  printf ("Microsoft Windows 98 ");
			  if ( osvi.szCSDVersion[1] == 'A' )
				  printf("SE " );
		  } 
		  
		  if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
		  {
			  printf ("Microsoft Windows Millennium Edition ");
		  } 
		  break;
   }
   return TRUE; 
}
#else
int DisplaySystemVersion(const char *filename)
{
    system("uname -a");

    std::string ldd_cmd("ldd ");
    ldd_cmd.append(filename);
    system(ldd_cmd.c_str());
}
#endif /* _WIN32 */


int main(int argc, char* argv[])
{
//	DisplaySystemVersion(argv[0]);

	try
	{
		testUtils();
		testLines();
        testProperties();
		testLogs();
		testThreadsLog();	
        _getch();
	}
	catch(std::exception& ex)
	{
		printf("\nERROR : %s\n", ex.what());
	}

	return 0;
}

