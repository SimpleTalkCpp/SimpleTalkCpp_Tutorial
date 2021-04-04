@REM #change current directory to this file

@rem change driver letter
@%~d0

@rem change current directory
@cd %~dp0

mkdir _build
cmake -B _build .

@pause