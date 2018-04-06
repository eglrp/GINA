classdef ReferenceReader_kinematic_w_iMAR_EU_test_01_reference < GNSS_Visualization.IReferenceReader

    methods 
        function Ref = Read(this,varargin)
            if nargin >=1 
                Ref = read_iMAR_ref(this,varargin{1});
            else
                Ref = read_iMAR_ref(this,[]);
            end
           	if sum(cellfun(@(x) strcmp('GNSS_Visualization.IReference',x),superclasses(Ref)))==0
                Ref = GNSS_Visualization.NullReference();
            end
        
        end
    end

    methods (Access = private)
        function iMAR_ref = read_iMAR_ref(this,varargin)
            reference   = [];
            filename    = [];
            if ~isempty(varargin{1}) && ischar(varargin{1})
                % filename    =  which(varargin{1}); 
                filename    =  fullfile(pwd,varargin{1});
            end
           	if ~isempty(filename)
             	[pathstr,name,ext] = fileparts(filename);
                
                    try
                        reference = load([filename]); 
                        iMAR_ref = GNSS_Visualization.kinematicReference();
                        
                        iMAR_ref.GPSSoW     = reference.GPSTime;
                        iMAR_ref.GPSWeek    = [];
                        iMAR_ref.LatRad     = reference.Latitude;
                        iMAR_ref.LonRad     = reference.Longitude;
                        iMAR_ref.High       = reference.HEll;
                        iMAR_ref.LatDeg     = reference.Latitude*180/pi;
                        iMAR_ref.LonDeg     = reference.Longitude*180/pi;
                    
                    catch ME
                      	warning(ME.message)
                        reference = [];
                    end

            end
            if isempty(reference)
               iMAR_ref = GNSS_Visualization.NullReference(); 
            end
        end
    end    
end
