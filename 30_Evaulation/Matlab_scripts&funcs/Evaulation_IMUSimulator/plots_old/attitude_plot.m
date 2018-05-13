% Külön ploton ábrázolni a beállási szöget. roll pitch yaw, 3 subploton OK
function attitude_plot(t_data) 
% % legyen save struktúra,
% és egy kapcsaoló jhogy mentesen vagy ne, Kommentezve nem jó ! 
    figure
    title('Roll,pitch és yaw szögek')
    subplot(3,1,1);
    plot(t_data.roll);
    
    xlabel('Idõlépés');% idõlépés-> idõ [s] vagy valami 
    ylabel('Roll [rad]');
    title('Roll');
    grid on;

    subplot(3,1,2);
    plot(t_data.pitch);
    xlabel('Idõlépés');
    ylabel('Pitch [rad]');
    title('Pitch');
    grid on;
    
    
    subplot(3,1,3);
    plot(t_data.yaw);
    xlabel('Idõlépés');
    ylabel('Yaw [rad]');
    title('Yaw');
    grid on;
% varargin varargout = functuon overload 

%     Save to file
%     print(filenamewithfullpath,'-dpng')
%     h=gcf;
%     set(h,'PaperOrientation','landscape');
%     set(h,'PaperUnits','normalized');
%     set(h,'PaperPosition', [0 0 1 1]);
%     print(filenamewithfullpath,'-dpdf')
%     savefig(h,filenamewithfullpath)
%     close all
end 