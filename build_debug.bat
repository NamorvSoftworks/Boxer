call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

set msbuildemitsolution=1
msbuild.exe Boxer.sln -m /p:Configuration=Debug
