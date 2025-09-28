@echo off

REM Сборка отдельных объектных файлов
g++ -std=c++17 -Iinclude -c src\gurobi.cpp -o gurobi.o
g++ -std=c++17 -Iinclude -c src\main.cpp -o main.o
g++ -std=c++17 -Iinclude -c src\prep_functions.cpp -o prep_functions.o
g++ -std=c++17 -Iinclude -c src\preprocessing.cpp -o preprocessing.o

REM Линковка в exe
g++ gurobi.o  main.o preprocessing.o prep_functions.o -o prog.exe  -LC:\gurobi1203\lib -lgurobi_c++ -lgurobi120

REM Запуск программы
prog.exe

@REM pause