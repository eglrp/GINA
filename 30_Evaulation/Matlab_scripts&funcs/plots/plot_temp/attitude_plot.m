% Külön ploton ábrázolni a beállási szöget. roll pitch yaw, 3 subploton OK
function attitude_plot(varargin) 
switch length(varargin)
    case 1
        t_data = varargin{1};
        save_function = false;
    case 2
        t_data = varargin{1};
        png_name = cell2mat(varargin(2));
        save_function = true;      
end
    f1 = figure('name','roll pitch yaw');
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

    if save_function == true
        %png_name = cell2mat(varargin(2));
        folder_of_save = '\result\';
        save_name_with_path = [pwd folder_of_save png_name];
        saveas(f1,[save_name_with_path '.png'])
        saveas(f1,[save_name_with_path,'.fig'])
    end
end 