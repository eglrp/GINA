clc;
clear all;
close all;
addpath(fullfile(pwd,'plot'))
addpath(fullfile(pwd,'pinaFiles'))
addpath(fullfile(pwd, 'read'))
addpath(fullfile(pwd,'read', 'calculations'))

ciricle_trajectory = 'ciricle_trajectory.pina';
t_data = create_data(ciricle_trajectory);
i_data = create_data('ciricle_imu.pina');

enu_plot(t_data,'enu','circle_enu_plot_enu');
enu_plot(t_data,'2D','circle_enu_plot_2d');
enu_plot(t_data,'3D','circle_enu_plot_3d');


trajectroy3d_plot(t_data,'llh','llh','circle_trajectroy3d_llh_llh_plot');
trajectroy3d_plot(t_data,'llh','2D','circle_trajectroy3d_llh_2D_plot');
trajectroy3d_plot(t_data,'llh','3D','circle_trajectroy3d_llh_3D_plot');

trajectroy3d_plot(t_data,'ecef','xyz','circle_trajectroy3d_ecef_xyz_plot');
trajectroy3d_plot(t_data,'ecef','3D','circle_trajectroy3d_ecef_3D_plot');

attitude_plot(t_data,'circle_attitude_plot');
google_plot2(t_data,'circle_google_plot2');

vector_plot(t_data,i_data,'vector_plot')


disp('Finish!')