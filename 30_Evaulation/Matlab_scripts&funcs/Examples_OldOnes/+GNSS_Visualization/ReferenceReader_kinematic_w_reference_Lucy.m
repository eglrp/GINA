classdef ReferenceReader_kinematic_w_reference_Lucy < GNSS_Visualization.IReferenceReader

    methods 
        function Ref = Read(this,varargin)
            if nargin >=1 
                Ref = read_Lucy_ref(this,varargin{1});
            else
                Ref = read_Lucy_ref(this,[]);
            end
           	if sum(cellfun(@(x) strcmp('GNSS_Visualization.IReference',x),superclasses(Ref)))==0
                Ref = GNSS_Visualization.NullReference();
            end
        
        end
    end

    methods (Access = private)
        function Lucy_ref = read_Lucy_ref(this,varargin)
            reference   = [];
            filename    = [];
            if ~isempty(varargin{1}) && ischar(varargin{1})
                % filename    =  which(varargin{1}); 
                filename    =  fullfile(pwd,varargin{1});
            end
           	if ~isempty(filename)
             	[pathstr,name,ext] = fileparts(filename);
                
                    try
                        reference = load(filename); 
                        Lucy_ref = GNSS_Visualization.kinematicReference();
                        
                        Lucy_ref.GPSSoW     = reference.Ref.pos_lat.time;
                        Lucy_ref.GPSWeek    = [];
                        Lucy_ref.LatRad     = reference.Ref.pos_lat.signals.values*pi/180;
                        Lucy_ref.LonRad     = reference.Ref.pos_lon.signals.values*pi/180;
                        Lucy_ref.High       = reference.Ref.pos_height.signals.values;
                        Lucy_ref.LatDeg     = reference.Ref.pos_lat.signals.values;
                        Lucy_ref.LonDeg     = reference.Ref.pos_lon.signals.values;
                    
                    catch ME
                      	warning(ME.message)
                        reference = [];
                    end

            end
            if isempty(reference)
               Lucy_ref = GNSS_Visualization.NullReference(); 
            end
        end
    end    
end
