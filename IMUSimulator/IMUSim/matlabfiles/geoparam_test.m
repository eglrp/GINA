clear all;
positon_matrix = dlmread('positions_data.txt');
maxmial_absolute_gatratian_differencial =  0.0001;
matlab_geoparam = fopen('matlab_geoparam_result.txt','wt'); %wt így jó nem tudom miez 
for i=1:length(positon_matrix)
    [Rn, Re, g, sL, cL, WIE_E] = geoparam(positon_matrix(i,:));
    fprintf(matlab_geoparam,'%d\n',g);
end
fclose(matlab_geoparam);

 matlab_geoparam = fopen('matlab_geoparam_result.txt','rt');
 matlab_result =  fscanf(matlab_geoparam,'%f');
 fclose(matlab_geoparam);  

own_geoparam = fopen('own_geoparam_result.txt','rt');
own_result = fscanf(own_geoparam,'%f');
fclose(own_geoparam);

differencail = matlab_result-own_result;
abs_quad_diff = abs(matlab_result.^2-own_result.^2);

if max(abs_quad_diff) > maxmial_absolute_gatratian_differencial
    disp('The maximum absolute quadratic difference is more than the defined value!')
    error('The maximum absolute quadratic difference is more than the defined value!')
else
    disp('The maximum absolute quadratic difference is less than the defined value!')
end

f1 = figure('Name','Maximum Absolute Quadratic Difference','NumberTitle','off');
bar(abs_quad_diff);
nbinds= length(abs_quad_diff);
title('Maximum Absolute Quadratic Difference')
xlabel('Nomber of position data')
ylabel('Difference^2')

