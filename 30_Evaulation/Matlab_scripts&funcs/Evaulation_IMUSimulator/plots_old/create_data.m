
function p_data = create_data(fileName) % p_data = .pina data
%% reada a .pina file into a struct of h_data and d_data. 
%( h_data ->header , d_data -> the rest of the .pina file):
h_data = read_header(fileName);

data = read_data(fileName);
d_data = make_pina_format(data,h_data);

[h_data,d_data]= calculate_other_format(h_data,d_data); % fomarts:llh,ecef

%% merge h_data and d_data structs:
f = fieldnames(h_data);
 for i = 1:length(f)
    p_data.(f{i}) = h_data.(f{i})
 end 
 
 f = fieldnames(d_data);
 for i = 1:length(f)
    if strcmp('format',f{i}) ~= 1 %d_data.fomramt unnecessary,(h_data.positionFormat)
        p_data.(f{i}) = d_data.(f{i})
    end
 end
 disp(p_data);
end 