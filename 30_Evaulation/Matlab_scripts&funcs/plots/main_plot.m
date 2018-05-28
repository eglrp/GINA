clc;
clear all;
close all;
addpath('C:\Users\ACER\Desktop\szakdolgozat\plots\plot')
addpath('C:\Users\ACER\Desktop\szakdolgozat\plots\pinaFiles')
addpath('C:\Users\ACER\Desktop\szakdolgozat\plots\read')
addpath('C:\Users\ACER\Desktop\szakdolgozat\plots\read\calculations')

ciricle_trajectory = 'ciricle_trajectory.pina';
t_data = create_data(ciricle_trajectory);
i_data = create_data('ciricle_imu.pina');

enu_plot(t_data,'enu','circle_enu_plot_enu');
enu_plot(t_data,'2D','circle_enu_plot_2d');
enu_plot(t_data,'3D','circle_enu_plot_3d');


trajectroy3d_plot(t_data,'llh','llh','circle_trajectroy3d_llh_llh_plot');
trajectroy3d_plot(t_data,'llh','2D','circle_trajectroy3d_llh_2D_plot');
trajectroy3d_plot(t_data,'llh','3D','circle_trajectroy3d_llh_3D_plot');

trajectroy3d_plot(t_data,'ecef','xyz','circle_trajectroy3d_llh_plot');
trajectroy3d_plot(t_data,'ecef','3D','circle_trajectroy3d_llh_plot');

attitude_plot(t_data,'circle_attitude_plot');
google_plot2(t_data,'circle_google_plot2');

vector_plot(t_data,i_data,'vector_plot')




%lat_long_plot(t_data);
%vector_plot(t_data,i_data)
% gP_file_name.mode = 'kinematic' % or nothing
% gP_file_name.name = 'googlePlotOutput'
%fileName = 'example_trajectory_out_steady_state.pina';
%fileName1 = 'example_trajectory_out_vx_1ms_yaw_1.51.pina';
%fileName2 = 'example_imu_out_steady_state_1ms_yaw_1.51.pina';

%fileName3 = 'example_trajectory_out_1ms_yaw_1.51.pina'
%fileName2 = 'example_trajectory_out_vx_1ms.pina';

%steady_state_IMU_vx_1ms  = 'example_imu_out_steady_state_vx_1ms.pina';