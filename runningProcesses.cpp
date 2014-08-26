#include <QObject>
#include <windows.h>
#include <sstream>
#include <string>
#include <tlhelp32.h>
#include <stdio.h>
#include <process.h>
#include <stdlib.h>
#include <assert.h>

#include "programLogTime.h"
#include "fioLogger.h"
#include "runningProcesses.h"

#define BUFFER_SIZE 100
/*
 * class is used to monitor open processes
 * the includes much like other classes in the program
 * are in a specific order
 */

//starts a new thread
void RunningProcesses::startThread()
{
    _beginthread(RunningProcesses::enterThread, 0, this);
}

void RunningProcesses::enterThread(void *p)
{
    ((RunningProcesses *) p)->threadBody();
    _endthread();
    return;
}
//gets the process list
void RunningProcesses::threadBody()
{
    Sleep(10000); //prevents the thread from running too often
    GetProcessList();
}
//records the process list
BOOL RunningProcesses::GetProcessList( )
{
  HANDLE hProcessSnap;
  HANDLE hProcess;
  PROCESSENTRY32 pe32;
  DWORD dwPriorityClass;
  processes = "";

  // Take a snapshot of all processes in the system.
  hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
  if( hProcessSnap == INVALID_HANDLE_VALUE )
  {
   // processes += "CreateToolhelp32Snapshot (of processes)";
    return( FALSE );
  }

  // Set the size of the structure before using it.
  pe32.dwSize = sizeof( PROCESSENTRY32 );

  // Retrieve information about the first process,
  // and exit if unsuccessful
  if( !Process32First( hProcessSnap, &pe32 ) )
  {
    printError( TEXT("Process32First") ); // show cause of failure
    CloseHandle( hProcessSnap );          // clean the snapshot object
    return( FALSE );
  }

  // Now walk the snapshot of processes, and
  // display information about each process in turn
  std::string s;
  char ch[260];
  char DefChar = ' ';

  do
  {
      WideCharToMultiByte(CP_ACP,0,pe32.szExeFile,-1, ch,260,&DefChar, NULL);
      s = ch;

             processes += "<font color = green>=====================================================</font><br>";
             processes += "<font color = blue>PROCESS NAME:</font> <font color = black>" + s + "</font><br>";



    // Retrieve the priority class.
    dwPriorityClass = 0;
    hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID );
    if( hProcess == NULL )
      processes += "";
    else
    {
      dwPriorityClass = GetPriorityClass( hProcess );
      if( !dwPriorityClass )
        processes += "";
      CloseHandle( hProcess );
    }

       std::ostringstream stream;
       stream << pe32.th32ProcessID;
       std::string str = stream.str();
    processes += "<font color=purple>  Process ID        </font><font color=black>= </font><font color = mediumvioletred>" + str + "</font>";
       stream << pe32.cntThreads;
       str = stream.str();
    if( dwPriorityClass ) {
     // processes += "\n  Priority class    = %d" + dwPriorityClass;
    }
    // List the modules and threads associated with this process
    ListProcessModules( pe32.th32ProcessID );


  } while( Process32Next( hProcessSnap, &pe32 ) );

  CloseHandle( hProcessSnap );
  processfname = "processinfo";
  processfnametemp = "tempprocessinfo";
  ftpo.writeToTempLog(processfnametemp,("<font color=\"red\"><h2>" + tpo.getCurrentDate() + "</h2></font>" += processes));
  ftpo.writeToLog(processfname,(tpo.getCurrentDate() += processes));
  runningProcessesUpdated();
  startThread();
  return( TRUE );
}

//adds in the modules for each of the processes
BOOL RunningProcesses::ListProcessModules( DWORD dwPID )
{
  HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
  MODULEENTRY32 me32;

  // Take a snapshot of all modules in the specified process.
  hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, dwPID );
  if( hModuleSnap == INVALID_HANDLE_VALUE )
  {
   // processes += "CreateToolhelp32Snapshot (of modules)";
    return( FALSE );
  }

  // Set the size of the structure before using it.
  me32.dwSize = sizeof( MODULEENTRY32 );

  // Retrieve information about the first module,
  // and exit if unsuccessful
  if( !Module32First( hModuleSnap, &me32 ) )
  {
    //processes += "Module32First";  // show cause of failure
    CloseHandle( hModuleSnap );           // clean the snapshot object
    return( FALSE );
  }

  // Now walk the module list of the process,
  // and display information about each module
  std::string s;
  char ch[260];
  char DefChar = ' ';
  do
  {
    WideCharToMultiByte(CP_ACP,0,me32.szExePath,-1, ch,260,&DefChar, NULL);
    s = ch;
    processes += "<br><br>     <font color=purple>Executable </font>    = <font color = brown>" + s + "</font>";
    std::ostringstream stream;
    stream << me32.th32ProcessID;
    std::string str = stream.str();
    processes += "<br>    <font color=purple> Process ID  </font>   =<font color = mediumvioletred> " + str + "</font>";

  } while( Module32Next( hModuleSnap, &me32 ) );
    processes += "<br><font color = green>--------------------------------------------------------------------------</font><br><br>";
  CloseHandle( hModuleSnap );

  return( TRUE );
}

//if there is some sort of error in retrieving a process,
//aka no sufficient admint privilidges etc
void RunningProcesses::printError( TCHAR* msg )
{
  DWORD eNum;
  TCHAR sysMsg[256];
  TCHAR* p;

  eNum = GetLastError( );
  FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
         NULL, eNum,
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
         sysMsg, 256, NULL );

  // Trim the end of the line and terminate it with a null
  p = sysMsg;
  while( ( *p > 31 ) || ( *p == 9 ) )
    ++p;
  do { *p-- = 0; } while( ( p >= sysMsg ) &&
                          ( ( *p == '.' ) || ( *p < 33 ) ) );
}
//emits when some of the processes have changed
void RunningProcesses::runningProcessesUpdated()
{
    emit runningProcessesChange();
}
