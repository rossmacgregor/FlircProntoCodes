@echo off
FOR /L %%C IN (%2,1,%3) DO FlircProntoCodes\Debug\FlircProntoCodes.exe %1 %%C dec
@echo on
