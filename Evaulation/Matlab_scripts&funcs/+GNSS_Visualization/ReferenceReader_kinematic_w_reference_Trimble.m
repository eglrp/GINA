classdef ReferenceReader_kinematic_w_reference_Trimble < GNSS_Visualization.IReferenceReader
    
    
    methods
       
        function this = ReferenceReader_kinematic_w_reference(varargin)
            
        end
        
        function Ref = Read(this,varargin)
            if nargin >=2 
                Ref = read_kinematic_ref(this,varargin{1});
            else
                Ref = read_kinematic_ref(this,[]);
            end
           	if sum(cellfun(@(x) strcmp('GNSS_Visualization.IReference',x),superclasses(Ref)))==0
                Ref = GNSS_Visualization.NullReference();
            end
        end
        
    end
    
    methods (Access = private)
        function kinematicRef = read_kinematic_ref(~,varargin)
            reference   = [];
            filename    = [];
            
            if ~isempty(varargin{1}) && ischar(varargin{1})
                filename = varargin{1}; 
            end
            
           	if ~isempty(filename)
             	[~,~,ext] = fileparts(filename);
                
                % Parse reference trajectory from .txt file
                % Trimble rower simulator
                
                if strcmp(ext,'.txt') 
                    try
                        
                        fID = fopen(filename,'r+');
                        
                        if (fID < 0)
                           throw(MException('ReferenceReader:Trimble:IOError',...
                               ['Failed to open ' filename])); 
                        end
                        
                        numLines = 1;
                        
                        while true
                            line = fgetl(fID);                            
                            
                            if line == -1
                                break
                            end
                            
                            numLines = numLines + 1;
                        end
                        
                        reference = zeros(numLines-1, 5);
                        
                        fseek(fID, 0, -1);

                        numLines = 1;
                        
                        while true
                           line = fgetl(fID);
                           
                           if line == -1
                              break 
                           end
                           
                           reference_data = strsplit(line,';');
                           
                           reference(numLines,1) = str2double(reference_data{1});
                           reference(numLines,2) = str2double(reference_data{2});
                           reference(numLines,3) = str2double(reference_data{3});
                           reference(numLines,4) = str2double(reference_data{4});
                           reference(numLines,5) = str2double(reference_data{5});
                           
                           numLines = numLines + 1;
                        end                        

                    catch ME
                      	warning(ME.message)
                        reference = [];
                    end
      
                end
                
                if ~isempty(reference)
                    gpsweek = reference(:,1);
                    gpssow  = reference(:,2);
                    
                    [latrad, longrad, high] = ecef2llh(reference(:,3),...
                        reference(:,4),reference(:,5));
                else
                    gpssow  = [];
                    gpsweek = [];
                    latrad  = [];
                    longrad = [];
                    high    = [];
                end
                
                kinematicRef = GNSS_Visualization.kinematicReference();
                kinematicRef.GPSSoW     = gpssow;
                kinematicRef.GPSWeek    = gpsweek;
                kinematicRef.LatRad     = latrad;
                kinematicRef.LonRad     = longrad;
                kinematicRef.High       = high;
                kinematicRef.LatDeg     = latrad*180/pi;
                kinematicRef.LonDeg     = longrad*180/pi;
                kinematicRef.X          = reference(:,3);
                kinematicRef.Y          = reference(:,4);
                kinematicRef.Z          = reference(:,5);
            else
             	kinematicRef = GNSS_Visualization.NullReference();
            end      
        end
        
    end
end