@echo off
cls

setlocal

set server_dir=C:\Users\user\Documents\Visual Studio 2013\Projects\Server\Server
set client_dir=C:\Users\user\Documents\Visual Studio 2013\Projects\Client\Client

rem  /B           ���� �������� �������� ������.
rem  /V           �������� ������������ ����������� ������.
rem  /Y           ���������� ������� ������������� �� ���������� �������������
rem               ��������� �����.

copy "%server_dir%\MessageFunc.h" "%client_dir%\" /B /V /Y
copy "%server_dir%\MessageFunc.cpp" "%client_dir%\" /B /V /Y

endlocal
EXIT /B %ERRORLEVEL%
