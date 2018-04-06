function [e,n,u] = xyz2enu(phi,lambda,x,y,z)
%XYZ2ENU  Transformation of [x;y;z] vector from geocentric to local
%   	  system. The local system has origin at (phi, lambda) 
%         that are given in degrees

%Kai Borre 21-07-99
%Copyright (c) by Kai Borre
%$Revision: 2.0 $  $Date: 2001/10/28  $

% phi = phi*pi/180;
% lambda = lambda*pi/180;

cl = cos(lambda);  
sl = sin(lambda);
cb = cos(phi);	    
sb = sin(phi);

E = []; 
N = []; 
U = [];

for i=1:length(cl)
    
    F = [-sl(i) -sb(i)*cl(i) cb(i)*cl(i);
          cl(i) -sb(i)*sl(i) cb(i)*sl(i);
          0	  cb(i)      sb(i)];
    
      local_vect = F'*[x(i); y(i); z(i)];
    
      east = local_vect(1);
      north = local_vect(2);
      up = local_vect(3);
    
    E = [E; east];
    N = [N; north];
    U = [U; up];
end

e = E;
n = N;
u = U;


%%%%%%%%% end xyz2enu.m %%%%%%%%%
