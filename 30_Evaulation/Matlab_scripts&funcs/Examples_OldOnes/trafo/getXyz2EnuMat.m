function F = getXyz2EnuMat(phi,lambda)
%getXyz2EnuMat()  returns a matrix to tranform a cartensien vector into a ENU (east north up)
%coordinatw system

%Markus Langer (CC-PS\ECS4), 22.12.2015
%http://www.navipedia.net/index.php/Transformations_between_ECEF_and_ENU_coordinates

cl = cos(lambda);  sl = sin(lambda);
cb = cos(phi);	    sb = sin(phi);

F = [-sl         cl       0;
      -cl*cb  -sb*sl     cb;
      cl*cb	    sl*cb     sb];

