
Egy apró tipp, hogy Eigen osztályok is debugolhatóak legyenek, 
azaz ne csak egy datapointert adjon a debug értékük, hanem rendesen sor,
oszlop propertykbe listázza az adatokat.

Ide kellene ezt bemásolni :
C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\Packages\Debugger\Visualizers 

Hol találod?
http://www.eigen.tuxfamily.org/index.php?title=Developer%27s_Corner#Debugging_under_Visual_Studio 
itt az első bekezdés
van egy link az eigen bitbuckethez

onnan ki copyztam a tartalmat, csináltam lokálban egy eigen.natvis fájlt és bemásoltam a célhelyre
mivel direkt downlaod linket nem találtam

illetve féligmeddig idejön
IO format
https://eigen.tuxfamily.org/dox/structEigen_1_1IOFormat.html 
