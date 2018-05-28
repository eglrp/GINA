function enu2_plot(t_data)
lat = t_data.startPosition(1);
lon = t_data.startPosition(2);
h = t_data.startPosition(3);



start_xyz=ecef2geo([lat,lon,h],1); % geo -> ecef 
X= start_xyz(1);
Y= start_xyz(2);
Z=start_xyz(3);
spheroid = referenceEllipsoid('WGS 84');
[e,n,u] = ecef2enu(X,Y,Z,lat,lon,h,spheroid)
figure('name','ENU')
plot3(e,n,u)
end