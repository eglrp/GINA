%http://www.instk.org/Tk/TransFunctions/index.html
%Given latitude and longtitude and wander angle, 
%computes Cne tranformation matrix (Navigation frame to ECEF DCM)

%wander=[sin(alpha);cos(alpha)] (optional)
%(rotate geo frame around z with "wander angle" to obtain nav frame)

function ned_plot(t_data) % ez balkezes ?
start_llh = t_data.startPosition_LLH;
C_ne= llh2dcm(start_llh);
C_en= transpose(C_ne);
 
for i = 1:length(t_data.lat)
     pos_llh = [ t_data.lat(i) t_data.lon(i) t_data.h(i) ]';
     pos_n(i,:) = C_en*pos_llh;
     disp(pos_n)
end

n = pos_n(:,1);
e = pos_n(:,2);
d = pos_n(:,3);

figure
plot(e,n)
title('East-North-Down koordináta rendszer,East-North vetülte')
xlabel('east [m]')
ylabel('north [m]')
% min_diff = 0.1 %[m]
% x_diff = max(e) - min(e);
% if x_diff < min_diff
%     xmax = 2*max(e);
%     xmin = 0;
%     xlim([xmax xmin])
% end 
% 
% y_diff = max(n) - min(n);
% if y_diff < min_diff
%     ymax = 2*max(n);
%     ymin = 0;
%     ylim([ymax ymin])
% end 
% 




