classdef ReferenceReader_kinematic_w_reference < GNSS_Visualization.IReferenceReader
    
    
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
        function kinematicRef = read_kinematic_ref(this,varargin)
            reference   = [];
            filename    = [];
            if ~isempty(varargin{1}) && ischar(varargin{1})
                filename    =  which(varargin{1}); 
            end
           	if ~isempty(filename)
             	[pathstr,name,ext] = fileparts(filename);
                if strcmp(ext,'.mat')
                    try
                        reference = load(filename); 
                        if isfield(reference,'day1')
                            reference = reference.day1;
                        end
                        if ismatrix(reference)
                            [~,sizeofref] = size(reference);
                            if sizeofref ~= 5
                                reference = [];
                            end
                        else
                            reference = [];
                        end
                    catch ME
                      	warning(ME.message)
                        reference = [];
                    end
      
                end
                if ~isempty(reference)
                    gpssow  = reference(:,2);
                    gpsweek = [];
                    latrad  = reference(:,3);
                    longrad = reference(:,4);
                    high    = reference(:,5);
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
            else
             	kinematicRef = GNSS_Visualization.NullReference();
            end      
        end
        
    end
end