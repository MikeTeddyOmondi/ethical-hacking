# Reverse Shell using C++ (w/ Win32 API)

Compiling linking the ws2_32 library file:

```sh
g++ -o rev-shell.exe rev-shell.cpp -lws2_32
```

## To create a Windows service:

1. **Define Service Control Handler:**
  
  - Define a function to handle service control requests (e.g., `ServiceCtrlHandler`):
  
  ```cpp
  // Service control handler function
  DWORD WINAPI ServiceCtrlHandler(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext);
  ```
  
2. **Define Service Main Function:**
  
  - Define the main service function (`ServiceMain`) where you'll set up the service registration and initialize resources:
  
  ```cpp
  // Service main function
  void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv);
  ```
  
3. **Service Registration:**
  
  - In the `ServiceMain` function, set up the service registration using `StartServiceCtrlDispatcher`:
  
  ```cpp
  void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv)
  {
      // Set up service registration
      SERVICE_TABLE_ENTRY ServiceTable[] =
      {
          { L"MyServiceName", ServiceMain },
          { NULL, NULL }
      };
  
      StartServiceCtrlDispatcher(ServiceTable);
  }
  ```
  
4. **Service Control Handler Implementation:**
  
  - Implement the `ServiceCtrlHandler` function to handle start, stop, and other control requests:
  
  ```cpp
  DWORD WINAPI ServiceCtrlHandler(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext)
  {
      switch (dwControl)
      {
          case SERVICE_CONTROL_STOP:
              // Implement service stop logic
              // Stop reverse shell and cleanup
              // Set service status to SERVICE_STOP_PENDING
              break;
          // Other control cases as needed
      }
  
      return NO_ERROR;
  }
  ```
  
5. **Start the Reverse Shell:**
  
  - Within your service's start logic, integrate your reverse shell code:
  
  ```cpp
  void StartReverseShell()
  {
      // Implement reverse shell setup
      // Set up socket connection
      // Execute command logic
  }
  ```
  
6. **Service Entry Point:**
  
  - Define the main entry point function and call `StartReverseShell` when the service starts:
  
  ```cpp
  int _tmain(int argc, _TCHAR* argv[])
  {
      // Set up service status
      // Register service control handler
      // Call StartReverseShell() when service starts
      // Implement service control loop
  }
  ```

### Sample source code (creating a Windows service)

```cpp
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

SERVICE_STATUS        g_ServiceStatus = { 0 };
SERVICE_STATUS_HANDLE g_StatusHandle = NULL;
HANDLE                g_ServiceStopEvent = NULL;

void ServiceCtrlHandler(DWORD);
void ServiceMain(DWORD, LPTSTR*);

void StartReverseShell()
{
    // Your reverse shell code here
    // Set up socket connection
    // Execute command logic
}

void ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv)
{
    g_StatusHandle = RegisterServiceCtrlHandler(L"MyServiceName", ServiceCtrlHandler);

    if (g_StatusHandle == NULL)
    {
        return;
    }

    g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_ServiceStatus.dwServiceSpecificExitCode = 0;

    SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

    g_ServiceStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (g_ServiceStopEvent == NULL)
    {
        // Handle error
        return;
    }

    StartReverseShell();

    WaitForSingleObject(g_ServiceStopEvent, INFINITE);

    CloseHandle(g_ServiceStopEvent);
}

void ServiceCtrlHandler(DWORD dwCtrl)
{
    switch (dwCtrl)
    {
        case SERVICE_CONTROL_STOP:
            if (g_ServiceStatus.dwCurrentState != SERVICE_RUNNING)
                break;

            // Stop reverse shell and cleanup
            g_ServiceStatus.dwControlsAccepted = 0;
            g_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
            g_ServiceStatus.dwWin32ExitCode = 0;
            g_ServiceStatus.dwCheckPoint = 4;

            SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

            // Signal the service to stop
            SetEvent(g_ServiceStopEvent);
            break;
    }
}

int main(int argc, char *argv[])
{
    SERVICE_TABLE_ENTRY ServiceTable[] =
    {
        { L"MyServiceName", (LPSERVICE_MAIN_FUNCTION)ServiceMain },
        { NULL, NULL }
    };

    StartServiceCtrlDispatcher(ServiceTable);

    return 0;
}
```
