copy Objects\T5L51.hex Download
cd Download
srec_cat.exe T5L51.hex -Intel -o T5L51.bin -Binary 
echo.
copy "T5L51.bin" ..\
copy "T5L51.bin" E:\DWIN_SET

