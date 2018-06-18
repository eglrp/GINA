clc;
clear all;
close all;
fileName1 = 'example_trajectory_out_steady_state.gina';
fileName2 = 'example_imu_out_steady_state.gina';
t_data = create_data(fileName1);
%p_data = create_data(fileName2);

%koordinttes of path 
x = t_data.x;
y = t_data.y;
z = t_data.z;

%koordinttes of vectors 
% u = p_data.ax;
% v = p_data.ay;
% w = p_data.az;

% u = gradient(x);
% v = gradient(y);
% w = gradient(z);

% straching the vector, sacele = 0 -> autoscale 
scale = 0;

figure

quiver3(x,y,z,1,1,1,scale)
hold on

plot3(x,y,z)


line(x(1),y(1),z(1), 'Marker', 'o', ...
    'Color', 'b', 'MarkerFaceColor', 'b', 'MarkerSize', 5);
line(x(end),y(end),z(end), 'Marker', 's', ...
    'Color', 'b', 'MarkerFaceColor', 'b', 'MarkerSize', 8);
hold off

